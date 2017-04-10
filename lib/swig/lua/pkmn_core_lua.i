/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <lua/lua_init.i>
PKMN_LUA_INIT

%import <pkmn_database_lua.i>
%import <pkmn_stl_lua.i>

%{
    #include <pkmn/types/datetime.hpp>

    #include <pkmn/game_save.hpp>
    #include <pkmn/item_slot.hpp>
    #include <pkmn/item_list.hpp>
    #include <pkmn/move_slot.hpp>
%}

%ignore from_file;
%ignore make;
%ignore get_native;
%ignore get_native_pc_data;
%ignore get_native_party_data;

// Datetime
%include <pkmn/types/datetime.hpp>

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

// Pokémon Party
%include <lua/pkmn_pokemon_party.i>

// Pokémon PC
%include <lua/pkmn_pokemon_pc.i>

// Game Save
%include <pkmn/game_save.hpp>
PKMN_LUA_SPTR(game_save)

%include <pkmn_statics.i>
