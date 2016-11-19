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
    #include <pkmn/database/item_entry.hpp>
    #include <pkmn/database/move_entry.hpp>
    #include <pkmn/database/pokemon_entry.hpp>

    #include <pkmn/database/lists.hpp>
%}

// Convert Doxygen docs to Python docstrings
%include <pkmn_python_docstrings.i>

%include <python/pkmn_levelup_move.i>

%include <pkmn/database/item_entry.hpp>
%include <pkmn/database/move_entry.hpp>
%include <pkmn/database/pokemon_entry.hpp>

%include <pkmn/database/lists.hpp>

PKMN_PYTHON_VECTOR(pkmn::database::move_entry, move_list)
PKMN_PYTHON_VECTOR(pkmn::database::pokemon_entry, pokemon_entries)
