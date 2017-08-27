/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include <pkmn/database/move_entry.hpp>
%}

%rename("name") pkmn::database::move_entry::get_name() const;
%rename("game") pkmn::database::move_entry::get_game() const;
%rename("type") pkmn::database::move_entry::get_type() const;
%rename("description") pkmn::database::move_entry::get_description() const;
%rename("target") pkmn::database::move_entry::get_target() const;
%rename("damage_class") pkmn::database::move_entry::get_damage_class() const;
%rename("base_power") pkmn::database::move_entry::get_base_power() const;
%rename("accuracy") pkmn::database::move_entry::get_accuracy() const;
%rename("priority") pkmn::database::move_entry::get_priority() const;
%rename("effect") pkmn::database::move_entry::get_effect() const;
%rename("contest_type") pkmn::database::move_entry::get_contest_type() const;
%rename("contest_effect") pkmn::database::move_entry::get_contest_effect() const;
%rename("super_contest_effect") pkmn::database::move_entry::get_super_contest_effect() const;

%ignore pkmn::database::move_entry::get_move_id;
%ignore pkmn::database::move_entry::get_game_id;
%include <pkmn/database/move_entry.hpp>

PKMN_RUBY_VECTOR(pkmn::database::move_entry, MoveEntryList);
