/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/pokemon2.h>

#include <cstdio>

#define INTERNAL_RCAST(ptr) (reinterpret_cast<pkmn_pokemon_internal_t*>(ptr))

// The caller is expected to be exception-safe.
void init_pokemon(
    pkmn::pokemon::sptr cpp_pokemon,
    pkmn_pokemon2_t* pokemon_ptr
)
{
    BOOST_ASSERT(pokemon_ptr);
    BOOST_ASSERT(cpp_pokemon.get());

    pokemon_ptr->_internal = new pkmn_pokemon_internal_t;
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);

    internal_ptr->cpp = cpp_pokemon;
    internal_ptr->last_error = "None";

    pkmn::c::string_cpp_to_c_alloc(
        internal_ptr->cpp->get_species(),
        &pokemon_ptr->species
    );
    pkmn::c::string_cpp_to_c_alloc(
        internal_ptr->cpp->get_game(),
        &pokemon_ptr->game
    );
}

pkmn_error_t pkmn_pokemon_init(
    const char* species,
    const char* game,
    const char* form,
    int level,
    pkmn_pokemon2_t* pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(species);
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(form);
    PKMN_CHECK_NULL_PARAM(pokemon_out);

    PKMN_CPP_TO_C(
        pkmn::pokemon::sptr cpp = pkmn::pokemon::make(
                                      species,
                                      game,
                                      form,
                                      level
                                  );

        init_pokemon(
            cpp,
            pokemon_out
        );
    )
}

pkmn_error_t pkmn_pokemon_free(
    pkmn_pokemon2_t* pokemon_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);

    pokemon_ptr->species = nullptr;
    pokemon_ptr->game = nullptr;

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_pokemon_internal_t**>(&pokemon_ptr->_internal)
        );
    )
}

const char* pkmn_pokemon_strerror(
    pkmn_pokemon2_t* pokemon_ptr
)
{
    if(!pokemon_ptr)
    {
        return nullptr;
    }

    try
    {
        pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
        if(!internal_ptr)
        {
            return nullptr;
        }

        boost::mutex::scoped_lock lock(internal_ptr->error_mutex);
        return internal_ptr->last_error.c_str();
    }
    catch(...)
    {
        return nullptr;
    }
}
