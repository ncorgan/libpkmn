/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_ITEM_SLOT_H
#define PKMN_C_TYPES_ITEM_SLOT_H

#include <pkmn-c/config.h>

#include <stdlib.h>

typedef struct {
    char* item;
    int amount;
} pkmn_item_slot_t;

typedef pkmn_item_slot_t* pkmn_item_slots_t;

#ifdef __cplusplus
extern "C" {
#endif

static PKMN_INLINE void pkmn_item_slot_free(
    pkmn_item_slot_t* item_slot
) {
    free(item_slot->item);
}

static PKMN_INLINE void pkmn_item_slots_free(
    pkmn_item_slots_t* item_slots,
    size_t list_length
) {
    for(size_t i = 0; i < list_length; ++i) {
        free((*item_slots)[i].item);
    }

    free(*item_slots);
    *item_slots = NULL;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_ITEM_SLOT_H */
