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
    #include <pkmn/utils/paths.hpp>

    #include "env.hpp"
%}

%include <pkmn/utils/paths.hpp>

%rename(getenv) pkmn_getenv;
%rename(setenv) pkmn_setenv;
%rename(unsetenv) pkmn_unsetenv;

%include "env.hpp"
