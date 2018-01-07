/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/move_entry.hpp>
%}

%include <attribute.i>

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::database::move_entry, std::string, Name, get_name);
%attributestring(pkmn::database::move_entry, std::string, Game, get_game);
%attributestring(pkmn::database::move_entry, std::string, MoveType, get_type);
%attributestring(pkmn::database::move_entry, std::string, Description, get_description);
%attributestring(pkmn::database::move_entry, std::string, Target, get_target);
%attributestring(pkmn::database::move_entry, std::string, DamageClass, get_damage_class);
%attribute(pkmn::database::move_entry, int, BasePower, get_base_power);
%attribute(pkmn::database::move_entry, float, Accuracy, get_accuracy);
%attribute(pkmn::database::move_entry, int, Priority, get_priority);
%attributestring(pkmn::database::move_entry, std::string, Effect, get_effect);
%attributestring(pkmn::database::move_entry, std::string, ContestType, get_contest_type);
%attributestring(pkmn::database::move_entry, std::string, ContestEffect, get_contest_effect);
%attributestring(pkmn::database::move_entry, std::string, SuperContestEffect, get_super_contest_effect);

%ignore pkmn::database::move_entry::get_move_id;
%ignore pkmn::database::move_entry::get_game_id;
%include <pkmn/database/move_entry.hpp>

%include <csharp/stl_macros.i>
PKMN_CSHARP_VECTOR(pkmn::database::move_entry, MoveEntry, MoveEntryList)
