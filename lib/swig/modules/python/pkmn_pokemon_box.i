/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokemon_box.hpp"
%}

%include <attribute.i>

%ignore pkmn::swig::pokemon_box::pokemon_box();
%ignore pkmn::swig::pokemon_box::pokemon_box(const pkmn::pokemon_box::sptr&);
%ignore pkmn::swig::pokemon_box::cptr();
%ignore pkmn::swig::pokemon_box::at(int);
%ignore pkmn::swig::pokemon_box::get_capacity();

// Convert getter/setter functions into attributes for more idiomatic Python.

%attributestring(pkmn::swig::pokemon_box, std::string, game, get_game);
%attributestring(pkmn::swig::pokemon_box, std::string, name, get_name, set_name);
%attributestring(pkmn::swig::pokemon_box, int, num_pokemon, get_num_pokemon);

%extend pkmn::swig::pokemon_box
{
    pkmn::swig::pokemon __getitem__(
        int position
    )
    {
        return self->get_pokemon(position);
    }

    void __setitem__(
        int position,
        const pkmn::swig::pokemon& pokemon
    )
    {
        self->set_pokemon(position, pokemon);
    }

    int __len__()
    {
        return self->get_capacity();
    }
}

%include "cpp_wrappers/pokemon_box.hpp"
