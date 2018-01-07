/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/item_entry.hpp>
%}

%include <attribute.i>

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::database::item_entry, std::string, Name, get_name);
%attributestring(pkmn::database::item_entry, std::string, Game, get_game);
%attributestring(pkmn::database::item_entry, std::string, Category, get_category);
%attributestring(pkmn::database::item_entry, std::string, Pocket, get_pocket);
%attributestring(pkmn::database::item_entry, std::string, Description, get_description);
%attribute(pkmn::database::item_entry, int, Cost, get_cost);
%attribute(pkmn::database::item_entry, bool, IsHoldable, holdable);
%attribute(pkmn::database::item_entry, int, FlingPower, get_fling_power);
%attributestring(pkmn::database::item_entry, std::string, FlingEffect, get_fling_effect);

%ignore pkmn::database::item_entry::get_item_id;
%ignore pkmn::database::item_entry::get_item_index;
%ignore pkmn::database::item_entry::get_item_list_id;
%ignore pkmn::database::item_entry::get_game_id;
%include <pkmn/database/item_entry.hpp>
