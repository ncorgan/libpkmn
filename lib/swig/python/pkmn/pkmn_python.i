/*
 * Copyright (c) 2015-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <python/python_init.i>
PKMN_PYTHON_INIT

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

/*
 * Remove access to bases of sptr'd classes, but don't give access to our
 * thin make wrapper.
 */
%rename(__make_game_save)     make_game_save;
%rename(__make_item_list)     make_item_list;
%rename(__make_item_bag)      make_item_bag;
%rename(__make_pokedex)       make_pokedex;
%rename(__make_pokemon)       make_pokemon;
%rename(__make_pokemon_box)   make_pokemon_box;
%rename(__make_pokemon_party) make_pokemon_party;
%rename(__make_pokemon_pc)    make_pokemon_pc;
%include <pkmn_statics.i>
%pythoncode %{
    game_save     = __make_game_save;
    item_list     = __make_item_list;
    item_bag      = __make_item_bag;
    pokedex       = __make_pokedex;
    pokemon       = __make_pokemon;
    pokemon_box   = __make_pokemon_box;
    pokemon_party = __make_pokemon_party;
    pokemon_pc    = __make_pokemon_pc;
%}
