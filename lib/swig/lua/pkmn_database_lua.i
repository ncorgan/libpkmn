/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <lua/lua_init.i>
PKMN_LUA_INIT

%import <pkmn_stl_lua.i>

%{
    #include <pkmn/database/item_entry.hpp>
    #include <pkmn/database/lists.hpp>
    #include <pkmn/database/levelup_move.hpp>
    #include <pkmn/database/move_entry.hpp>
    #include <pkmn/database/pokemon_entry.hpp>
%}

%include <lua/pkmn_item_entry.i>
%include <lua/pkmn_move_entry.i>
%include <lua/pkmn_pokemon_entry.i>

%include <pkmn/database/lists.hpp>

%include <pkmn/database/levelup_move.hpp>
PKMN_LUA_VECTOR(pkmn::database::levelup_move, levelup_moves)

%include <pkmn/database/pokemon_entry.hpp>
