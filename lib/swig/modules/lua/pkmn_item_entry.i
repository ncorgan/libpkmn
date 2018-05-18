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

// Convert getter/setter functions into attributes for more idiomatic Lua.

%attributestring(pkmn::database::item_entry, std::string, name, get_name);
%attributestring(pkmn::database::item_entry, std::string, game, get_game);
%attributestring(pkmn::database::item_entry, std::string, category, get_category);
%attributestring(pkmn::database::item_entry, std::string, pocket, get_pocket);
%attributestring(pkmn::database::item_entry, std::string, description, get_description);
%attribute(pkmn::database::item_entry, int, cost, get_cost);
%attribute(pkmn::database::item_entry, bool, is_holdable, holdable);
%attribute(pkmn::database::item_entry, int, fling_power, get_fling_power);
%attributestring(pkmn::database::item_entry, std::string, fling_effect, get_fling_effect);

%ignore pkmn::database::item_entry::get_item_id;
%ignore pkmn::database::item_entry::get_item_index;
%ignore pkmn::database::item_entry::get_item_list_id;
%ignore pkmn::database::item_entry::get_game_id;

%include <pkmn/database/item_entry.hpp>
