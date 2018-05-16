/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <python/python_init.i>
PKMN_PYTHON_INIT

%import <std_string.i>

%{
    #include <pkmn/qt/Spinda.hpp>
%}

%include <pkmn/qt/Spinda.hpp>
