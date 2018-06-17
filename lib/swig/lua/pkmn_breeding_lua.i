/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <lua/lua_init.i>
PKMN_LUA_INIT

%import <pkmn_core_lua.i>
%import <pkmn_stl_lua.i>

%{
    #include <pkmn/breeding/child_info.hpp>
    #include <pkmn/breeding/compatibility.hpp>

    #include "cpp_wrappers/breeding.hpp"
%}

%rename(gender) pkmn::e_gender;
%import <pkmn/enums/gender.hpp>

%include <pkmn/breeding/child_info.hpp>
%include <pkmn/breeding/compatibility.hpp>
%include <cpp_wrappers/breeding.hpp>
