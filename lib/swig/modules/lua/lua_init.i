/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%define PKMN_LUA_INIT
    %include <swig_exceptions.i>
    SWIG_CATCH_DEFAULT

    %include <pkmn.i>
    %include <lua/unsigned_nums_typemap.i> 
    %include <lua/stl_macros.i>
%enddef
