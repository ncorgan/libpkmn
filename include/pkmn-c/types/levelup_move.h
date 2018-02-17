/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_LEVELUP_MOVE_H
#define PKMN_C_TYPES_LEVELUP_MOVE_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

typedef struct
{
    char* move;
    int level;
} pkmn_levelup_move_t;

typedef struct
{
    pkmn_levelup_move_t* levelup_moves;
    size_t length;
} pkmn_levelup_moves_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API pkmn_error_t pkmn_levelup_move_free(
    pkmn_levelup_move_t* levelup_move_ptr
);

PKMN_C_API pkmn_error_t pkmn_levelup_moves_free(
    pkmn_levelup_moves_t* levelup_moves_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_LEVELUP_MOVE_H */
