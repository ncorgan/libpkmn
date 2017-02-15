/*
 * Copyright (c) 2015-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <java/java_init.i>
PKMN_JAVA_INIT

%import <stl_java.i>

%{
    #include <pkmn/database/item_entry.hpp>
    #include <pkmn/database/move_entry.hpp>
    #include <pkmn/database/pokemon_entry.hpp>
    #include <pkmn/database/lists.hpp>
%}

%include <pkmn/database/item_entry.hpp>
%include <pkmn/database/move_entry.hpp>
%include <java/pkmn_database_levelup_move.i>
%include <pkmn/database/pokemon_entry.hpp>
%include <pkmn/database/lists.hpp>

// std::vector
PKMN_JAVA_VECTOR(pkmn::database::move_entry,    MoveEntry,    MoveEntryVector)
PKMN_JAVA_VECTOR(pkmn::database::pokemon_entry, PokemonEntry, PokemonEntryVector)
