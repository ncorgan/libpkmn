/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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

typedef struct
{
    char* name;
    char* game;
    size_t capacity;

    void* _internal;
} pkmn_item_list_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API pkmn_error_t pkmn_item_list_init(
    const char* name,
    const char* game,
    pkmn_item_list_t* item_list_out
);

PKMN_C_API pkmn_error_t pkmn_item_list_free(
    pkmn_item_list_t* item_list_ptr
);

PKMN_C_API const char* pkmn_item_list_strerror(
    pkmn_item_list_t* item_list_ptr
);

PKMN_C_API pkmn_error_t pkmn_item_list_get_num_items(
    pkmn_item_list_t* item_list_ptr,
    size_t* num_items_out
);

PKMN_C_API pkmn_error_t pkmn_item_list_at(
    pkmn_item_list_t* item_list_ptr,
    size_t position,
    struct pkmn_item_slot* item_slot_out
);

PKMN_C_API pkmn_error_t pkmn_item_list_add(
    pkmn_item_list_t* item_list_ptr,
    const char* item,
    size_t amount
);

PKMN_C_API pkmn_error_t pkmn_item_list_remove(
    pkmn_item_list_t* item_list_ptr,
    const char* item,
    size_t amount
);

PKMN_C_API pkmn_error_t pkmn_item_list_move(
    pkmn_item_list_t* item_list_ptr,
    size_t old_position,
    size_t new_position
);

PKMN_C_API pkmn_error_t pkmn_item_list_set_item(
    pkmn_item_list_t* item_list_ptr,
    size_t position,
    const char* item,
    size_t amount
);

PKMN_C_API pkmn_error_t pkmn_item_list_get_valid_items(
    pkmn_item_list_t* item_list_ptr,
    struct pkmn_string_list* valid_items_out
);

PKMN_C_API pkmn_error_t pkmn_item_list_as_list(
    pkmn_item_list_t* item_list_ptr,
    struct pkmn_item_slots* item_slots_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ITEM_LIST_H */
