/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/item_bag.h>

pkmn_error_t pkmn_item_bag_make(
    pkmn_item_bag_handle_t* handle_ptr,
    const char* game_name
) {
    PKMN_CPP_TO_C(
        pkmn::item_bag::sptr cpp = pkmn::item_bag::make(
                                       game_name
                                   );

        (*handle_ptr) = new pkmn_item_bag_t;
        (*handle_ptr)->cpp = cpp;
        (*handle_ptr)->last_error = "None";
    )
}

pkmn_error_t pkmn_item_bag_free(
    pkmn_item_bag_handle_t* handle_ptr
) {
    PKMN_CPP_TO_C(
        delete (*handle_ptr);
        *handle_ptr = NULL;
    )
}

const char* pkmn_item_bag_strerror(
    pkmn_item_bag_handle_t handle
) {
    return handle->last_error.c_str();
}

pkmn_error_t pkmn_item_bag_get_game(
    pkmn_item_bag_handle_t handle,
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
        );
    )
}

pkmn_error_t pkmn_item_bag_get_pocket(
    pkmn_item_bag_handle_t handle,
    const char* name,
    pkmn_item_list_handle_t* item_list_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        (*item_list_out) = new pkmn_item_list_t;
        (*item_list_out)->cpp = handle->cpp->get_pocket(name);
        (*item_list_out)->last_error = "None";
    )
}

pkmn_error_t pkmn_item_bag_get_pocket_names(
    pkmn_item_bag_handle_t handle,
    pkmn_string_list_t* pocket_names_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_vector_std_string_to_string_list(
            handle->cpp->get_pocket_names(),
            pocket_names_out,
            list_length_out
        );
    )
}

pkmn_error_t pkmn_item_bag_add(
    pkmn_item_bag_handle_t handle,
    const char* name,
    int amount
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->add(
            name, amount
        );
    )
}

pkmn_error_t pkmn_item_bag_remove(
    pkmn_item_bag_handle_t handle,
    const char* name,
    int amount
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->remove(
            name, amount
        );
    )
}
