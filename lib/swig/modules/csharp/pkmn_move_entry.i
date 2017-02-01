/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/move_entry.hpp>
%}

%csmethodmodifiers pkmn::database::move_entry::get_name() const "private";
%csmethodmodifiers pkmn::database::move_entry::get_game() const "private";
%csmethodmodifiers pkmn::database::move_entry::get_type() const "private";
%csmethodmodifiers pkmn::database::move_entry::get_description() const "private";
%csmethodmodifiers pkmn::database::move_entry::get_target() const "private";
%csmethodmodifiers pkmn::database::move_entry::get_damage_class() const "private";
%csmethodmodifiers pkmn::database::move_entry::get_base_power() const "private";
%csmethodmodifiers pkmn::database::move_entry::get_accuracy() const "private";
%csmethodmodifiers pkmn::database::move_entry::get_priority() const "private";
%csmethodmodifiers pkmn::database::move_entry::get_effect() const "private";
%csmethodmodifiers pkmn::database::move_entry::get_contest_type() const "private";
%csmethodmodifiers pkmn::database::move_entry::get_contest_effect() const "private";
%csmethodmodifiers pkmn::database::move_entry::get_super_contest_effect() const "private";

%ignore pkmn::database::move_entry::get_move_id;
%ignore pkmn::database::move_entry::get_game_id;
%include <pkmn/database/move_entry.hpp>

%include <csharp/stl_macros.i>
PKMN_CSHARP_VECTOR(pkmn::database::move_entry, MoveEntry, MoveEntryList)
