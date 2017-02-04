/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_MOVE_SLOT_H
#define PKMN_C_TYPES_MOVE_SLOT_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

typedef struct {
    char* move;
    int pp;
} pkmn_move_slot_t;

typedef struct {
    pkmn_move_slot_t* move_slots;
    size_t length;
} pkmn_move_slots_t;

#ifdef __cplusplus
extern "C" {
#endif

static PKMN_INLINE pkmn_error_t pkmn_move_slot_free(
    pkmn_move_slot_t* move_slot
) {
    free(move_slot->move);
    move_slot->move = NULL;
    move_slot->pp = 0;

    return PKMN_ERROR_NONE;
}

static PKMN_INLINE pkmn_error_t pkmn_move_slots_free(
    pkmn_move_slots_t* move_slots
) {
    for(size_t i = 0; i < move_slots->length; ++i) {
        free(move_slots->move_slots[i].move);
    }

    free(move_slots->move_slots);
    move_slots->move_slots = NULL;
    move_slots->length = 0;

    return PKMN_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_MOVE_SLOT_H */
