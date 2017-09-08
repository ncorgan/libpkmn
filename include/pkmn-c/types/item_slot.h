/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_ITEM_SLOT_H
#define PKMN_C_TYPES_ITEM_SLOT_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

typedef struct {
    char* item;
    int amount;
} pkmn_item_slot_t;

typedef struct {
    pkmn_item_slot_t* item_slots;
    size_t length;
} pkmn_item_slots_t;

#ifdef __cplusplus
extern "C" {
#endif

static PKMN_INLINE pkmn_error_t pkmn_item_slot_free(
    pkmn_item_slot_t* item_slot
) {
    free(item_slot->item);
    item_slot->item = NULL;
    item_slot->amount = 0;

    return PKMN_ERROR_NONE;
}

static PKMN_INLINE pkmn_error_t pkmn_item_slots_free(
    pkmn_item_slots_t* item_slots
) {
    for(size_t i = 0; i < item_slots->length; ++i) {
        free(item_slots->item_slots[i].item);
    }

    free(item_slots->item_slots);
    item_slots->item_slots = NULL;
    item_slots->length = 0;

    return PKMN_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_ITEM_SLOT_H */
