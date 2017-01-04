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
    #include <pkmn/move_slot.hpp>
%}

%ignore from_file;
%ignore make;
%ignore get_native;
%ignore get_native_pc_data;
%ignore get_native_party_data;

// Item Slot
%include <pkmn/item_slot.hpp>
PKMN_LUA_VECTOR(pkmn::item_slot, item_slot_list)

// Item List
%include <lua/pkmn_item_list.i>

// Item Bag
%include <lua/pkmn_item_bag.i>

// Move Slot
%include <pkmn/move_slot.hpp>
PKMN_LUA_VECTOR(pkmn::move_slot, move_slot_list)

// Pokémon
%include <lua/pkmn_pokemon.i>

// Pokémon Box
%include <lua/pkmn_pokemon_box.i>

// Pokémon PC
%include <lua/pkmn_pokemon_pc.i>

%include <pkmn_statics.i>
