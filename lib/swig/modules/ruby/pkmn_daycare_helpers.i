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

// Replace methods with more idiomatic Ruby.

%extend pkmn::swig::daycare_levelup_pokemon
{
    size_t __len__()
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

    pkmn::swig::daycare_levelup_pokemon* each()
    {
        if(!rb_block_given_p())
        {
            rb_raise(rb_eArgError, "no block given");
        }

        VALUE r;
        for(size_t levelup_pokemon_index = 0;
            levelup_pokemon_index < self->get_capacity();
            ++levelup_pokemon_index)
        {
            r = swig::from<pkmn::swig::pokemon>(
                    self->get_pokemon(static_cast<int>(levelup_pokemon_index))
                );
            rb_yield(r);
        }

        return self;
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

// Replace methods with more idiomatic Ruby.

%extend pkmn::swig::daycare_breeding_pokemon
{
    size_t __len__()
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

    pkmn::swig::daycare_breeding_pokemon* each()
    {
        if(!rb_block_given_p())
        {
            rb_raise(rb_eArgError, "no block given");
        }

        VALUE r;
        for(size_t breeding_pokemon_index = 0;
            breeding_pokemon_index < self->get_capacity();
            ++breeding_pokemon_index)
        {
            r = swig::from<pkmn::swig::pokemon>(
                    self->get_pokemon(static_cast<int>(breeding_pokemon_index))
                );
            rb_yield(r);
        }

        return self;
    }
}

%include "cpp_wrappers/daycare_helpers.hpp"
