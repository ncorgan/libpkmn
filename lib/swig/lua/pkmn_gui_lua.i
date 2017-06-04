/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <lua/lua_init.i>
PKMN_LUA_INIT

%import <pkmn_stl_lua.i>

%{
    #include <pkmn/qt/Spinda.hpp>
%}

%rename(generate_spinda_sprite_at_filepath) GenerateSpindaSpriteAtFilepath;
%include <pkmn/qt/Spinda.hpp>
