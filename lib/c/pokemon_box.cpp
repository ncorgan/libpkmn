/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "common/misc.hpp"

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
            reinterpret_cast<pkmn::c::pokemon_box_internal_t**>(&p_pokemon_box->p_internal)
        );
    )
}

const char* pkmn_pokemon_box_strerror(
    const struct pkmn_pokemon_box* p_pokemon_box
)
{
    return pkmn::c::strerror<struct pkmn_pokemon_box, pkmn::pokemon_box>(p_pokemon_box);
}

enum pkmn_error pkmn_pokemon_box_get_name(
    const struct pkmn_pokemon_box* p_pokemon_box,
    char* p_name_buffer_out,
    size_t name_buffer_length,
    size_t* p_actual_name_length_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_box);
    pkmn::c::pokemon_box_internal_t* p_internal = POKEMON_BOX_INTERNAL_RCAST(p_pokemon_box->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_name_buffer_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_cpp_to_c(
            p_internal->cpp->get_name(),
            p_name_buffer_out,
            name_buffer_length,
            p_actual_name_length_out
        );
    )
}

enum pkmn_error pkmn_pokemon_box_set_name(
    const struct pkmn_pokemon_box* p_pokemon_box,
    const char* p_name
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_box);
    pkmn::c::pokemon_box_internal_t* p_internal = POKEMON_BOX_INTERNAL_RCAST(p_pokemon_box->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_name, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_name(p_name);
    )
}

enum pkmn_error pkmn_pokemon_box_get_num_pokemon(
    const struct pkmn_pokemon_box* p_pokemon_box,
    size_t* p_num_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_box);
    pkmn::c::pokemon_box_internal_t* p_internal = POKEMON_BOX_INTERNAL_RCAST(p_pokemon_box->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_num_pokemon_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_num_pokemon_out = p_internal->cpp->get_num_pokemon();
    )
}

enum pkmn_error pkmn_pokemon_box_get_pokemon(
    const struct pkmn_pokemon_box* p_pokemon_box,
    size_t position,
    struct pkmn_pokemon* p_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_box);
    pkmn::c::pokemon_box_internal_t* p_internal = POKEMON_BOX_INTERNAL_RCAST(p_pokemon_box->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_pokemon_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::init_pokemon(
            p_internal->cpp->get_pokemon(int(position)),
            p_pokemon_out
        );
    )
}

enum pkmn_error pkmn_pokemon_box_set_pokemon(
    const struct pkmn_pokemon_box* p_pokemon_box,
    size_t position,
    struct pkmn_pokemon* p_pokemon
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_box);
    pkmn::c::pokemon_box_internal_t* p_internal = POKEMON_BOX_INTERNAL_RCAST(p_pokemon_box->p_internal);
    PKMN_CHECK_NULL_WRAPPER_PARAM_WITH_HANDLE(p_pokemon, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::pokemon_internal_t* p_new_pokemon_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

        p_internal->cpp->set_pokemon(
            int(position),
            p_new_pokemon_internal->cpp
        );
    )
}

enum pkmn_error pkmn_pokemon_box_as_list(
    const struct pkmn_pokemon_box* p_pokemon_box,
    struct pkmn_pokemon_list* p_pokemon_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_box);
    pkmn::c::pokemon_box_internal_t* p_internal = POKEMON_BOX_INTERNAL_RCAST(p_pokemon_box->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_pokemon_list_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::pokemon_list_cpp_to_c(
            p_internal->cpp->as_vector(),
            p_pokemon_list_out
        );
    )
}
