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

%include <pkmn/pokemon.hpp>

%extend pkmn::shared_ptr<pkmn::pokemon> {

    bool __eq__(
        const pkmn::pokemon::sptr &rhs
    ) {
        return (self->get() == rhs.get());
    }

}
%template(pokemon_sptr) pkmn::shared_ptr<pkmn::pokemon>;

%import <lua/stl_macros.i>
PKMN_LUA_VECTOR(pkmn::pokemon::sptr, pokemon_list)
