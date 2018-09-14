/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%module(directors="0", package="pkmn.paths") paths_python

%include <python/python_init.i>
PKMN_PYTHON_INIT

%include <std_string.i>

%{
    #include <pkmn/utils/paths.hpp>
%}

%include <pkmn/utils/paths.hpp>
