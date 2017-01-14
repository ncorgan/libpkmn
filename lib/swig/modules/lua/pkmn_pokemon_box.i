/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/pokemon_box.hpp>
%}

%rename(as_list) as_vector;
%include <pkmn/pokemon_box.hpp>

%extend pkmn::shared_ptr<pkmn::pokemon_box> {

    pkmn::pokemon::sptr __getitem__(
        int index
    ) throw (std::out_of_range)
    {
        if (index == 0)
            throw std::out_of_range("Lua lists are 1-based");
        return self->get()->as_vector().at(index-1);
    }

    void __setitem__(
        int index, pkmn::pokemon::sptr value
    ) throw (std::out_of_range)
    {
        if (index == 0)
            throw std::out_of_range("Lua lists are 1-based");
        self->get()->set_pokemon(index-1, value);
    }

    bool __eq__(
        const pkmn::pokemon_box::sptr &rhs
    ) {
        return (self->get() == rhs.get());
    }

    int __len(void*) {
        return int(self->get()->get_capacity());
    }

}
%template(pokemon_box_sptr) pkmn::shared_ptr<pkmn::pokemon_box>;

%import <lua/stl_macros.i>
PKMN_LUA_VECTOR(pkmn::pokemon_box::sptr, pokemon_box_list)
