/*
 * p_Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_ITEM_LIST_H
#define PKMN_C_ITEM_LIST_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/types/item_slot.h>
#include <pkmn-c/types/string_types.h>

struct pkmn_item_list
{
    char* p_name;
    char* p_game;
    size_t capacity;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_item_list_init(
    const char* p_name,
    const char* p_game,
    struct pkmn_item_list* p_item_list_out
);

PKMN_C_API enum pkmn_error pkmn_item_list_free(
    struct pkmn_item_list* p_item_list
);

PKMN_C_API const char* pkmn_item_list_strerror(
    struct pkmn_item_list* p_item_list
);

PKMN_C_API enum pkmn_error pkmn_item_list_get_num_items(
    struct pkmn_item_list* p_item_list,
    size_t* p_num_items_out
);

PKMN_C_API enum pkmn_error pkmn_item_list_at(
    struct pkmn_item_list* p_item_list,
    size_t position,
    struct pkmn_item_slot* p_item_slot_out
);

PKMN_C_API enum pkmn_error pkmn_item_list_add(
    struct pkmn_item_list* p_item_list,
    const char* p_item,
    size_t amount
);

PKMN_C_API enum pkmn_error pkmn_item_list_remove(
    struct pkmn_item_list* p_item_list,
    const char* p_item,
    size_t amount
);

PKMN_C_API enum pkmn_error pkmn_item_list_move(
    struct pkmn_item_list* p_item_list,
    size_t old_position,
    size_t new_position
);

PKMN_C_API enum pkmn_error pkmn_item_list_set_item(
    struct pkmn_item_list* p_item_list,
    size_t position,
    const char* p_item,
    size_t amount
);

PKMN_C_API enum pkmn_error pkmn_item_list_get_valid_items(
    struct pkmn_item_list* p_item_list,
    struct pkmn_string_list* p_valid_items_out
);

PKMN_C_API enum pkmn_error pkmn_item_list_as_list(
    struct pkmn_item_list* p_item_list,
    struct pkmn_item_slots* p_item_slots_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ITEM_LIST_H */
