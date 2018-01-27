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

#include <pkmn-c/pokemon_pc2.h>

pkmn_error_t pkmn_pokemon_pc2_init(
    const char* game,
    pkmn_pokemon_pc2_t* pokemon_pc_out
)
{
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(pokemon_pc_out);

    PKMN_CPP_TO_C(
        pkmn::pokemon_pc::sptr cpp = pkmn::pokemon_pc::make(game);

        pkmn::c::init_pokemon_pc(
            cpp,
            pokemon_pc_out
        );
    )
}

pkmn_error_t pkmn_pokemon_pc2_free(
    pkmn_pokemon_pc2_t* pokemon_pc_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_pc_ptr);

    pkmn::c::free_pointer_and_set_to_null(&pokemon_pc_ptr->game);
    pokemon_pc_ptr->capacity = 0;

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_pokemon_pc_internal_t**>(&pokemon_pc_ptr->_internal)
        );
    )
}

const char* pkmn_pokemon_pc2_strerror(
    pkmn_pokemon_pc2_t* pokemon_pc_ptr
)
{
    if(!pokemon_pc_ptr)
    {
        return nullptr;
    }

    try
    {
        pkmn_pokemon_pc_internal_t* internal_ptr = POKEMON_PC_INTERNAL_RCAST(pokemon_pc_ptr->_internal);
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

pkmn_error_t pkmn_pokemon_box2_get_box(
    pkmn_pokemon_box2_t* pokemon_pc_ptr,
    size_t index,
    pkmn_pokemon_box2_t* pokemon_box_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_pc_ptr);
    pkmn_pokemon_pc_internal_t* internal_ptr = POKEMON_PC_INTERNAL_RCAST(pokemon_pc_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_box_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::init_pokemon_box(
            internal_ptr->cpp->get_box(int(index)),
            pokemon_box_out
        );
    )
}

pkmn_error_t pkmn_pokemon_pc2_as_list(
    pkmn_pokemon_pc2_t* pokemon_pc_ptr,
    pkmn_pokemon_box_list2_t* pokemon_box_list_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_pc_ptr);
    pkmn_pokemon_pc_internal_t* internal_ptr = POKEMON_PC_INTERNAL_RCAST(pokemon_pc_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_box_list_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::pokemon_box_list2_cpp_to_c(
            internal_ptr->cpp->as_vector(),
            pokemon_box_list_out
        );
    )
}
