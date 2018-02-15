/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/pokemon_pc.hpp>
%}

%extend std::shared_ptr<pkmn::pokemon_pc> {
    pkmn::pokemon_box::sptr __getitem__(
        int index
    ) {
        return self->get()->get_box(index);
    }

    int __len__() {
        return self->get()->get_num_boxes();
    }
}

%include <pkmn/pokemon_pc.hpp>
PKMN_PYTHON_SPTR(pokemon_pc)
