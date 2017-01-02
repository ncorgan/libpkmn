/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <lua/lua_init.i>
PKMN_LUA_INIT

%include <std_string.i>

%{
    #include <pkmn/build_info.hpp>

    PKMN_INLINE std::string get_swig_version() {
        return std::string("@SWIG_VERSION@");
    }
%}

%include <pkmn/build_info.hpp>
std::string get_swig_version();
