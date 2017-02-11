/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/pokemon_pc.hpp>
%}

%rename(as_list) as_vector;
%include <pkmn/pokemon_pc.hpp>

%extend pkmn::shared_ptr<pkmn::pokemon_pc> {

    pkmn::pokemon_box::sptr __getitem__(
        int index
    ) throw (std::out_of_range)
    {
        if (index == 0)
            throw std::out_of_range("Lua lists are 1-based");
        return self->get()->as_vector().at(index-1);
    }

    bool __eq__(
        const pkmn::pokemon_pc::sptr &rhs
    ) {
        return (self->get() == rhs.get());
    }

    int __len(void*) {
        return int(self->get()->get_num_boxes());
    }

}
%template(pokemon_pc_sptr) pkmn::shared_ptr<pkmn::pokemon_pc>;
