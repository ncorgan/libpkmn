/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/item_list.hpp>
%}

%rename("as_list") as_vector;

%rename("name") pkmn::database::item_entry::get_name() const;
%rename("game") pkmn::database::item_entry::get_game() const;
%rename("category") pkmn::database::item_entry::get_category() const;
%rename("pocket") pkmn::database::item_entry::get_pocket() const;
%rename("description") pkmn::database::item_entry::get_description() const;
%rename("cost") pkmn::database::item_entry::get_cost() const;
%rename("is_holdable?") pkmn::database::item_entry::holdable() const;
%rename("fling_power") pkmn::database::item_entry::get_fling_power() const;
%rename("fling_effect") pkmn::database::item_entry::get_fling_effect() const;

%ignore pkmn::database::item_entry::get_item_id;
%ignore pkmn::database::item_entry::get_item_index;
%ignore pkmn::database::item_entry::get_item_list_id;
%ignore pkmn::database::item_entry::get_game_id;
%include <pkmn/database/item_entry.hpp>
