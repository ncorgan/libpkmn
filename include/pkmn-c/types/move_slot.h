/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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

PKMN_C_API pkmn_error_t pkmn_move_slot_free(
    pkmn_move_slot_t* move_slot_ptr
);

PKMN_C_API pkmn_error_t pkmn_move_slots_free(
    pkmn_move_slots_t* move_slots_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_MOVE_SLOT_H */
