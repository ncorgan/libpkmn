/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../misc_common.hpp"
#include "database_common.hpp"
#include "id_to_string.hpp"

#include <pkmn/database/move_entry.hpp>

#include <boost/config.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <sstream>
#include <stdexcept>

namespace pkmn { namespace database {

    static pkmn::database::sptr _db;

    /*
     * This array lists the upper bound of valid move IDs
     * for each generation. This is trivial enough not to
     * warrant a database query.
     */
    BOOST_STATIC_CONSTEXPR int MOVE_INDEX_BOUNDS[] = {
        -1, 165, 251, 354, 467, 559, 621
    };

    // Some Generation VI moves are only in OR/AS
    BOOST_STATIC_CONSTEXPR int XY_MAX = 617;

    // Gamecube games have an extra range.
    BOOST_STATIC_CONSTEXPR int SHADOW_RUSH = 10001;
    BOOST_STATIC_CONSTEXPR int SHADOW_SKY  = 10018;

    // Relevant version IDs
    BOOST_STATIC_CONSTEXPR int COLO = 19;
    BOOST_STATIC_CONSTEXPR int XD   = 20;
    BOOST_STATIC_CONSTEXPR int Y    = 24;

    move_entry::move_entry(
        int move_id,
        int game_id
    ):
        _move_id(move_id),
        _game_id(game_id),
        _none(move_id == 0)
    {
        // Connect to database
        pkmn::database::get_connection(_db);

        _generation = pkmn::database::game_id_to_generation(
                          _game_id
                      );

        /*
         * Check to see if the move is valid for the given game.
         *
         * With this constructor, it's fine if not.
         */
        if(_move_id > MOVE_INDEX_BOUNDS[_generation]) {
            if(_game_id == COLO) {
                _invalid = (_move_id != SHADOW_RUSH);
            } else if(_game_id == XD) {
                _invalid = (_move_id < SHADOW_RUSH) or
                           (_move_id > SHADOW_SKY);
            } else {
                _invalid = true;
            }
        } else if(_generation == 6 and _game_id <= Y) {
            _invalid = (_move_id > XY_MAX);
        } else {
            _invalid = false;
        }
    }

    move_entry::move_entry(
        const std::string &move_name,
        const std::string &game_name
    ):
        _none(move_name == "None")
    {
        // Connect to database
        pkmn::database::get_connection(_db);

        // Input validation
        _game_id = pkmn::database::game_name_to_id(
                       game_name
                   );
        _generation = pkmn::database::game_id_to_generation(
                          _game_id
                      );
        _move_id = pkmn::database::move_name_to_id(
                       move_name
                   );

        // Check to see if the move is valid for the given game.
        if(_move_id > MOVE_INDEX_BOUNDS[_generation]) {
            if(_game_id == COLO) {
                _invalid = (_move_id != SHADOW_RUSH);
            } else if(_game_id == XD) {
                _invalid = (_move_id < SHADOW_RUSH) or
                           (_move_id > SHADOW_SKY);
            } else {
                _invalid = true;
            }
        } else if(_generation == 6 and _game_id <= Y) {
            _invalid = (_move_id > XY_MAX);
        } else {
            _invalid = false;
        }
        if(_invalid) {
            throw std::invalid_argument("This move was not in this game.");
        }
    }

    std::string move_entry::get_name() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return str(boost::format("Invalid (0x%x)") % _move_id);
        }

