/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <lua/lua_init.i>
PKMN_LUA_INIT

// Will be called on import

%{
    #include "private_exports.hpp"
%}

namespace pkmn { namespace priv {
    void initialize_database_connection();
}}

%import <pkmn_database_lua.i>
%import <pkmn_stl_lua.i>

// Attribute Maps
%include <lua/pkmn_attribute_maps.i>

// Item Slot
%include <lua/pkmn_item_slot.i>

// Item List
%include <lua/pkmn_item_list.i>

// Item Bag
%include <lua/pkmn_item_bag.i>

// Pokédex
%include <lua/pkmn_pokedex_helpers.i>
%include <lua/pkmn_pokedex.i>

// Pokémon
%include <lua/pkmn_pokemon_helpers.i>
%include <lua/pkmn_pokemon.i>

// Pokémon Box
%include <lua/pkmn_pokemon_box.i>

// Pokémon Party
%include <lua/pkmn_pokemon_party.i>

// Pokémon PC
%include <lua/pkmn_pokemon_pc.i>

// Game Save
%include <lua/pkmn_game_save.i>

%include <pkmn_statics.i>
