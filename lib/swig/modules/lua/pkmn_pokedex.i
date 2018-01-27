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

%extend std::shared_ptr<pkmn::pokedex> {

    bool __eq__(
        const pkmn::pokedex::sptr &rhs
    ) {
        return (self->get() == rhs.get());
    }

}
%template(pokedex_sptr) std::shared_ptr<pkmn::pokedex>;
