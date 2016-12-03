/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/pokemon_box.hpp>
%}

%extend pkmn::shared_ptr<pkmn::pokemon_box> {
    pkmn::pokemon::sptr __getitem__(
        int index
    ) {
        return self->get()->get_pokemon(index);
    }

    int __len__() {
        return self->get()->get_capacity();
    }
}

%include <pkmn/pokemon_box.hpp>
PKMN_PYTHON_SPTR(pokemon_box)
