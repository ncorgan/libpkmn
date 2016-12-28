/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_ITEM_LIST_H
#define PKMN_C_ITEM_LIST_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/types/item_slot.h>
#include <pkmn-c/types/string_types.h>

#if !defined(PKMN_ITEM_LIST_DECLARED) && !defined(__DOXYGEN__)
struct pkmn_item_list_t;
typedef struct pkmn_item_list_t pkmn_item_list_t;
#define PKMN_ITEM_LIST_DECLARED
#endif

typedef pkmn_item_list_t* pkmn_item_list_handle_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_item_list_make(
    pkmn_item_list_handle_t* handle_ptr,
    const char* item_list_name,
    const char* game_name
);

PKMN_API pkmn_error_t pkmn_item_list_free(
    pkmn_item_list_handle_t* handle_ptr
);

PKMN_API const char* pkmn_item_list_strerror(
    pkmn_item_list_handle_t handle
);

PKMN_API pkmn_error_t pkmn_item_list_get_name(
    pkmn_item_list_handle_t handle,
    char* name_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_item_list_get_game(
    pkmn_item_list_handle_t handle,
    char* game_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_item_list_get_capacity(
    pkmn_item_list_handle_t handle,
    int* capacity_out
);

PKMN_API pkmn_error_t pkmn_item_list_get_num_items(
    pkmn_item_list_handle_t handle,
    int* num_items_out
);

PKMN_API pkmn_error_t pkmn_item_list_at(
    pkmn_item_list_handle_t handle,
    int position,
    pkmn_item_slot_t* item_slot_out
);

PKMN_API pkmn_error_t pkmn_item_list_add(
    pkmn_item_list_handle_t handle,
    const char* item_name,
    int amount
);

PKMN_API pkmn_error_t pkmn_item_list_remove(
    pkmn_item_list_handle_t handle,
    const char* item_name,
    int amount
);

PKMN_API pkmn_error_t pkmn_item_list_move(
    pkmn_item_list_handle_t handle,
    int old_position,
    int new_position
);

PKMN_API pkmn_error_t pkmn_item_list_get_valid_items(
    pkmn_item_list_handle_t handle,
    pkmn_string_list_t* string_list_out
);

PKMN_API pkmn_error_t pkmn_item_list_as_array(
    pkmn_item_list_handle_t handle,
    pkmn_item_slots_t* array_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ITEM_LIST_H */