        return pkmn::database::move_id_to_name(
                   _move_id, _generation
               );
    }

    std::string move_entry::get_game() const {
        return pkmn::database::game_id_to_name(
                   _game_id
               );
    }

    std::string move_entry::get_type() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return "Invalid";
        }

        /*
         * In Generation I, before the Dark type was introduced,
         * four moves were Normal type.
         *
         * There aren't enough edge cases to warrant adding them
         * to the database.
         */
        if(_generation == 1) {
            BOOST_STATIC_CONSTEXPR int NORMAL_IDS[] = {2,16,28,44};
            for(int i = 0; i < 4; ++i) {
                if(_move_id == NORMAL_IDS[i]) {
                    return "Normal";
                }
            }
        }

        /*
         * In Generation VI, before the Fairy type was introduced,
         * three moves were Normal type.
         *
         * There aren't enough edge cases to warrant adding them
         * to the database.
         */
        if(_generation < 6) {
            BOOST_STATIC_CONSTEXPR int NORMAL_IDS[] = {186,204,236};
            for(int i = 0; i < 3; ++i) {
                if(_move_id == NORMAL_IDS[i]) {
                    return "Normal";
                }
            }
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM type_names WHERE local_language_id=9 "
            "AND type_id=(SELECT type_id FROM moves WHERE id=?)";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, _move_id
               );
    }

    std::string move_entry::get_description() const {
        if(_none or _invalid) {
            return get_name();
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT flavor_text FROM move_flavor_text WHERE move_id=? "
            "AND language_id=9";

        std::string from_db = pkmn::database::query_db_bind1<std::string, int>(
                                  _db, query, _move_id
                              );
        return fix_veekun_whitespace(from_db);
    }

    std::string move_entry::get_target() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM move_target_prose WHERE local_language_id=9 "
            "AND move_target_id=(SELECT id FROM move_targets WHERE id="
            "(SELECT target_id FROM moves WHERE id=?))";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, _move_id
               );
    }

    std::string move_entry::get_damage_class() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        /*
         * In Generations I-III (minus the Gamecube games), a move's damage
         * class was associated with its type instead of the move itself,
         * unless it's a status move.
         */
        static BOOST_CONSTEXPR const char* old_games_query = \
            "SELECT damage_class_id FROM types WHERE id="
            "(SELECT type_id FROM moves where id=?)";

        static BOOST_CONSTEXPR const char* main_query = \
            "SELECT damage_class_id FROM moves WHERE id=?";

        bool old_game = (_generation < 4 and not game_is_gamecube(_game_id));
        int damage_class_id = pkmn::database::query_db_bind1<int, int>(
                                  _db, main_query, _move_id
                              );

        static BOOST_CONSTEXPR const char* damage_classes[] = {
            "", "Status", "Physical", "Special"
        };

        if(old_game and damage_class_id > 1) {
            damage_class_id = pkmn::database::query_db_bind1<int, int>(
                                  _db, old_games_query, _move_id
                              );
        }

        return damage_classes[damage_class_id];
    }

    int move_entry::get_base_power() const {
        if(_none or _invalid) {
            return -1;
        }

        // Edge case not worth putting in a database
        if(_move_id == 10001 and _game_id == 19) {
            return 90;
        }

        static BOOST_CONSTEXPR const char* main_query = \
            "SELECT power FROM moves WHERE id=?";

        static BOOST_CONSTEXPR const char* old_queries[] = {
            "",
            "SELECT gen1_power FROM old_move_powers WHERE move_id=?",
            "SELECT gen2_power FROM old_move_powers WHERE move_id=?",
            "SELECT gen3_power FROM old_move_powers WHERE move_id=?",
            "SELECT gen4_power FROM old_move_powers WHERE move_id=?",
            "SELECT gen5_power FROM old_move_powers WHERE move_id=?",
        };

        /*
         * If this entry is for an older game, check if it had an older
         * power. If not, fall back to the default query.
         */
        if(_generation < 6) {
            int old_ret = 0;
            if(pkmn::database::maybe_query_db_bind1<int, int>(
                   _db, old_queries[_generation], old_ret,
                   _move_id
                ))
            {
                return old_ret;
            }
        }

        return pkmn::database::query_db_bind1<int, int>(
                   _db, main_query, _move_id
               );
    }

    int move_entry::get_pp(
        int num_pp_ups
    ) const {
        if(_none or _invalid) {
            return -1;
        } else if(num_pp_ups < 0 or num_pp_ups > 3) {
            throw std::out_of_range("num_pp_ups: valid range 0-3");
        }

        static BOOST_CONSTEXPR const char* main_query = \
            "SELECT pp FROM moves WHERE id=?";

        static BOOST_CONSTEXPR const char* old_queries[] = {
            "",
            "SELECT gen1_pp FROM old_move_pps WHERE move_id=?",
            "SELECT gen2_pp FROM old_move_pps WHERE move_id=?",
            "SELECT gen3_pp FROM old_move_pps WHERE move_id=?",
            "SELECT gen4_pp FROM old_move_pps WHERE move_id=?",
            "SELECT gen5_pp FROM old_move_pps WHERE move_id=?",
        };

        /*
         * If this entry is for an older game, check if it had an older
         * base PP. If not, fall back to the default query.
         */
        int base_pp = -1;
        if(_generation < 6) {
            (void)pkmn::database::maybe_query_db_bind1<int, int>(
                      _db, old_queries[_generation],
                      base_pp, _move_id
                  );
        }

        if(base_pp == -1) {
            base_pp = pkmn::database::query_db_bind1<int, int>(
                          _db, main_query, _move_id
                      );
        }

        if(num_pp_ups == 0) {
            return base_pp;
        } else {
            int _20p = int(base_pp * 0.2);
            return (base_pp + (num_pp_ups * _20p));
        }
    }

    float move_entry::get_accuracy() const {
        if(_none or _invalid) {
            return -1.0f;
        }

        static BOOST_CONSTEXPR const char* main_query = \
            "SELECT accuracy FROM moves WHERE id=?";

        static BOOST_CONSTEXPR const char* old_queries[] = {
            "",
            "SELECT gen1_accuracy FROM old_move_accuracies WHERE move_id=?",
            "SELECT gen2_accuracy FROM old_move_accuracies WHERE move_id=?",
            "SELECT gen3_accuracy FROM old_move_accuracies WHERE move_id=?",
            "SELECT gen4_accuracy FROM old_move_accuracies WHERE move_id=?",
            "SELECT gen5_accuracy FROM old_move_accuracies WHERE move_id=?",
        };

        /*
         * If this entry is for an older game, check if it had an older
         * accuracy.
         */
        if(_generation < 6) {
            double old_ret;
            if(pkmn::database::maybe_query_db_bind1<double, int>(
                   _db, old_queries[_generation], old_ret,
                   _move_id
               ))
            {
                // Veekun's database stores this as an int 0-100.
                return (float(old_ret) / 100.0f);
            }
        }

        // SQLite uses doubles, so avoid implicit casting ambiguity
        return ((float)pkmn::database::query_db_bind1<double, int>(
                          _db, main_query, _move_id
                       )) / 100.0f;
    }

    int move_entry::get_priority() const {
        if(_none or _invalid) {
            return -9;
        }

        static BOOST_CONSTEXPR const char* main_query = \
            "SELECT priority FROM moves WHERE id=?";

        static BOOST_CONSTEXPR const char* old_queries[] = {
            "",
            "SELECT gen1_priority FROM old_move_priorities WHERE move_id=?",
            "SELECT gen2_priority FROM old_move_priorities WHERE move_id=?",
            "SELECT gen3_priority FROM old_move_priorities WHERE move_id=?",
            "SELECT gen4_priority FROM old_move_priorities WHERE move_id=?",
            "SELECT gen5_priority FROM old_move_priorities WHERE move_id=?",
        };

        /*
         * If this entry is for an older game, check if it had an older
         * priority. If not, fall back to the default query.
         */
        if(_generation < 6) {
            int old_ret = 0;
            if(pkmn::database::maybe_query_db_bind1<int, int>(
                   _db, old_queries[_generation], old_ret,
                   _move_id
                ))
            {
                return old_ret;
            }
        }

        return pkmn::database::query_db_bind1<int, int>(
                   _db, main_query, _move_id
               );
    }

    static std::string _cleanup_effect(
        const std::string &input,
        int move_id
    ) {
        // Get effect chance
        static BOOST_CONSTEXPR const char* query = \
            "SELECT effect_chance FROM moves WHERE id=?";

        int effect_chance = pkmn::database::query_db_bind1<int, int>(
                                _db, query, move_id
                            );

        std::string ret = input;

        ret.erase(std::remove(ret.begin(), ret.end(), '['), ret.end());
        ret.erase(std::remove(ret.begin(), ret.end(), ']'), ret.end());

        size_t open = ret.find("{");
        if(open != std::string::npos) {
            size_t close = ret.find("}");
            ret.replace(open, (close-open+1), "");
        }

        if(effect_chance > 0) {
            std::stringstream stream;
            stream << effect_chance << "%";

            size_t effect_chance_pos = ret.find("$effect_chance%");
            if(effect_chance_pos != std::string::npos) {
                ret.replace(effect_chance_pos, 15, stream.str());
            }
        }

        return ret;
    }

    std::string move_entry::get_effect() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT short_effect FROM move_effect_prose WHERE move_effect_id="
            "(SELECT effect_id FROM moves WHERE id=?)";

        std::string from_db = pkmn::database::query_db_bind1<std::string, int>(
                                  _db, query, _move_id
                              );
        return _cleanup_effect(from_db, _move_id);
    }

    std::string move_entry::get_contest_type() const {
        // Contests started in Generation III
        if(_none or _generation < 3 or game_is_gamecube(_game_id)) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM contest_type_names WHERE contest_type_id="
            "(SELECT contest_type_id FROM moves WHERE id=?) "
            "AND local_language_id=9";

        std::string ret;
        if(pkmn::database::maybe_query_db_bind1<std::string, int>(
               _db, query, ret, _move_id
           )) {
            return ret;
        } else {
            return "None";
        }
    }

    std::string move_entry::get_contest_effect() const {
        // Contests started in Generation III
        if(_none or _generation < 3 or game_is_gamecube(_game_id)) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT flavor_text FROM contest_effect_prose WHERE contest_effect_id="
            "(SELECT contest_effect_id FROM moves WHERE id=?)";

        std::string ret;
        if(pkmn::database::maybe_query_db_bind1<std::string, int>(
               _db, query, ret, _move_id
           )) {
            return ret;
        } else {
            return "None";
        }
    }

    std::string move_entry::get_super_contest_effect() const {
        // Super Contests are only in Generation IV
        if(_none or _generation != 4) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT flavor_text FROM super_contest_effect_prose WHERE super_contest_effect_id="
            "(SELECT super_contest_effect_id FROM moves WHERE id=?)";

        std::string ret;
        if(pkmn::database::maybe_query_db_bind1<std::string, int>(
               _db, query, ret, _move_id
           )) {
            return ret;
        } else {
            return "None";
        }
    }

}}
