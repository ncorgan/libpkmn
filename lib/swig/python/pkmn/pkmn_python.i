/*
 * Copyright (c) 2015-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <python/python_init.i>
PKMN_PYTHON_INIT

// Will be called on import

%{
    #include "private_exports.hpp"
%}


namespace pkmn { namespace priv {
    void initialize_database_connection();
}}

%include <std_string.i>

%import <database/database_python.i>
%import <stl_python.i>

%{
    #include <pkmn/build_info.hpp>

    #include <pkmn/game_save.hpp>
    #include <pkmn/item_list.hpp>
    #include <pkmn/item_bag.hpp>

    #include <pkmn/utils/paths.hpp>
%}

%warnfilter(401);

// Convert Doxygen docs to Python docstrings
%include <pkmn_python_docstrings.i>

%include <pkmn/build_info.hpp>
%include <pkmn/utils/paths.hpp>

// Item Slot
%include <python/pkmn_item_slot.i>

// Move Slot
%include <python/pkmn_move_slot.i>

// For all sptrs, for some reason didn't get ignored from pkmn.i
%ignore make;
%ignore get_native;

// Game Save
%ignore detect_type;
%include <pkmn/game_save.hpp>
PKMN_PYTHON_SPTR(game_save)

// Item List
%include <python/pkmn_item_list.i>

// Item Bag
%include <python/pkmn_item_bag.i>

// Pokédex
%include <python/pkmn_pokedex.i>

// Pokémon
%include <python/pkmn_pokemon.i>

// Pokémon Box
%include <python/pkmn_pokemon_box.i>

// Pokémon Party
%include <python/pkmn_pokemon_party.i>

// Pokémon PC
%include <python/pkmn_pokemon_pc.i>
