/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/pokemon_pc.h>

pkmn_error_t pkmn_pokemon_pc_make(
    pkmn_pokemon_pc_handle_t* handle_ptr,
    const char* game
) {
    PKMN_CHECK_NULL_PARAM(handle_ptr);
    PKMN_CHECK_NULL_PARAM(game);

    PKMN_CPP_TO_C(
        pkmn::pokemon_pc::sptr cpp = pkmn::pokemon_pc::make(game);

        (*handle_ptr) = new pkmn_pokemon_pc_t;
        (*handle_ptr)->cpp = cpp;
        (*handle_ptr)->last_error = "None";
    )
}

pkmn_error_t pkmn_pokemon_pc_free(
    pkmn_pokemon_pc_handle_t* handle_ptr
) {
    PKMN_CHECK_NULL_PARAM(handle_ptr);
    PKMN_CHECK_NULL_PARAM((*handle_ptr));

    PKMN_CPP_TO_C(
        delete (*handle_ptr);
        *handle_ptr = NULL;
    )
}

const char* pkmn_pokemon_pc_strerror(
    pkmn_pokemon_pc_handle_t handle
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

pkmn_error_t pkmn_pokemon_pc_get_game(
    pkmn_pokemon_pc_handle_t handle,
    char* game_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(game_out, handle);

    PKMN_CPP_TO_C(
        return pkmn::std_string_to_c_str(
                   handle->cpp->get_game(),
                   game_out,
                   buffer_len
               );
    )
}

pkmn_error_t pkmn_pokemon_pc_get_num_boxes(
    pkmn_pokemon_pc_handle_t handle,
    int* num_boxes_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(num_boxes_out, handle);

    PKMN_CPP_TO_C(
        *num_boxes_out = handle->cpp->get_num_boxes();
    )
}

pkmn_error_t pkmn_pokemon_pc_get_box(
    pkmn_pokemon_pc_handle_t handle,
    int index,
    pkmn_pokemon_box_handle_t* pokemon_box_handle_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_box_handle_out, handle);

    PKMN_CPP_TO_C(
        pkmn::pokemon_box::sptr temp = handle->cpp->get_box(index);

        *pokemon_box_handle_out = new pkmn_pokemon_box_t;
        (*pokemon_box_handle_out)->cpp = temp;
        (*pokemon_box_handle_out)->last_error = "None";
    )
}

pkmn_error_t pkmn_pokemon_pc_as_array(
    pkmn_pokemon_pc_handle_t handle,
    pkmn_pokemon_box_list_t* pokemon_box_list_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_box_list_out, handle);

    PKMN_CPP_TO_C(
        pkmn::pkmn_pokemon_box_list_cpp_to_c(
            handle->cpp->as_vector(),
            pokemon_box_list_out
        );
    )
}

pkmn_error_t pkmn_pokemon_pc_get_box_names(
    pkmn_pokemon_pc_handle_t handle,
    pkmn_string_list_t* box_names_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(box_names_out, handle);

    PKMN_CPP_TO_C(
        pkmn::string_list_cpp_to_c(
            handle->cpp->get_box_names(),
            box_names_out
        );
    )
}
