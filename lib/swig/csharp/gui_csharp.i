/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%module(directors="0") GUI

%include <csharp/csharp_init.i>
PKMN_CSHARP_INIT

%include <std_string.i>

%import <stl_csharp.i>

%{
    #include <pkmn/qt/Spinda.hpp>
%}

// Convert Doxygen docs to C# docstrings
%include <pkmn_csharp_docs.i>

%include <pkmn/qt/Spinda.hpp>
