/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <python/python_init.i>
PKMN_PYTHON_INIT

%include <std_string.i>

%import <stl_python.i>

%{
    #include <pkmn/database/lists.hpp>
%}

// Convert Doxygen docs to Python docstrings
%include <pkmn_python_docstrings.i>

%include <python/pkmn_database_item_entry.i>
%include <python/pkmn_database_levelup_move.i>
%include <python/pkmn_database_move_entry.i>
%include <python/pkmn_database_pokemon_entry.i>

%include <pkmn/database/lists.hpp>
