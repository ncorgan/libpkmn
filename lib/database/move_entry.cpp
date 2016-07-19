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

#include <stdexcept>

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
        return "";
    }

    std::string move_entry::get_damage_class() const {
        return "";
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

    std::string move_entry::get_effect() const {
        return "";
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
        return "";
    }

    std::string move_entry::get_contest_effect() const {
        return "";
    }

    std::string move_entry::get_super_contest_effect() const {
        return "";
    }

}}
