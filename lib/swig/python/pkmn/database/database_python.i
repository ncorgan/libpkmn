/*
 * Copyright (c) 2015-2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <python/python_init.i>
PKMN_PYTHON_INIT

%include <std_string.i>

%import <stl_python.i>

// Convert Doxygen docs to Python docstrings
%include <pkmn_python_docstrings.i>

%rename(stat) pkmn::e_stat;
%import <pkmn/enums/stat.hpp>

%include <python/pkmn_database_item_entry.i>
%include <python/pkmn_database_move_entry.i>
%include <python/pkmn_database_pokemon_entry.i>

%include <pkmn/database/levelup_move.hpp>
PKMN_PYTHON_VECTOR(pkmn::database::levelup_move, levelup_moves)
