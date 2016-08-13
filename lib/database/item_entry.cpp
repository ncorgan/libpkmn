/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "id_to_index.hpp"
#include "id_to_string.hpp"

#include <pkmn/database/item_entry.hpp>

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

    item_entry::item_entry():
        _item_id(0),
        _item_index(0),
        _item_list_id(0),
        _game_id(0),
        _generation(0),
        _version_group_id(0),
        _none(true),
        _invalid(true)
    {}

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
                           _item_id, _game_id
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
            _item_list_id = pkmn::database::version_group_id_to_item_list_id(
                                _item_id, _version_group_id
                            );
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
                       item_name, _version_group_id
                   );
        _item_index = pkmn::database::item_id_to_index(
                          _item_id, _game_id
                      );
        _item_list_id = pkmn::database::version_group_id_to_item_list_id(
                            _item_id, _version_group_id
                        );
    }

    std::string item_entry::get_name() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return str(boost::format("Invalid (0x%x)") % _item_id);
        }

        return pkmn::database::item_id_to_name(
                   _item_id, _version_group_id
               );
    }

    std::string item_entry::get_game() const {
        return pkmn::database::game_id_to_name(
                   _game_id
               );
    }

    std::string item_entry::get_category() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM item_category_prose WHERE item_category_id="
            "(SELECT category_id FROM items WHERE id=?) AND local_language_id=9";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, _item_id
               );
    }

    std::string item_entry::get_pocket() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        return pkmn::database::item_list_id_to_name(
                   _item_list_id, _version_group_id
               );
    }

    // TM/HM detection
    BOOST_STATIC_CONSTEXPR int TM01  = 305;
    BOOST_STATIC_CONSTEXPR int HM08  = 404;
    BOOST_STATIC_CONSTEXPR int TM93  = 659;
    BOOST_STATIC_CONSTEXPR int TM95  = 661;
    BOOST_STATIC_CONSTEXPR int TM96  = 745;
    BOOST_STATIC_CONSTEXPR int TM100 = 749;
    BOOST_STATIC_CONSTEXPR bool item_id_is_tmhm(
        int item_id
    ) {
        return (item_id >= TM01 and item_id <= HM08) or
               (item_id >= TM93 and item_id <= TM95) or
               (item_id >= TM96 and item_id <= TM100);
    }

    std::string item_entry::get_description() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        /*
         * If the item is a TM/HM, ignore what the database shows
         * as the description and show what move it teaches.
         */
        if(item_id_is_tmhm(_item_id)) {
            static BOOST_CONSTEXPR const char* tmhm_move_query = \
                "SELECT move_id FROM machines WHERE version_group_id=? "
                "AND item_id=?";

            int move_id = pkmn::database::query_db_bind2<int, int, int>(
                _db, tmhm_move_query, _version_group_id, _item_id
            );
            std::string move_name = pkmn::database::move_id_to_name(
                                        move_id, _version_group_id
                                    );
            return str(boost::format("Teaches the move %s.") % move_name);

        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT flavor_text FROM item_flavor_text WHERE item_id=? "
                "AND version_group_id=? AND language_id=9";

            std::string from_db = pkmn::database::query_db_bind2<std::string, int, int>(
                                      _db, query, _item_id, _version_group_id
                                  );
            return fix_veekun_whitespace(from_db);
        }
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

    bool item_entry::holdable() const {
        // Items could not be held in Generation I
        if(_none or _invalid or _generation == 1) {
            return false;
        }

        /*
         * Veekun's database is horribly inconsistent in its item flags,
         * probably due to contributors not caring. Rather than go through
         * and fix all of it, it's easier just to fake it here and pretend
         * we queried the database.
         */
        std::string name = this->get_name();
        if(name.find("Ball") != std::string::npos and _item_id != 1013) { // GS Ball
            return true;
        } else if(name.find("Berry") != std::string::npos) {
            return true;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT item_flag_id FROM item_flag_map WHERE "
            "item_id=? AND item_flag_id IN (5,6,7)";

        PKMN_UNUSED(int result);
        return pkmn::database::maybe_query_db_bind1<int, int>(
                   _db, query, result, _item_id
               );
    }

    int item_entry::get_fling_power() const {
        // Fling was introduced in Generation IV
        if(_none or _invalid or _generation < 4) {
            return -1;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT fling_power FROM items WHERE id=?";

        return pkmn::database::query_db_bind1<int, int>(
                   _db, query, _item_id
               );
    }

    std::string item_entry::get_fling_effect() const {
        // Fling was introduced in Generation IV
        if(_none or _generation < 4) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT effect FROM item_fling_effect_prose WHERE "
            "local_language_id=9 AND item_fling_effect_id="
            "(SELECT fling_effect_id FROM items WHERE id=?)";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, _item_id
               );
    }

}}
