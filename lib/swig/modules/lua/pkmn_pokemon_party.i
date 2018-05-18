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

// Convert getter/setter functions into attributes for more idiomatic Lua.

%attributestring(pkmn::swig::pokemon_party, std::string, game, get_game);
%attributestring(pkmn::swig::pokemon_party, int, num_pokemon, get_num_pokemon);

%extend pkmn::swig::pokemon_party
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

    int __len(void*)
    {
        return 6;
    }
}

%include "cpp_wrappers/pokemon_party.hpp"
