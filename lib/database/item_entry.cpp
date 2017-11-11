/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "id_to_index.hpp"
#include "id_to_string.hpp"
#include "../misc_common.hpp"

#include <pkmn/config.hpp>
#include <pkmn/database/item_entry.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/config.hpp>
#include <boost/format.hpp>

#include <stdexcept>

namespace pkmn { namespace database {

    static pkmn::database::sptr _db;

    static int version_group_id_to_item_list_id(
        int item_id,
        int version_group_id
    ) {
        if(item_id == 0 or version_group_id == 0) {
            return 0;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT libpkmn_list_id FROM veekun_pocket_to_libpkmn_list "
            "WHERE version_group_id=? AND veekun_pocket_id=(SELECT "
            "pocket_id FROM item_categories WHERE id=(SELECT category_id "
            "FROM items WHERE id=?))";

        return pkmn::database::query_db_bind2<int, int, int>(
                   _db, query, version_group_id, item_id
               );
    }

    /*
     * TM/HM detection
     */
    BOOST_STATIC_CONSTEXPR int TM01  = 305;
    BOOST_STATIC_CONSTEXPR int HM01  = 397;
    BOOST_STATIC_CONSTEXPR int HM08  = 404;
    BOOST_STATIC_CONSTEXPR int TM93  = 659;
    BOOST_STATIC_CONSTEXPR int TM95  = 661;
    BOOST_STATIC_CONSTEXPR int TM96  = 745;
    BOOST_STATIC_CONSTEXPR int TM100 = 749;

    static PKMN_CONSTEXPR_OR_INLINE bool item_id_is_tm(
        int item_id
    ) {
        return (item_id >= TM01 and item_id < HM01) or
               (item_id >= TM93 and item_id <= TM95) or
               (item_id >= TM96 and item_id <= TM100);
    }

    static PKMN_CONSTEXPR_OR_INLINE bool item_id_is_hm(
        int item_id
    ) {
        return (item_id >= HM01 and item_id <= HM08);
    }

    static PKMN_CONSTEXPR_OR_INLINE bool item_id_is_tmhm(
        int item_id
    ) {
        return item_id_is_tm(item_id) or item_id_is_hm(item_id);
    }

    /*
     * In Generation II, Apricorns were placed in the "Items" pocket instead
     * of separate storage.
     */
    BOOST_STATIC_CONSTEXPR int APRICORN_LIST_IDS[] = {
        -1, // None
        -1, // Red/Blue
        -1, // Yellow
        5,  // Gold/Silver
        10, // Crystal
    };

    /*
     * Veekun's database does not distinguish berries from other healing items,
     * but they go in separate pockets in every game past Generation II, so this
     * overrides the database query.
     */
    BOOST_STATIC_CONSTEXPR int BERRY_LIST_IDS[] = {
        -1, // None
        -1, // Red/Blue
        -1, // Yellow
        5,  // Gold/Silver
        10, // Crystal
        18, // Ruby/Sapphire
        24, // Emerald
        30, // FireRed/LeafGreen
        37, // Diamond/Pearl
        45, // Platinum
        53, // HeartGold/SoulSilver
        60, // Black/White
        66, // Colosseum
        73, // XD
        79, // Black 2/White 2
        84, // X/Y
        89  // Omega Ruby/Alpha Sapphire
    };

    /*
     * The pockets in which Fossils are stored change from version group to
     * version group, so we'll just override it here.
     */
    BOOST_STATIC_CONSTEXPR int FOSSIL_LIST_IDS[] = {
        -1, // None
        1, // Red/Blue
        3, // Yellow
        7,  // Gold/Silver
        12, // Crystal
        19, // Ruby/Sapphire
        25, // Emerald
        31, // FireRed/LeafGreen
        33, // Diamond/Pearl
        41, // Platinum
        49, // HeartGold/SoulSilver
        57, // Black/White
        -1, // Colosseum (no fossils)
        -1, // XD (no fossils)
        62, // Black 2/White 2
        81, // X/Y
        86  // Omega Ruby/Alpha Sapphire
    };

    item_entry::item_entry(
        int item_index,
        int game_id
    ):
        _item_index(item_index),
        _game_id(game_id),
        _none(item_index == 0)
    {
        // Connect to database
        pkmn::database::get_connection(_db);

        // Get item information. This also serves as input validation.
        try {
            _item_id = pkmn::database::item_index_to_id(
                           _item_index, _game_id
                       );
            _invalid = false;
        } catch(const std::invalid_argument&) {
            _item_id = -1;
            _invalid = true;
        }

        /*
         * Get version information. This gives us the information we need
         * to get version-specific information.
         */
        _generation = pkmn::database::game_id_to_generation(
                          _game_id
                      );
        _version_group_id = pkmn::database::game_id_to_version_group(
                                _game_id
                            );

        if(_invalid) {
            _item_list_id = -1;
        } else {
            // Overrides
            std::string name = this->get_name();
            if(name.find("Berry") != std::string::npos) {
                _item_list_id = BERRY_LIST_IDS[_version_group_id];
            } else if(_generation == 2 and name.find("Apricorn") != std::string::npos) {
                _item_list_id = APRICORN_LIST_IDS[_version_group_id];
            } else if(name.find("Fossil") != std::string::npos) {
                if(game_is_gamecube(_game_id)) {
                    throw std::invalid_argument("Gamecube games have no fossils.");
                } else {
                    _item_list_id = FOSSIL_LIST_IDS[_version_group_id];
                }
            } else {
                _item_list_id = pkmn::database::version_group_id_to_item_list_id(
                                    _item_id, _version_group_id
                                );
            }
        }
    }

    item_entry::item_entry(
        const std::string &item_name,
        const std::string &game_name
    ):
        _none(item_name == "None"),
        _invalid(false)
    {
        // Connect to database
        pkmn::database::get_connection(_db);

        /*
         * Get version information. This validates the game input and gives
         * us the information we need to get version-specific information.
         */
        _game_id = pkmn::database::game_name_to_id(
                       game_name
                   );
        _generation = pkmn::database::game_id_to_generation(
                          _game_id
                      );
        _version_group_id = pkmn::database::game_id_to_version_group(
                                _game_id
                            );

        _item_id = pkmn::database::item_name_to_id(
                       item_name
                   );
        _item_index = pkmn::database::item_id_to_index(
                          _item_id, _game_id
                      );
        _item_list_id = pkmn::database::version_group_id_to_item_list_id(
                            _item_id, _version_group_id
                        );

        // Don't allow HMs in Gamecube games
        if(game_is_gamecube(_game_id) and item_id_is_hm(_item_id)) {
            throw std::invalid_argument("Gamecube games have no HMs.");
        }

        // Overrides
        if(item_name.find("Berry") != std::string::npos) {
            _item_list_id = BERRY_LIST_IDS[_version_group_id];
        } else if(_generation == 2 and item_name.find("Apricorn") != std::string::npos) {
            _item_list_id = APRICORN_LIST_IDS[_version_group_id];
        } else if(item_name.find("Fossil") != std::string::npos) {
            if(game_is_gamecube(_game_id)) {
                throw std::invalid_argument("Gamecube games have no fossils.");
            } else {
                _item_list_id = FOSSIL_LIST_IDS[_version_group_id];
            }
        } else {
            _item_list_id = pkmn::database::version_group_id_to_item_list_id(
                                _item_id, _version_group_id
                            );
        }
    }

    std::string item_entry::get_name() const {
        std::string ret;

        if(_none) {
            ret = "None";
        } else if(_invalid) {
            ret = str(boost::format("Invalid (0x%x)") % _item_index);
        } else {
            ret = pkmn::database::item_id_to_name(
                      _item_id, _version_group_id
                  );
        }

        return ret;
    }

    std::string item_entry::get_game() const {
        return pkmn::database::game_id_to_name(
                   _game_id
               );
    }

    std::string item_entry::get_category() const {
        std::string ret;

        if(_none) {
            ret = "None";
        } else if(_invalid) {
            ret = "Unknown";
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT name FROM item_category_prose WHERE item_category_id="
                "(SELECT category_id FROM items WHERE id=?) AND local_language_id=9";

            ret = pkmn::database::query_db_bind1<std::string, int>(
                      _db, query, _item_id
                  );
        }

        return ret;
    }

    std::string item_entry::get_pocket() const {
        std::string ret;

        if(_none) {
            ret = "None";
        } else if(_invalid) {
            ret = "Unknown";
        } else {
            ret = pkmn::database::item_list_id_to_name(
                      _item_list_id
                  );
        }

        return ret;
    }

    std::string item_entry::get_description() const {
        std::string ret;

        if(_none) {
            ret = "None";
        } else if(_invalid) {
            ret = get_name();
        } else {
            /*
             * If the item is a TM/HM, ignore what the database shows
             * as the description and show what move it teaches.
             *
             * For Gamecube games, use Ruby/Sapphire to check since
             * the indices are the same, and the database doesn't
             * know those items are in the Gamecube games.
             *
             * For Generation VI, the database has some TMs associated with
             * XY but not ORAS, so just use XY for any queries.
             */
            if(item_id_is_tmhm(_item_id)) {
                BOOST_STATIC_CONSTEXPR int RS   = 5;
                BOOST_STATIC_CONSTEXPR int XY   = 15;
                BOOST_STATIC_CONSTEXPR int ORAS = 16;

                int version_group_id = _version_group_id;
                if(game_is_gamecube(_game_id)) {
                    version_group_id = RS;
                } else if(version_group_id == ORAS) {
                    version_group_id = XY;
                }

                static BOOST_CONSTEXPR const char* tmhm_move_query =
                    "SELECT name FROM move_names WHERE local_language_id=9 AND move_id="
                    "(SELECT move_id FROM machines WHERE version_group_id=? "
                    "AND item_id=?)";

                /*
                 * TM94 is different between XY and ORAS. Since is the only time that happens,
                 * just deal with it here.
                 */
                BOOST_STATIC_CONSTEXPR int TM94 = 660;
                std::string move_name;
                if(_item_id == TM94 and _generation == 6) {
                    move_name = (_version_group_id == ORAS) ? "Secret Power"
                                                            : "Rock Smash";
                } else {
                    move_name = pkmn::database::query_db_bind2<std::string, int, int>(
                                    _db, tmhm_move_query, version_group_id, _item_id
                                );
                }

                boost::format tmhm_desc("Teaches the move %s.");
                ret = str(tmhm_desc % move_name.c_str());

            } else {
                /*
                 * Veekun's database has no item flavor text for Generations I-II,
                 * so if this entry corresponds to one of those games, the query
                 * will likely fail. In that case, fall back on the flavor text
                 * from X/Y.
                 */

                static BOOST_CONSTEXPR const char* main_query = \
                    "SELECT flavor_text FROM item_flavor_text WHERE item_id=? "
                    "AND version_group_id=? AND language_id=9";

                std::string from_db = "";
                if(not pkmn::database::maybe_query_db_bind2<std::string, int, int>(
                       _db, main_query, from_db, _item_id, _version_group_id
                  ))
                {
                    static BOOST_CONSTEXPR const char* fallback_query = \
                        "SELECT flavor_text FROM item_flavor_text WHERE item_id=? "
                        "AND version_group_id=15 AND language_id=9";

                    from_db = pkmn::database::query_db_bind1<std::string, int>(
                                  _db, fallback_query, _item_id
                              );
                }

                ret = fix_veekun_whitespace(from_db);
            }
        }

        return ret;
    }

    int item_entry::get_cost() const {
        if(_none or _invalid) {
            return -1;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT cost FROM items WHERE id=?";

        return pkmn::database::query_db_bind1<int, int>(
                   _db, query, _item_id
               );
    }

    bool item_entry::holdable() const
    {
        bool ret = true;

        // Items could not be held in Generation I
        if(_none or _invalid or _generation == 1)
        {
            ret = false;
        }
        else
        {
            /*
             * Veekun's database is horribly inconsistent in its item flags,
             * probably due to contributors not caring. Rather than go through
             * and fix all of it, it's easier just to fake it here and pretend
             * we queried the database.
             */
            std::string name = this->get_name();
            if(name.find("Ball") != std::string::npos and _item_id != 1013)
            {
                // GS Ball
                ret = true;
            }
            else if((name.find("Berry") != std::string::npos) && (name != "Berry Pouch"))
            {
                ret = true;
            }
            else if(boost::algorithm::ends_with(name, "ite") and
                    (name.find("Meteor") == std::string::npos))
            {
                ret = true;
            }
            else
            {
                static BOOST_CONSTEXPR const char* query = \
                    "SELECT item_flag_id FROM item_flag_map WHERE "
                    "item_id=? AND item_flag_id IN (5,6,7)";

                PKMN_UNUSED(int result);
                ret = pkmn::database::maybe_query_db_bind1<int, int>(
                          _db, query, result, _item_id
                      );
            }
        }

        return ret;
    }

    int item_entry::get_fling_power() const {
        int ret = 0;

        // Fling was introduced in Generation IV
        if(_none or _invalid or _generation < 4) {
            ret = -1;
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT fling_power FROM items WHERE id=?";

            ret = pkmn::database::query_db_bind1<int, int>(
                      _db, query, _item_id
                  );
        }

        return ret;
    }

    std::string item_entry::get_fling_effect() const {
        std::string ret;

        // Fling was introduced in Generation IV
        if(_none or _generation < 4) {
            ret = "None";
        } else if(_invalid) {
            ret = "Unknown";
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT effect FROM item_fling_effect_prose WHERE "
                "local_language_id=9 AND item_fling_effect_id="
                "(SELECT fling_effect_id FROM items WHERE id=?)";

            // Allow for no fling effect
            if(not pkmn::database::maybe_query_db_bind1<std::string, int>(
                   _db, query, ret, _item_id
               ))
            {
                ret = "None";
            }
        }

        return ret;
    }

}}
