/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/item_entry.hpp>
%}

%csmethodmodifiers pkmn::database::item_entry::get_name() const "private";
%csmethodmodifiers pkmn::database::item_entry::get_game() const "private";
%csmethodmodifiers pkmn::database::item_entry::get_category() const "private";
%csmethodmodifiers pkmn::database::item_entry::get_pocket() const "private";
%csmethodmodifiers pkmn::database::item_entry::get_description() const "private";
%csmethodmodifiers pkmn::database::item_entry::get_cost() const "private";
%csmethodmodifiers pkmn::database::item_entry::holdable() const "private";
%csmethodmodifiers pkmn::database::item_entry::get_fling_power() const "private";
%csmethodmodifiers pkmn::database::item_entry::get_fling_effect() const "private";

%ignore pkmn::database::item_entry::get_item_id;
%ignore pkmn::database::item_entry::get_item_index;
%ignore pkmn::database::item_entry::get_item_list_id;
%ignore pkmn::database::item_entry::get_game_id;
%include <pkmn/database/item_entry.hpp>
