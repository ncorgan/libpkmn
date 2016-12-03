/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/pokemon.hpp>
%}

// SWIG doesn't deal well with read-only static variables
%ignore pkmn::pokemon::LIBPKMN_OT_ID;
%ignore pkmn::pokemon::LIBPKMN_OT_NAME;

%extend pkmn::pokemon {
    %pythoncode %{
        LIBPKMN_OT_ID = 2105214279
        LIBPKMN_OT_NAME = "LibPKMN"
    %}
}

%include <pkmn/pokemon.hpp>
PKMN_PYTHON_SPTR(pokemon)
PKMN_PYTHON_VECTOR(pkmn::pokemon::sptr, pokemon_list)
