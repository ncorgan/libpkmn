/*
 * Copyright (c) 2015-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <csharp/csharp_init.i>
PKMN_CSHARP_INIT

%include <std_string.i>

%import <stl_csharp.i>

%{
    #include <pkmn/database/pokemon_entry.hpp>
%}

// Convert Doxygen docs to C# docstrings
%include <pkmn_csharp_docs.i>

%import <pkmn/enums/stat.hpp>

%include <csharp/pkmn_item_entry.i>
%include <csharp/pkmn_move_entry.i>
%include <csharp/pkmn_pokemon_entry.i>

%include <csharp/pkmn_levelup_move.i>
