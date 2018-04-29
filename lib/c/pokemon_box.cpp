/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/pokemon_box.h>

enum pkmn_error pkmn_pokemon_box_init(
    const char* game,
    struct pkmn_pokemon_box* pokemon_box_out
)
{
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(pokemon_box_out);

    PKMN_CPP_TO_C(
        pkmn::pokemon_box::sptr cpp = pkmn::pokemon_box::make(game);

        pkmn::c::init_pokemon_box(
            cpp,
            pokemon_box_out
        );
    )
}

enum pkmn_error pkmn_pokemon_box_free(
    struct pkmn_pokemon_box* pokemon_box_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_box_ptr);

    pkmn::c::free_pointer_and_set_to_null(&pokemon_box_ptr->game);
    pokemon_box_ptr->capacity = 0;

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_pokemon_box_internal_t**>(&pokemon_box_ptr->_internal)
        );
    )
}

const char* pkmn_pokemon_box_strerror(
    struct pkmn_pokemon_box* pokemon_box_ptr
)
{
    if(!pokemon_box_ptr)
    {
        return nullptr;
    }

    try
    {
        pkmn_pokemon_box_internal_t* internal_ptr = POKEMON_BOX_INTERNAL_RCAST(pokemon_box_ptr->_internal);
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

enum pkmn_error pkmn_pokemon_box_get_name(
    struct pkmn_pokemon_box* pokemon_box_ptr,
    char* name_buffer_out,
    size_t name_buffer_length,
    size_t* actual_name_length_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_box_ptr);
    pkmn_pokemon_box_internal_t* internal_ptr = POKEMON_BOX_INTERNAL_RCAST(pokemon_box_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(name_buffer_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_cpp_to_c(
            internal_ptr->cpp->get_name(),
            name_buffer_out,
            name_buffer_length,
            actual_name_length_out
        );
    )
}

enum pkmn_error pkmn_pokemon_box_set_name(
    struct pkmn_pokemon_box* pokemon_box_ptr,
    const char* name
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_box_ptr);
    pkmn_pokemon_box_internal_t* internal_ptr = POKEMON_BOX_INTERNAL_RCAST(pokemon_box_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(name, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_name(name);
    )
}

enum pkmn_error pkmn_pokemon_box_get_num_pokemon(
    struct pkmn_pokemon_box* pokemon_box_ptr,
    size_t* num_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_box_ptr);
    pkmn_pokemon_box_internal_t* internal_ptr = POKEMON_BOX_INTERNAL_RCAST(pokemon_box_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(num_pokemon_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *num_pokemon_out = internal_ptr->cpp->get_num_pokemon();
    )
}

enum pkmn_error pkmn_pokemon_box_get_pokemon(
    struct pkmn_pokemon_box* pokemon_box_ptr,
    size_t position,
    struct pkmn_pokemon* pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_box_ptr);
    pkmn_pokemon_box_internal_t* internal_ptr = POKEMON_BOX_INTERNAL_RCAST(pokemon_box_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::init_pokemon(
            internal_ptr->cpp->get_pokemon(int(position)),
            pokemon_out
        );
    )
}

enum pkmn_error pkmn_pokemon_box_set_pokemon(
    struct pkmn_pokemon_box* pokemon_box_ptr,
    size_t position,
    struct pkmn_pokemon* pokemon_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_box_ptr);
    pkmn_pokemon_box_internal_t* internal_ptr = POKEMON_BOX_INTERNAL_RCAST(pokemon_box_ptr->_internal);
    PKMN_CHECK_NULL_WRAPPER_PARAM_WITH_HANDLE(pokemon_ptr, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn_pokemon_internal_t* new_pokemon_internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

        internal_ptr->cpp->set_pokemon(
            int(position),
            new_pokemon_internal_ptr->cpp
        );
    )
}

enum pkmn_error pkmn_pokemon_box_as_list(
    struct pkmn_pokemon_box* pokemon_box_ptr,
    struct pkmn_pokemon_list* pokemon_list_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_box_ptr);
    pkmn_pokemon_box_internal_t* internal_ptr = POKEMON_BOX_INTERNAL_RCAST(pokemon_box_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_list_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::pokemon_list_cpp_to_c(
            internal_ptr->cpp->as_vector(),
            pokemon_list_out
        );
    )
}
