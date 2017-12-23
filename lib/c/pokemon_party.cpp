/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/pokemon_party.h>

pkmn_error_t pkmn_pokemon_party_make(
    pkmn_pokemon_party_handle_t* handle_ptr,
    const char* game
) {
    PKMN_CHECK_NULL_PARAM(handle_ptr);
    PKMN_CHECK_NULL_PARAM(game);

    PKMN_CPP_TO_C(
        pkmn::pokemon_party::sptr cpp = pkmn::pokemon_party::make(
                                          game
                                      );

        (*handle_ptr) = new pkmn_pokemon_party_t;
        (*handle_ptr)->cpp = cpp;
        (*handle_ptr)->last_error = "None";
    )
}

pkmn_error_t pkmn_pokemon_party_free(
    pkmn_pokemon_party_handle_t* handle_ptr
)
{
    PKMN_CHECK_NULL_PARAM(handle_ptr);
    PKMN_CHECK_NULL_PARAM((*handle_ptr));

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(handle_ptr);
    )
}

const char* pkmn_pokemon_party_strerror(
    pkmn_pokemon_party_handle_t handle
) {
    if(!handle) {
        return NULL;
    }

    try {
        boost::mutex::scoped_lock lock(handle->error_mutex);
        return handle->last_error.c_str();
    } catch(...) {
        return NULL;
    }
}

pkmn_error_t pkmn_pokemon_party_get_game(
    pkmn_pokemon_party_handle_t handle,
    char* game_out,
    size_t buffer_len,
    size_t* game_length_out
)
{
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(game_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::c::string_cpp_to_c(
            handle->cpp->get_game(),
            game_out,
            buffer_len,
            game_length_out
        );
    )
}

pkmn_error_t pkmn_pokemon_party_get_num_pokemon(
    pkmn_pokemon_party_handle_t handle,
    int* num_pokemon_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(num_pokemon_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *num_pokemon_out = handle->cpp->get_num_pokemon();
    )
}

pkmn_error_t pkmn_pokemon_party_get_pokemon(
    pkmn_pokemon_party_handle_t handle,
    int index,
    pkmn_pokemon_handle_t* pokemon_handle_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_handle_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        // Catch any exceptions before allocating new memory.
        pkmn::pokemon::sptr temp = handle->cpp->get_pokemon(index);

        *pokemon_handle_out = new pkmn_pokemon_t;
        (*pokemon_handle_out)->cpp = temp;
        (*pokemon_handle_out)->last_error = "None";
    )
}

pkmn_error_t pkmn_pokemon_party_set_pokemon(
    pkmn_pokemon_party_handle_t handle,
    int index,
    pkmn_pokemon_handle_t pokemon_handle
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_handle, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_pokemon(
            index,
            pokemon_handle->cpp
        );
    )
}

pkmn_error_t pkmn_pokemon_party_as_array(
    pkmn_pokemon_party_handle_t handle,
    pkmn_pokemon_list_t* pokemon_list_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_list_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::pkmn_pokemon_list_cpp_to_c(
            handle->cpp->as_vector(),
            pokemon_list_out
        );
    )
}
