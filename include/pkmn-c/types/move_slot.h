/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_MOVE_SLOT_H
#define PKMN_C_TYPES_MOVE_SLOT_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

typedef struct
{
    char* move;
    int pp;
} pkmn_move_slot_t;

typedef struct
{
    pkmn_move_slot_t* move_slots;
    size_t length;
} pkmn_move_slots_t;

#ifdef __cplusplus
extern "C" {
#endif

static inline pkmn_error_t pkmn_move_slot_free(
    pkmn_move_slot_t* move_slot_ptr
)
{
    if(!move_slot_ptr)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    free(move_slot_ptr->move);
    move_slot_ptr->move = NULL;
    move_slot_ptr->pp = 0;

    return PKMN_ERROR_NONE;
}

static inline pkmn_error_t pkmn_move_slots_free(
    pkmn_move_slots_t* move_slots_ptr
)
{
    if(!move_slots_ptr)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    if(move_slots_ptr->length > 0)
    {
        for(size_t index = 0; index < move_slots_ptr->length; ++index)
        {
            pkmn_move_slot_free(&move_slots_ptr->move_slots[index]);
        }
        free(move_slots_ptr->move_slots);
    }

    move_slots_ptr->move_slots = NULL;
    move_slots_ptr->length = 0;

    return PKMN_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_MOVE_SLOT_H */
