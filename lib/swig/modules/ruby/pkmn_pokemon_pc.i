/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include "cpp_wrappers/pokemon_pc.hpp"
%}

%include <std_string.i>

%rename("PokemonPC") pokemon_pc;
%rename("as_list") as_vector;
%rename("game") get_game;
%ignore get_num_boxes;
%rename("box_names") get_box_names;

%include "cpp_wrappers/pokemon_pc.hpp"

%extend pkmn::swig::pokemon_pc
{
    int __len__()
    {
        return self->get_num_boxes();
    }

    pkmn::swig::pokemon_box __getitem__(
        int index
    )
    {
        return self->get_box(index);
    }

    pkmn::swig::pokemon_pc* each()
    {
        if ( !rb_block_given_p() )
            rb_raise( rb_eArgError, "no block given");

        const std::vector<pkmn::swig::pokemon_box>& pokemon_box_list = self->as_vector();

        VALUE r;
        for(auto iter = pokemon_box_list.begin(); iter != pokemon_box_list.end(); ++iter)
        {
            r = swig::from<std::vector<pkmn::swig::pokemon_box>::value_type>(*iter);
            rb_yield(r);
        }

        return self;
    }
}
