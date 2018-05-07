/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/pokemon_party.h>

enum pkmn_error pkmn_pokemon_party_init(
    const char* game,
    struct pkmn_pokemon_party* p_pokemon_party_out
)
{
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(p_pokemon_party_out);

    PKMN_CPP_TO_C(
        pkmn::pokemon_party::sptr cpp = pkmn::pokemon_party::make(game);

        pkmn::c::init_pokemon_party(
            cpp,
            p_pokemon_party_out
        );
    )
}

enum pkmn_error pkmn_pokemon_party_free(
    struct pkmn_pokemon_party* p_pokemon_party
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_party);

    pkmn::c::free_pointer_and_set_to_null(&p_pokemon_party->p_game);
    p_pokemon_party->capacity = 0;

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_pokemon_party_internal_t**>(&p_pokemon_party->p_internal)
        );
    )
}

const char* pkmn_pokemon_party_strerror(
    struct pkmn_pokemon_party* p_pokemon_party
)
{
    if(!p_pokemon_party)
    {
        return nullptr;
    }

    try
    {
        pkmn_pokemon_party_internal_t* internal_ptr = POKEMON_PARTY_INTERNAL_RCAST(p_pokemon_party->p_internal);
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

enum pkmn_error pkmn_pokemon_party_get_num_pokemon(
    struct pkmn_pokemon_party* p_pokemon_party,
    size_t* p_num_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_party);
    pkmn_pokemon_party_internal_t* internal_ptr = POKEMON_PARTY_INTERNAL_RCAST(p_pokemon_party->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_num_pokemon_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *p_num_pokemon_out = internal_ptr->cpp->get_num_pokemon();
    )
}

enum pkmn_error pkmn_pokemon_party_get_pokemon(
    struct pkmn_pokemon_party* p_pokemon_party,
    size_t position,
    struct pkmn_pokemon* p_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_party);
    pkmn_pokemon_party_internal_t* internal_ptr = POKEMON_PARTY_INTERNAL_RCAST(p_pokemon_party->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_pokemon_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::init_pokemon(
            internal_ptr->cpp->get_pokemon(int(position)),
            p_pokemon_out
        );
    )
}

enum pkmn_error pkmn_pokemon_party_set_pokemon(
    struct pkmn_pokemon_party* p_pokemon_party,
    size_t position,
    struct pkmn_pokemon* p_pokemon
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_party);
    pkmn_pokemon_party_internal_t* internal_ptr = POKEMON_PARTY_INTERNAL_RCAST(p_pokemon_party->p_internal);
    PKMN_CHECK_NULL_WRAPPER_PARAM_WITH_HANDLE(p_pokemon, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn_pokemon_internal_t* new_pokemon_internal_ptr = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

        internal_ptr->cpp->set_pokemon(
            int(position),
            new_pokemon_internal_ptr->cpp
        );
    )
}

enum pkmn_error pkmn_pokemon_party_as_list(
    struct pkmn_pokemon_party* p_pokemon_party,
    struct pkmn_pokemon_list* pokemon_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_party);
    pkmn_pokemon_party_internal_t* internal_ptr = POKEMON_PARTY_INTERNAL_RCAST(p_pokemon_party->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_list_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::pokemon_list_cpp_to_c(
            internal_ptr->cpp->as_vector(),
            pokemon_list_out
        );
    )
}
