/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <lua/lua_init.i>
PKMN_LUA_INIT

%import <pkmn_database_lua.i>
%import <pkmn_stl_lua.i>

%{
    #include <pkmn/item_slot.hpp>
    #include <pkmn/item_list.hpp>
%}

%ignore make;
%ignore get_native;

%include <pkmn/item_slot.hpp>

// Item List
%include <lua/pkmn_item_list.i>

// Item Bag
%include <lua/pkmn_item_bag.i>

%include <pkmn_statics.i>
