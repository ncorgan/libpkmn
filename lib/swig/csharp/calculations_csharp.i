/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <csharp/csharp_init.i>
PKMN_CSHARP_INIT

%include <stdint.i>
%include <std_string.i>

%import <stl_csharp.i>

%{
    #include <pkmn/calculations/form.hpp>
    #include <pkmn/calculations/gender.hpp>
    #include <pkmn/calculations/hidden_power.hpp>
    #include <pkmn/calculations/shininess.hpp>
    #include <pkmn/calculations/stats.hpp>
%}

// Convert Doxygen docs to C# docstrings
%include <pkmn_csharp_docs.i>

%include <pkmn/calculations/form.hpp>
%include <pkmn/calculations/gender.hpp>
%include <pkmn/calculations/hidden_power.hpp>
%include <pkmn/calculations/shininess.hpp>
%include <pkmn/calculations/stats.hpp>
