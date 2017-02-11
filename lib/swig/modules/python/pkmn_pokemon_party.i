/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/pokemon_party.hpp>

%}

%extend pkmn::shared_ptr<pkmn::pokemon_party> {

    pkmn::pokemon::sptr __getitem__(
        int index
    ) {
        return self->get()->get_pokemon(index);
    }

    void __setitem__(
        int index,
        pkmn::pokemon::sptr new_pokemon
    ) {
        self->get()->set_pokemon(
            index,
            new_pokemon
        );
    }

    int __len__() {
        (void)self;
        return 6;
    }
}

%include <pkmn/pokemon_party.hpp>
PKMN_PYTHON_SPTR(pokemon_party)
