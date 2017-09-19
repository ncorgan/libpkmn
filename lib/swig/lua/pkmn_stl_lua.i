/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <lua/lua_init.i>
PKMN_LUA_INIT

/*
 * This file contains all pure STL wrappers. Other .i files can declare maps,
 * pairs, and vectors of their classes, but there's no reason for multiple
 * files to declare string vectors, etc.
 */

%include <std_string.i>

// std::map
PKMN_LUA_MAP(std::string, bool,        string_bool_dict)
PKMN_LUA_MAP(std::string, std::string, string_string_dict)
PKMN_LUA_MAP(std::string, int,         string_int_dict)

// std::pair
PKMN_LUA_PAIR(std::string, std::string, string_pair)

// std::vector
PKMN_LUA_VECTOR(int, int_list)
PKMN_LUA_VECTOR(std::string, string_list)
