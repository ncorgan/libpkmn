/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokemon_party.hpp"
%}

%include <attribute.i>

%ignore pkmn::swig::pokemon_party::pokemon_party();
%ignore pkmn::swig::pokemon_party::pokemon_party(const pkmn::pokemon_party::sptr&);
%ignore pkmn::swig::pokemon_party::get_pokemon(int);
%ignore pkmn::swig::pokemon_party::get_pokemon(int, const pkmn::swig::pokemon&);
%ignore pkmn::swig::pokemon_party::cptr();

// Convert getter/setter functions into attributes for more idiomatic Ruby.

%attributestring(pkmn::swig::pokemon_party, std::string, game, get_game);
%attributestring(pkmn::swig::pokemon_party, int, num_pokemon, get_num_pokemon);

%extend pkmn::swig::pokemon_party
{
    pkmn::swig::pokemon __getitem__(
        size_t position
    )
    {
        return self->get_pokemon(position);
    }

    void __setitem__(
        size_t position,
        const pkmn::swig::pokemon& pokemon
    )
    {
        self->set_pokemon(position, pokemon);
    }

    size_t __len__()
    {
        return 6;
    }

    pkmn::swig::pokemon_party* each()
    {
        if(!rb_block_given_p())
        {
            rb_raise(rb_eArgError, "no block given");
        }

        VALUE r;
        for(int party_index = 0; party_index < 6; ++party_index)
        {
            r = swig::from<pkmn::swig::pokemon>(self->get_pokemon(party_index));
            rb_yield(r);
        }

        return self;
    }
}

%include "cpp_wrappers/pokemon_party.hpp"
