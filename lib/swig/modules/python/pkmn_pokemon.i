/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/pokemon.hpp>
%}

// SWIG doesn't deal well with read-only static variables
%ignore pkmn::pokemon::DEFAULT_TRAINER_ID;
%ignore pkmn::pokemon::DEFAULT_TRAINER_NAME;

%include <pkmn/pokemon.hpp>
PKMN_PYTHON_SPTR(pokemon)
PKMN_PYTHON_VECTOR(pkmn::pokemon::sptr, pokemon_list)
