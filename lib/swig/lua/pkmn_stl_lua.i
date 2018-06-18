/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <lua/lua_init.i>
PKMN_LUA_INIT

%{
    #include <pkmn/enums/stat.hpp>
%}

/*
 * This file contains STL wrappers with pure STL types or LibPKMN enums.
 */

%rename(stat) pkmn::e_stat;
%include <pkmn/enums/stat.hpp>

%include <std_string.i>

// std::map
PKMN_LUA_MAP(pkmn::e_stat, int, stat_dict);

// std::pair
PKMN_LUA_PAIR(std::string, std::string, string_pair)

// std::vector
PKMN_LUA_VECTOR(int,          int_list)
PKMN_LUA_VECTOR(std::string,  string_list)
PKMN_LUA_VECTOR(pkmn::e_stat, stat_list);
