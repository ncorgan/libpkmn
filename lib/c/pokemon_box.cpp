/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/pokemon_box.h>

pkmn_error_t pkmn_pokemon_box_make(
    pkmn_pokemon_box_handle_t* handle_ptr,
    const char* game
) {
    PKMN_CPP_TO_C(
        pkmn::pokemon_box::sptr cpp = pkmn::pokemon_box::make(
                                          game
                                      );

        (*handle_ptr) = new pkmn_pokemon_box_t;
        (*handle_ptr)->cpp = cpp;
        (*handle_ptr)->last_error = "None";
    )
}

pkmn_error_t pkmn_pokemon_box_free(
    pkmn_pokemon_box_handle_t* handle_ptr
) {
    PKMN_CPP_TO_C(
        delete (*handle_ptr);
        *handle_ptr = NULL;
    )
}

const char* pkmn_pokemon_box_strerror(
    pkmn_pokemon_box_handle_t handle
) {
    return handle->last_error.c_str();
}

pkmn_error_t pkmn_pokemon_box_get_name(
    pkmn_pokemon_box_handle_t handle,
    char* name_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_name(),
            name_out,
            buffer_len,
            actual_strlen_out
        )
    )
}

pkmn_error_t pkmn_pokemon_box_set_name(
    pkmn_pokemon_box_handle_t handle,
    const char* name
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_name(name);
    )
}

pkmn_error_t pkmn_pokemon_box_get_game(
    pkmn_pokemon_box_handle_t handle,
    char* game_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_game(),
            game_out,
            buffer_len,
            actual_strlen_out
        )
    )
}

pkmn_error_t pkmn_pokemon_box_get_capacity(
    pkmn_pokemon_box_handle_t handle,
    int* capacity_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *capacity_out = handle->cpp->get_capacity();
    )
}

pkmn_error_t pkmn_pokemon_box_get_pokemon(
    pkmn_pokemon_box_handle_t handle,
    int index,
    pkmn_pokemon_handle_t* pokemon_handle_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        // Catch any exceptions before allocating new memory.
        pkmn::pokemon::sptr temp = handle->cpp->get_pokemon(index);

        *pokemon_handle_out = new pkmn_pokemon_t;
        (*pokemon_handle_out)->cpp = temp;
        (*pokemon_handle_out)->last_error = "None";
    )
}

pkmn_error_t pkmn_pokemon_box_set_pokemon(
    pkmn_pokemon_box_handle_t handle,
    pkmn_pokemon_handle_t pokemon_handle,
    int index
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_pokemon(
            index,
            pokemon_handle->cpp
        );
    )
}

pkmn_error_t pkmn_pokemon_box_as_array(
    pkmn_pokemon_box_handle_t handle,
    pkmn_pokemon_list_t* pokemon_list_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::pkmn_pokemon_list_cpp_to_c(
            handle->cpp->as_vector(),
            pokemon_list_out
        );
    )
}
