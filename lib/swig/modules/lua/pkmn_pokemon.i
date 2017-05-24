/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/exception.hpp>
    #include <pkmn/pokemon.hpp>
%}

// SWIG doesn't deal well with read-only static variables
%ignore pkmn::pokemon::LIBPKMN_OT_ID;
%ignore pkmn::pokemon::LIBPKMN_OT_NAME;
%ignore pkmn::pokemon::set_move;

%include <pkmn/pokemon.hpp>

%extend pkmn::shared_ptr<pkmn::pokemon> {

    void set_move(
        const std::string &move_name,
        int index
    ) {
        if(index < 1 or index > 4) {
            pkmn::throw_out_of_range("index", 1, 4);
        }

        self->get()->set_move(move_name, index-1);
    }

    bool __eq__(
        const pkmn::pokemon::sptr &rhs
    ) {
        return (self->get() == rhs.get());
    }

}
%template(pokemon_sptr) pkmn::shared_ptr<pkmn::pokemon>;

%import <lua/stl_macros.i>
PKMN_LUA_VECTOR(pkmn::pokemon::sptr, pokemon_list)
