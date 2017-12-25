/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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

    size_t num_items;
    size_t capacity;
    pkmn_item_slots_t item_slots;

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
    pkmn_item_list_t* item_list
);

PKMN_C_API const char* pkmn_item_list_strerror(
    pkmn_item_list_t* item_list
);

PKMN_C_API pkmn_error_t pkmn_item_list_add(
    pkmn_item_list_t* item_list,
    const char* item,
    int amount
);

PKMN_C_API pkmn_error_t pkmn_item_list_remove(
    pkmn_item_list_t* item_list,
    const char* item,
    int amount
);

PKMN_C_API pkmn_error_t pkmn_item_list_move(
    pkmn_item_list_t* item_list,
    int old_position,
    int new_position
);

PKMN_C_API pkmn_error_t pkmn_item_list_set_item(
    pkmn_item_list_t* item_list,
    int position,
    const char* item,
    int amount
);

PKMN_C_API pkmn_error_t pkmn_item_list_get_valid_items(
    pkmn_item_list_t* item_list,
    pkmn_string_list_t* valid_items_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ITEM_LIST_H */
