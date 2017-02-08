/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <python/python_init.i>
PKMN_PYTHON_INIT

%include <stdint.i>
%include <std_string.i>

%{
    #include <pkmn/calculations/form.hpp>
    #include <pkmn/calculations/gender.hpp>
    #include <pkmn/calculations/hidden_power.hpp>
    #include <pkmn/calculations/nature.hpp>
    #include <pkmn/calculations/shininess.hpp>
    #include <pkmn/calculations/size.hpp>
    #include <pkmn/calculations/spinda_spots.hpp>
    #include <pkmn/calculations/stats.hpp>
%}

// Convert Doxygen docs to Python docstrings
%include <pkmn_python_docstrings.i>

%include <pkmn/calculations/form.hpp>
%include <pkmn/calculations/gender.hpp>
%include <pkmn/calculations/hidden_power.hpp>
%include <pkmn/calculations/nature.hpp>
%include <pkmn/calculations/shininess.hpp>
%include <pkmn/calculations/size.hpp>
%include <pkmn/calculations/spinda_spots.hpp>
%include <pkmn/calculations/stats.hpp>
