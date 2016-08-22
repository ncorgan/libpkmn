/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/item_list.h>

pkmn_error_t pkmn_item_list_make(
    pkmn_item_list_handle_t* handle_ptr,
    const char* item_name,
    const char* game_name
) {
    PKMN_CPP_TO_C(
        pkmn::item_list::sptr cpp = pkmn::item_list::make(
                                        item_name, game_name
                                    );

        (*handle_ptr) = new pkmn_item_list_t;
        (*handle_ptr)->cpp = cpp;
        (*handle_ptr)->last_error = "None";
    )
}

pkmn_error_t pkmn_item_list_free(
    pkmn_item_list_handle_t* handle_ptr
) {
    PKMN_CPP_TO_C(
        delete (*handle_ptr);
        *handle_ptr = NULL;
    )
}

const char* pkmn_item_list_strerror(
    pkmn_item_list_handle_t handle
) {
    return handle->last_error.c_str();
}

pkmn_error_t pkmn_item_list_get_capacity(
    pkmn_item_list_handle_t handle,
    int* capacity_out
) {
    PKMN_CPP_TO_C(
        *capacity_out = handle->cpp->get_capacity();
    )
}

pkmn_error_t pkmn_item_list_get_num_items(
    pkmn_item_list_handle_t handle,
    int* num_items_out
) {
    PKMN_CPP_TO_C(
        *num_items_out = handle->cpp->get_num_items();
    )
}

pkmn_error_t pkmn_item_list_at(
    pkmn_item_list_handle_t handle,
    int position,
    pkmn_item_slot_t* item_slot_out
) {
    PKMN_CPP_TO_C(
        pkmn::pkmn_item_slot_cpp_to_c(
            handle->cpp->at(position),
            item_slot_out
        );
    )
}

pkmn_error_t pkmn_item_list_add(
    pkmn_item_list_handle_t handle,
    const char* name,
    int amount
) {
    PKMN_CPP_TO_C(
        handle->cpp->add(
            name, amount
        );
    )
}

pkmn_error_t pkmn_item_list_remove(
    pkmn_item_list_handle_t handle,
    const char* name,
    int amount
) {
    PKMN_CPP_TO_C(
        handle->cpp->remove(
            name, amount
        );
    )
}

pkmn_error_t pkmn_item_list_move(
    pkmn_item_list_handle_t handle,
    int old_position,
    int new_position
) {
    PKMN_CPP_TO_C(
        handle->cpp->move(
            old_position,
            new_position
        );
    )
}

pkmn_error_t pkmn_item_list_as_array(
    pkmn_item_list_handle_t handle,
    pkmn_item_slots_t* array_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::pkmn_item_slots_cpp_to_c(
            handle->cpp->as_vector(),
            array_out,
            list_length_out
        );
    )
}
