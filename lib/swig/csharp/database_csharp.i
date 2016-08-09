/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <csharp/csharp_init.i>
PKMN_CSHARP_INIT

%include <std_string.i>

%import <stl_csharp.i>

%{
    #include <pkmn/database/item_entry.hpp>
    #include <pkmn/database/lists.hpp>
    #include <pkmn/database/move_entry.hpp>
    #include <pkmn/database/pokemon_entry.hpp>
%}

// Convert Doxygen docs to C# docstrings
%include <pkmn_csharp_docs.i>

%include <csharp/pkmn_levelup_move.i>

%include <pkmn/database/item_entry.hpp>
%include <pkmn/database/lists.hpp>
%include <pkmn/database/move_entry.hpp>
%include <pkmn/database/pokemon_entry.hpp>

// Entry lists
PKMN_CSHARP_VECTOR(pkmn::database::move_entry, MoveEntry, MoveEntryList)
PKMN_CSHARP_VECTOR(pkmn::database::pokemon_entry, PokemonEntry, PokemonEntryList)
