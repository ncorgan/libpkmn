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
    int __len__()
    {
        (void)self;
        return 6;
    }

    pkmn::swig::pokemon __getitem__(
        int index
    )
    {
        return self->get_pokemon(index);
    }

    void __setitem__(
        int index,
        const pkmn::swig::pokemon& pokemon
    )
    {
        self->set_pokemon(index, pokemon);
    }

    pkmn::swig::pokemon_party* each()
    {
        if ( !rb_block_given_p() )
            rb_raise( rb_eArgError, "no block given");

        const std::vector<pkmn::swig::pokemon>& pokemon_list = self->as_vector();

        VALUE r;
        for(auto iter = pokemon_list.begin(); iter != pokemon_list.end(); ++iter)
        {
            r = swig::from<std::vector<pkmn::swig::pokemon>::value_type>(*iter);
            rb_yield(r);
        }

        return self;
    }
}
