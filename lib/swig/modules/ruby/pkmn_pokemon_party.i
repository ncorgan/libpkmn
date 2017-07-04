/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include "cpp_wrappers/pokemon_party.hpp"
%}

%include <std_string.i>

%rename("as_list") as_vector;
%rename("game") get_game;
%rename("num_pokemon") get_num_pokemon;

%include "cpp_wrappers/pokemon_party.hpp"

%extend pkmn::swig::pokemon_party
{
    unsigned int __len__()
    {
        (void)self;
        return 6;
    }

    pkmn::swig::pokemon __getitem__(
        unsigned int index
    )
    {
        return self->get_pokemon(index);
    }

    void __setitem__(
        unsigned int index,
        const pkmn::swig::pokemon& pokemon
    )
    {
        self->set_pokemon(index, pokemon);
    }
}
