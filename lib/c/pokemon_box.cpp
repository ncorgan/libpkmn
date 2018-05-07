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
    const char* p_game,
    struct pkmn_pokemon_box* p_pokemon_box_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game);
    PKMN_CHECK_NULL_PARAM(p_pokemon_box_out);

    PKMN_CPP_TO_C(
        pkmn::pokemon_box::sptr cpp = pkmn::pokemon_box::make(p_game);

        pkmn::c::init_pokemon_box(
            cpp,
            p_pokemon_box_out
        );
    )
}

enum pkmn_error pkmn_pokemon_box_free(
    struct pkmn_pokemon_box* p_pokemon_box
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_box);

    pkmn::c::free_pointer_and_set_to_null(&p_pokemon_box->p_game);
    p_pokemon_box->capacity = 0;

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_pokemon_box_internal_t**>(&p_pokemon_box->p_internal)
        );
    )
}

const char* pkmn_pokemon_box_strerror(
    struct pkmn_pokemon_box* p_pokemon_box
)
{
    if(!p_pokemon_box)
    {
        return nullptr;
    }

    try
    {
        pkmn_pokemon_box_internal_t* p_internal = POKEMON_BOX_INTERNAL_RCAST(p_pokemon_box->p_internal);
        if(!p_internal)
        {
            return nullptr;
        }

        boost::mutex::scoped_lock lock(p_internal->error_mutex);
        return p_internal->last_error.c_str();
    }
    catch(...)
    {
        return nullptr;
    }
}

enum pkmn_error pkmn_pokemon_box_get_name(
    struct pkmn_pokemon_box* p_pokemon_box,
    char* name_buffer_out,
    size_t name_buffer_length,
    size_t* actual_name_length_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_box);
    pkmn_pokemon_box_internal_t* p_internal = POKEMON_BOX_INTERNAL_RCAST(p_pokemon_box->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(name_buffer_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_cpp_to_c(
            p_internal->cpp->get_name(),
            name_buffer_out,
            name_buffer_length,
            actual_name_length_out
        );
    )
}

enum pkmn_error pkmn_pokemon_box_set_name(
    struct pkmn_pokemon_box* p_pokemon_box,
    const char* name
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_box);
    pkmn_pokemon_box_internal_t* p_internal = POKEMON_BOX_INTERNAL_RCAST(p_pokemon_box->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(name, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_name(name);
    )
}

enum pkmn_error pkmn_pokemon_box_get_num_pokemon(
    struct pkmn_pokemon_box* p_pokemon_box,
    size_t* num_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_box);
    pkmn_pokemon_box_internal_t* p_internal = POKEMON_BOX_INTERNAL_RCAST(p_pokemon_box->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(num_pokemon_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *num_pokemon_out = p_internal->cpp->get_num_pokemon();
    )
}

enum pkmn_error pkmn_pokemon_box_get_pokemon(
    struct pkmn_pokemon_box* p_pokemon_box,
    size_t position,
    struct pkmn_pokemon* pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_box);
    pkmn_pokemon_box_internal_t* p_internal = POKEMON_BOX_INTERNAL_RCAST(p_pokemon_box->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::init_pokemon(
            p_internal->cpp->get_pokemon(int(position)),
            pokemon_out
        );
    )
}

enum pkmn_error pkmn_pokemon_box_set_pokemon(
    struct pkmn_pokemon_box* p_pokemon_box,
    size_t position,
    struct pkmn_pokemon* pokemon_ptr
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_box);
    pkmn_pokemon_box_internal_t* p_internal = POKEMON_BOX_INTERNAL_RCAST(p_pokemon_box->p_internal);
    PKMN_CHECK_NULL_WRAPPER_PARAM_WITH_HANDLE(pokemon_ptr, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn_pokemon_internal_t* new_pokemon_p_internal = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

        p_internal->cpp->set_pokemon(
            int(position),
            new_pokemon_p_internal->cpp
        );
    )
}

enum pkmn_error pkmn_pokemon_box_as_list(
    struct pkmn_pokemon_box* p_pokemon_box,
    struct pkmn_pokemon_list* pokemon_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_box);
    pkmn_pokemon_box_internal_t* p_internal = POKEMON_BOX_INTERNAL_RCAST(p_pokemon_box->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_list_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::pokemon_list_cpp_to_c(
            p_internal->cpp->as_vector(),
            pokemon_list_out
        );
    )
}
