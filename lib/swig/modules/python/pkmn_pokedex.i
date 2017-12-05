/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/pokedex.hpp>
%}

%ignore pkmn::pokedex::get_native_has_seen;
%ignore pkmn::pokedex::get_native_has_caught;

%include <pkmn/pokedex.hpp>
PKMN_PYTHON_SPTR(pokedex)
