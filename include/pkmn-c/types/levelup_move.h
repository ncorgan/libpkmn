/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_LEVELUP_MOVE_H
#define PKMN_C_TYPES_LEVELUP_MOVE_H

#include <pkmn-c/config.h>

#include <stdlib.h>

typedef struct {
    char* move;
    int level;
} pkmn_levelup_move_t;

typedef pkmn_levelup_move_t* pkmn_levelup_moves_t;

#ifdef __cplusplus
extern "C" {
#endif

static PKMN_INLINE void pkmn_levelup_move_free(
    pkmn_levelup_move_t* levelup_move
) {
    free(levelup_move->move);
}

static PKMN_INLINE void pkmn_levelup_moves_free(
    pkmn_levelup_moves_t* levelup_moves,
    size_t list_length
) {
    for(size_t i = 0; i < list_length; ++i) {
        free((*levelup_moves)[i].move);
    }

    free(*levelup_moves);
    *levelup_moves = NULL;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_LEVELUP_MOVE_H */
