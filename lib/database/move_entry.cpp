/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "id_to_string.hpp"

#include <pkmn/database/move_entry.hpp>

#include <boost/config.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <stdexcept>

// TODO: confirm this is still accurate
#define GAMECUBE (_game_id == 19 or _game_id == 20)

namespace pkmn { namespace database {

    // TODO: database sptr

    move_entry::move_entry():
        _move_id(0),
        _game_id(0),
        _language_id(0),
        _generation(0),
        _none(false),
        _invalid(true) {}

    move_entry::move_entry(
        int move_id,
        int game_id,
        int language_id
    ):
        _move_id(move_id),
        _game_id(game_id),
        _language_id(-1),
        _generation(0),
        _none(false),
        _invalid(true)
    {
        // Confirm validity of language ID
        typedef std::pair<std::string, int> si_pair_t;
        BOOST_FOREACH(const si_pair_t &si_pair, libpkmn_languages) {
            if(si_pair.second == language_id) {
                _language_id = language_id;
            }
        }
        if(language_id == -1) {
            throw std::runtime_error("Invalid language.");
        }
    }

    move_entry::move_entry(
        const std::string &move_name,
        const std::string &game_name,
        const std::string &language
    ):
        _generation(0),
        _none(false),
        _invalid(false)
    {
        // Make sure move and game are valid
        _move_id = pkmn::database::move_name_to_id(
                       move_name
                   );
        _game_id = pkmn::database::game_name_to_id(
                       game_name
                   );

        // Confirm validity of language ID
        try {
            _language_id = libpkmn_languages.at(language);
        } catch(const std::exception&) {
            throw std::runtime_error("Invalid language.");
        }
    }

    std::string move_entry::get_name() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return str(boost::format("Invalid (0x%x)") % _move_id);
        }

        // TODO
        return "";
    }

    std::string move_entry::get_game() const {
        return pkmn::database::game_id_to_name(
                   _game_id,
                   _language_id
               );
    }

    std::string move_entry::get_type() const {
        return "";
    }

    std::string move_entry::get_description() const {
        if(_none or _invalid) {
            return get_name();
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT flavor_text FROM move_flavor_text WHERE move_id=? "
            "AND language_id=?";

        return pkmn_db_query_bind2<std::string, int, int>(
                   query, _move_id, _language_id
               );
    }

    std::string move_entry::get_damage_class() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* old_games_query = \
            "SELECT description FROM move_damage_class_prose "
            "WHERE local_language_id=9 AND move_damage_class_id="
            "(SELECT damage_class_id FROM types WHERE id="
            "(SELECT type_id FROM moves where id=?))";

        static BOOST_CONSTEXPR const char* new_games_query = \
            "SELECT description FROM move_damage_class_prose "
            "WHERE local_language_id=9 AND move_damage_class_id="
            "(SELECT damage_class_id FROM moves WHERE id=?)";

        bool old_game = (_generation < 4 and not GAMECUBE);
        return pkmn_db_query_bind1<std::string, int>(
                   (old_game ? old_games_query : new_games_query),
                   _move_id
               );
    }

    int move_entry::get_base_power() const {
        return 0;
    }

    int move_entry::get_pp(
        int num_pp_ups
    ) const {
        if(num_pp_ups < 0 or num_pp_ups > 3) {
            throw std::out_of_range("num_pp_ups: valid range 0-3");
        }

        return 0;
    }

    float move_entry::get_accuracy() const {
        return 0.0f;
    }

    int move_entry::get_priority() const {
        return 0;
    }

    static std::string _cleanup_effect(
        const std::string &input,
        float effect_chance
    ) {
        std::string ret = input;

        ret.erase(std::remove(ret.begin(), ret.end(), '['), ret.end());
        ret.erase(std::remove(ret.begin(), ret.end(), ']'), ret.end());

        size_t open = ret.find("{");
        if(open != std::string::npos) {
            size_t close = ret.find("}");
            ret.replace(open, (close-open+1), "");
        }

        int effect_chance_num = int(effect_chance * 100.0);
        if(effect_chance_num > 0) {
            std::stringstream stream;
            stream << effect_chance_num << "%";

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

        std::string from_db = pkmn_db_query_bind1<std::string, int>(
                                  query, _move_id
                              );
        return _cleanup_effect(from_db, this->get_effect_chance());
    }

    float move_entry::get_effect_chance() const {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT effect_chance FROM moves WHERE id=?";

        float effect_chance_from_db = pkmn_db_query_bind1<float, int>(
                                          query, _move_id
                                      );

        // Veekun's database stores this as an int 0-100.
        return (effect_chance_from_db / 100.0f);
    }

    std::string move_entry::get_contest_type() const {
        // Contests started in Generation III
        if(_none or _generation < 3 or GAMECUBE) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM contest_type_names WHERE contest_type_id="
            "(SELECT contest_type_id FROM moves WHERE id=?) "
            "AND local_language_id=?";

        return pkmn_db_query_bind2<std::string, int, int>(
                   query, _move_id, _language_id
               );
    }

    std::string move_entry::get_contest_effect() const {
        // Contests started in Generation III
        if(_none or _generation < 3 or GAMECUBE) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT flavor_text FROM contest_effect_prose WHERE contest_effect_id="
            "(SELECT contest_effect_id FROM moves WHERE id=?)";

        return pkmn_db_query_bind1<std::string, int>(
                   query, _move_id
               );
    }

    std::string move_entry::get_super_contest_effect() const {
        // Super Contests started in Generation III
        if(_none or _generation < 4) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT flavor_text FROM super_contest_effect_prose WHERE super_contest_effect_id="
            "(SELECT super_contest_effect_id FROM moves WHERE id=?)";

        return pkmn_db_query_bind1<std::string, int>(
                   query, _move_id
               );
    }

}}
