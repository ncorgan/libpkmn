/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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

%include <pkmn/database/item_entry.hpp>
%include <pkmn/database/lists.hpp>

%include <pkmn/database/move_entry.hpp>
PKMN_LUA_VECTOR(pkmn::database::move_entry, move_entry_list)

%include <pkmn/database/levelup_move.hpp>
PKMN_LUA_VECTOR(pkmn::database::levelup_move, levelup_moves)

%include <pkmn/database/pokemon_entry.hpp>
PKMN_LUA_VECTOR(pkmn::database::pokemon_entry, pokemon_entry_list)
