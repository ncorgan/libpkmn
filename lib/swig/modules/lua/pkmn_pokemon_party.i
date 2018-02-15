/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/pokemon_party.hpp>
%}

%ignore pkmn::pokemon_party::set_pokemon;
%rename(as_list) as_vector;
%include <pkmn/pokemon_party.hpp>

%extend std::shared_ptr<pkmn::pokemon_party> {

    void set_pokemon(
        int index,
        pkmn::pokemon::sptr pokemon
    ) throw (std::out_of_range)
    {
        if (index == 0)
            throw std::out_of_range("Lua lists are 1-based");
        self->get()->set_pokemon(index-1, pokemon);
    }

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
        const pkmn::pokemon_party::sptr &rhs
    ) {
        return (self->get() == rhs.get());
    }

    int __len(void*) {
        (void)self;
        return 6;
    }

}
%template(pokemon_party_sptr) std::shared_ptr<pkmn::pokemon_party>;
