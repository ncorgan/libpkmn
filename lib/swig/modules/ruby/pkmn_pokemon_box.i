/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include "cpp_wrappers/pokemon_box.hpp"
%}

%include <std_string.i>

%rename("PokemonBox") pokemon_box;
%rename("as_list") as_vector;
%rename("name") get_name;
%rename("name=") set_name;
%rename("game") get_game;
%rename("num_pokemon") get_num_pokemon;

%include "cpp_wrappers/pokemon_box.hpp"

%extend pkmn::swig::pokemon_box
{
    unsigned int __len__()
    {
        return (unsigned int)self->get_capacity();
    }

    pkmn::swig::pokemon __getitem__(
        unsigned int index
    )
    {
        return self->get_pokemon((int)index);
    }

    void __setitem__(
        unsigned int index,
        const pkmn::swig::pokemon& pokemon
    ) {
        self->set_pokemon((int)index, pokemon);
    }
}

PKMN_RUBY_VECTOR(pkmn::swig::pokemon_box, PokemonBoxList);
