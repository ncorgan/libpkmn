/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "id_to_string.hpp"

#include <pkmn/database/move_entry.hpp>

#include <boost/config.hpp>

#include <sstream>

namespace pkmn { namespace database {

    // TODO: database sptr

    move_entry::move_entry():
        _move_id(0),
        _game_id(0),
        _generation(0),
        _none(false),
        _invalid(true) {}

    move_entry::move_entry(
        int move_id,
        int game_id
    ):
        _move_id(move_id),
        _game_id(game_id),
        _generation(0),
        _none(false),
        _invalid(true) {}

    move_entry::move_entry(
        const std::string &move_name,
        const std::string &game_name
    ):
        _generation(0),
        _none(false),
        _invalid(true)
    {
        // Make sure move and game are valid
        _move_id = pkmn::database::move_name_to_id(
                       move_name
                   );
        _game_id = pkmn::database::game_name_to_id(
                       game_name
                   );
    }

    std::string move_entry::get_name() const {
        return "";
    }

    std::string move_entry::get_game() const {
        return pkmn::database::game_id_to_name(
                   _game_id
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
        (void)num_pp_ups;
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
        return 0.0f;
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
