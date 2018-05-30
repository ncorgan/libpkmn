/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/daycare_helpers.hpp"
%}

//
// Levelup Pokémon
//

%ignore pkmn::swig::daycare_levelup_pokemon::daycare_levelup_pokemon(const pkmn::daycare::sptr&);
%ignore pkmn::swig::daycare_levelup_pokemon::get_capacity();
%ignore pkmn::swig::daycare_levelup_pokemon::get_pokemon(int);
%ignore pkmn::swig::daycare_levelup_pokemon::set_pokemon(int, const pkmn::swig::pokemon&);
%ignore pkmn::swig::daycare_levelup_pokemon::cptr();

// Replace methods with more idiomatic Lua.

%extend pkmn::swig::daycare_levelup_pokemon
{
    size_t __len(void*)
    {
        return self->get_capacity();
    }

    pkmn::swig::pokemon __getitem__(size_t index)
    {
        return self->get_pokemon(static_cast<int>(index));
    }

    void __setitem__(
        size_t index,
        const pkmn::swig::pokemon& pokemon
    )
    {
        self->set_pokemon(
            static_cast<int>(index),
            pokemon
        );
    }
}

//
// Breeding Pokémon
//

%ignore pkmn::swig::daycare_breeding_pokemon::daycare_breeding_pokemon(const pkmn::daycare::sptr&);
%ignore pkmn::swig::daycare_breeding_pokemon::get_capacity();
%ignore pkmn::swig::daycare_breeding_pokemon::get_pokemon(int);
%ignore pkmn::swig::daycare_breeding_pokemon::set_pokemon(int, const pkmn::swig::pokemon&);
%ignore pkmn::swig::daycare_breeding_pokemon::cptr();

// Replace methods with more idiomatic Lua.

%extend pkmn::swig::daycare_breeding_pokemon
{
    size_t __len(void*)
    {
        return self->get_capacity();
    }

    pkmn::swig::pokemon __getitem__(size_t index)
    {
        return self->get_pokemon(static_cast<int>(index));
    }

    void __setitem__(
        size_t index,
        const pkmn::swig::pokemon& pokemon
    )
    {
        self->set_pokemon(
            static_cast<int>(index),
            pokemon
        );
    }
}

%include "cpp_wrappers/daycare_helpers.hpp"
