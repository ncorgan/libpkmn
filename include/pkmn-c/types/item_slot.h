/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_ITEM_SLOT_H
#define PKMN_C_TYPES_ITEM_SLOT_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

typedef struct
{
    char* item;
    size_t amount;
} pkmn_item_slot_t;

typedef struct
{
    pkmn_item_slot_t* item_slots;
    size_t length;
} pkmn_item_slots_t;

#ifdef __cplusplus
extern "C" {
#endif

static inline pkmn_error_t pkmn_item_slot_free(
    pkmn_item_slot_t* item_slot_ptr
)
{
    if(!item_slot_ptr)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    free(item_slot_ptr->item);
    item_slot_ptr->item = NULL;
    item_slot_ptr->amount = 0;

    return PKMN_ERROR_NONE;
}

static inline pkmn_error_t pkmn_item_slots_free(
    pkmn_item_slots_t* item_slots_ptr
)
{
    if(!item_slots_ptr)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    if(item_slots_ptr->length > 0)
    {
        for(size_t index = 0; index < item_slots_ptr->length; ++index)
        {
            pkmn_item_slot_free(&item_slots_ptr->item_slots[index]);
        }
        free(item_slots_ptr->item_slots);
    }

    item_slots_ptr->item_slots = NULL;
    item_slots_ptr->length = 0;

    return PKMN_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_ITEM_SLOT_H */
