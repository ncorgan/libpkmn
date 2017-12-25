/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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

static inline pkmn_error_t pkmn_levelup_move_free(
    pkmn_levelup_move_t* levelup_move
)
{
    if(!levelup_move)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    free(levelup_move->move);
    levelup_move->move = NULL;
    levelup_move->level = 0;

    return PKMN_ERROR_NONE;
}

static inline pkmn_error_t pkmn_levelup_moves_free(
    pkmn_levelup_moves_t* levelup_moves
)
{
    if(!levelup_moves)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    if(levelup_moves->length > 0)
    {
        for(size_t index = 0; index < levelup_moves->length; ++index)
        {
            free(levelup_moves->levelup_moves[index].move);
        }
        free(levelup_moves->levelup_moves);
    }

    levelup_moves->levelup_moves = NULL;
    levelup_moves->length = 0;

    return PKMN_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_LEVELUP_MOVE_H */
