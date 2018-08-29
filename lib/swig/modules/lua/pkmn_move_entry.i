/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/move_entry.hpp>
%}

%include <attribute.i>

// Convert getter/setter functions into attributes for more idiomatic Lua.

%attribute(pkmn::database::move_entry, pkmn::e_move, move, get_move);
%attributestring(pkmn::database::move_entry, std::string, name, get_name);
%attribute(pkmn::database::move_entry, pkmn::e_game, game, get_game);
%attribute(pkmn::database::move_entry, pkmn::e_type, type, get_type);
%attributestring(pkmn::database::move_entry, std::string, description, get_description);
%attribute(pkmn::database::move_entry, pkmn::e_move_target, target, get_target);
%attribute(pkmn::database::move_entry, pkmn::e_move_damage_class, damage_class, get_damage_class);
%attribute(pkmn::database::move_entry, int, base_power, get_base_power);
%attribute(pkmn::database::move_entry, float, accuracy, get_accuracy);
%attribute(pkmn::database::move_entry, int, priority, get_priority);
%attributestring(pkmn::database::move_entry, std::string, effect, get_effect);
%attribute(pkmn::database::move_entry, pkmn::e_contest_stat, contest_type, get_contest_type);
%attributestring(pkmn::database::move_entry, std::string, contest_effect, get_contest_effect);
%attributestring(pkmn::database::move_entry, std::string, super_contest_effect, get_super_contest_effect);

%ignore pkmn::database::move_entry::get_move_id;
%ignore pkmn::database::move_entry::get_game_id;

%include <pkmn/database/move_entry.hpp>

%include <lua/stl_macros.i>
PKMN_LUA_VECTOR(pkmn::database::move_entry, move_entry_list)
