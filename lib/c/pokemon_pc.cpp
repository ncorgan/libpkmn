/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include "common/misc.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/pokemon_pc.h>

enum pkmn_error pkmn_pokemon_pc_init(
    const char* p_game,
    struct pkmn_pokemon_pc* p_pokemon_pc_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game);
    PKMN_CHECK_NULL_PARAM(p_pokemon_pc_out);

    PKMN_CPP_TO_C(
        pkmn::pokemon_pc::sptr cpp = pkmn::pokemon_pc::make(p_game);

        pkmn::c::init_pokemon_pc(
            cpp,
            p_pokemon_pc_out
        );
    )
}

enum pkmn_error pkmn_pokemon_pc_free(
    struct pkmn_pokemon_pc* p_pokemon_pc
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_pc);

    pkmn::c::free_pointer_and_set_to_null(&p_pokemon_pc->p_game);
    p_pokemon_pc->capacity = 0;

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn::c::pokemon_pc_internal_t**>(&p_pokemon_pc->p_internal)
        );
    )
}

const char* pkmn_pokemon_pc_strerror(
    const struct pkmn_pokemon_pc* p_pokemon_pc
)
{
    if(!p_pokemon_pc)
    {
        return nullptr;
    }

    try
    {
        pkmn::c::pokemon_pc_internal_t* p_internal = POKEMON_PC_INTERNAL_RCAST(p_pokemon_pc->p_internal);
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

enum pkmn_error pkmn_pokemon_pc_get_box(
    const struct pkmn_pokemon_pc* p_pokemon_pc,
    size_t index,
    struct pkmn_pokemon_box* p_pokemon_box_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_pc);
    pkmn::c::pokemon_pc_internal_t* p_internal = POKEMON_PC_INTERNAL_RCAST(p_pokemon_pc->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_pokemon_box_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::init_pokemon_box(
            p_internal->cpp->get_box(int(index)),
            p_pokemon_box_out
        );
    )
}

enum pkmn_error pkmn_pokemon_pc_get_box_names(
    const struct pkmn_pokemon_pc* p_pokemon_pc,
    struct pkmn_string_list* p_box_names_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_pc);
    pkmn::c::pokemon_pc_internal_t* p_internal = POKEMON_PC_INTERNAL_RCAST(p_pokemon_pc->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_box_names_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_list_cpp_to_c(
            p_internal->cpp->get_box_names(),
            p_box_names_out
        );
    )
}

enum pkmn_error pkmn_pokemon_pc_as_list(
    const struct pkmn_pokemon_pc* p_pokemon_pc,
    struct pkmn_pokemon_box_list* p_pokemon_box_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_pc);
    pkmn::c::pokemon_pc_internal_t* p_internal = POKEMON_PC_INTERNAL_RCAST(p_pokemon_pc->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_pokemon_box_list_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::pokemon_box_list_cpp_to_c(
            p_internal->cpp->as_vector(),
            p_pokemon_box_list_out
        );
    )
}
