/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%module(directors="0", package="pkmn.qt") qt_python_swig

%include <python/python_init.i>
PKMN_PYTHON_INIT

%import <std_string.i>

%{
    #include <pkmn/qt/Spinda.hpp>
%}

%include <pkmn/qt/Spinda.hpp>
