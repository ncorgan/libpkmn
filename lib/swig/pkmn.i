/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * This file is to be included by all SWIG modules. It contains all global
 * settings and ignores.
 */

%{
    #include <pkmn/config.hpp>
%}

%ignore PKMN_ENABLE_QT;
%ignore PKMN_QT4;
%ignore PKMN_QT5;

%include <pkmn/config.hpp>

// SWIG can't parse constexpr
#define constexpr

/*
 * All SWIG modules will ignore the following functions/classes.
 */

%ignore pkmn::database::item_entry::item_entry(int, int);
%ignore pkmn::database::move_entry::move_entry(int, int);
%ignore pkmn::database::pokemon_entry::pokemon_entry(int, int);
%ignore pkmn::database::pokemon_entry::pokemon_entry(int, int, int);
%ignore get_item_id;
%ignore get_game_id;
%ignore get_move_id;
%ignore get_pokemon_id;
%ignore get_species_id;
%ignore get_native;
%ignore make;

#if !defined(SWIGLUA) && !defined(SWIGPYTHON)
%ignore operator ==;
%ignore operator !=;
%ignore operator +;
%ignore operator +=;
#endif
