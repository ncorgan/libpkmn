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

pkmn_error_t pkmn_pokedex_init(
    const char* game,
    pkmn_pokedex_t* pokedex_out
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

pkmn_error_t pkmn_pokedex_free(
    pkmn_pokedex_t* pokedex_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokedex_ptr);

    pkmn::c::free_pointer_and_set_to_null(&pokedex_ptr->game);

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_pokedex_internal_t**>(&pokedex_ptr->_internal)
        );
    )
}

const char* pkmn_pokedex_strerror(
    pkmn_pokedex_t* pokedex_ptr
)
{
    if(!pokedex_ptr)
    {
        return nullptr;
    }

    try
    {
        pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->_internal);
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

pkmn_error_t pkmn_pokedex_has_seen(
    pkmn_pokedex_t* pokedex_ptr,
    const char* species,
    bool* has_seen_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(species, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(has_seen_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *has_seen_out = internal_ptr->cpp->has_seen(species);
    )
}

pkmn_error_t pkmn_pokedex_set_has_seen(
    pkmn_pokedex_t* pokedex_ptr,
    const char* species,
    bool has_seen
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(species, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_has_seen(species, has_seen);
    )
}

pkmn_error_t pkmn_pokedex_get_all_seen(
    pkmn_pokedex_t* pokedex_ptr,
    pkmn_string_list_t* all_seen_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(all_seen_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_list_cpp_to_c(
            internal_ptr->cpp->get_all_seen(),
            all_seen_out
        );
    )
}

pkmn_error_t pkmn_pokedex_get_num_seen(
    pkmn_pokedex_t* pokedex_ptr,
    size_t* num_seen_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(num_seen_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *num_seen_out = internal_ptr->cpp->get_num_seen();
    )
}

pkmn_error_t pkmn_pokedex_has_caught(
    pkmn_pokedex_t* pokedex_ptr,
    const char* species,
    bool* has_caught_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(species, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(has_caught_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *has_caught_out = internal_ptr->cpp->has_caught(species);
    )
}

pkmn_error_t pkmn_pokedex_set_has_caught(
    pkmn_pokedex_t* pokedex_ptr,
    const char* species,
    bool has_caught
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(species, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_has_caught(species, has_caught);
    )
}

pkmn_error_t pkmn_pokedex_get_all_caught(
    pkmn_pokedex_t* pokedex_ptr,
    pkmn_string_list_t* all_caught_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(all_caught_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_list_cpp_to_c(
            internal_ptr->cpp->get_all_caught(),
            all_caught_out
        );
    )
}

pkmn_error_t pkmn_pokedex_get_num_caught(
    pkmn_pokedex_t* pokedex_ptr,
    size_t* num_caught_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(pokedex_ptr);
    pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(pokedex_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(num_caught_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *num_caught_out = internal_ptr->cpp->get_num_caught();
    )
}
