/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_MOVE_SLOT_H
#define PKMN_C_TYPES_MOVE_SLOT_H

#include <pkmn-c/config.h>

#include <stdlib.h>

typedef struct {
    char* move;
    int pp;
} pkmn_move_slot_t;

typedef pkmn_move_slot_t* pkmn_move_slots_t;

#ifdef __cplusplus
extern "C" {
#endif

static PKMN_INLINE void pkmn_move_slot_free(
    pkmn_move_slot_t* move_slot
) {
    free(move_slot->move);
}

static PKMN_INLINE void pkmn_move_slots_free(
    pkmn_move_slots_t* move_slots,
    size_t list_length
) {
    for(size_t i = 0; i < list_length; ++i) {
        free((*move_slots)[i].move);
    }

    free(*move_slots);
    *move_slots = NULL;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_MOVE_SLOT_H */
