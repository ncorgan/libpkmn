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

void pkmn_item_list_free(
    pkmn_item_list_handle_t* handle_ptr
) {
    delete (*handle_ptr);
    *handle_ptr = NULL;
}

const char* pkmn_item_list_strerror(
    pkmn_item_list_handle_t handle
) {
    return handle->last_error.c_str();
}
