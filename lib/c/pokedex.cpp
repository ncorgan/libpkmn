/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/item_list.h>

#include <cstdio>

enum pkmn_error pkmn_pokedex_init(
    const char* game,
    struct pkmn_pokedex* pokedex_out
)
{
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(pokedex_out);

    PKMN_CPP_TO_C(
        pkmn::pokedex::sptr cpp = pkmn::pokedex::make(game);

        pkmn::c::init_pokedex(
            cpp,
            pokedex_out
        );
    )
}

enum pkmn_error pkmn_pokedex_free(
    struct pkmn_pokedex* pokedex_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokedex_ptr);

    pkmn::c::free_pointer_and_set_to_null(&pokedex_ptr->p_game);

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_pokedex_internal_t**>(&pokedex_ptr->p_internal)
        );
    )
}

const char* pkmn_pokedex_strerror(
    struct pkmn_pokedex* pokedex_ptr
)
{
    if(!pokedex_ptr)
    {
        return nullptr;
    }

    try
    {
        pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->p_internal);
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

enum pkmn_error pkmn_pokedex_has_seen(
    struct pkmn_pokedex* pokedex_ptr,
    const char* species,
    bool* has_seen_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(species, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(has_seen_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *has_seen_out = internal_ptr->cpp->has_seen(species);
    )
}

enum pkmn_error pkmn_pokedex_set_has_seen(
    struct pkmn_pokedex* pokedex_ptr,
    const char* species,
    bool has_seen
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(species, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_has_seen(species, has_seen);
    )
}

enum pkmn_error pkmn_pokedex_get_all_seen(
    struct pkmn_pokedex* pokedex_ptr,
    struct pkmn_string_list* all_seen_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(all_seen_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_list_cpp_to_c(
            internal_ptr->cpp->get_all_seen(),
            all_seen_out
        );
    )
}

enum pkmn_error pkmn_pokedex_get_num_seen(
    struct pkmn_pokedex* pokedex_ptr,
    size_t* num_seen_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(num_seen_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *num_seen_out = internal_ptr->cpp->get_num_seen();
    )
}

enum pkmn_error pkmn_pokedex_has_caught(
    struct pkmn_pokedex* pokedex_ptr,
    const char* species,
    bool* has_caught_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(species, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(has_caught_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *has_caught_out = internal_ptr->cpp->has_caught(species);
    )
}

enum pkmn_error pkmn_pokedex_set_has_caught(
    struct pkmn_pokedex* pokedex_ptr,
    const char* species,
    bool has_caught
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(species, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_has_caught(species, has_caught);
    )
}

enum pkmn_error pkmn_pokedex_get_all_caught(
    struct pkmn_pokedex* pokedex_ptr,
    struct pkmn_string_list* all_caught_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(all_caught_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_list_cpp_to_c(
            internal_ptr->cpp->get_all_caught(),
            all_caught_out
        );
    )
}

enum pkmn_error pkmn_pokedex_get_num_caught(
    struct pkmn_pokedex* pokedex_ptr,
    size_t* num_caught_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(num_caught_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *num_caught_out = internal_ptr->cpp->get_num_caught();
    )
}
