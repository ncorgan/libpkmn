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

struct pkmn_levelup_move
{
    char* p_move;
    int level;
};

struct pkmn_levelup_moves
{
    struct pkmn_levelup_move* p_levelup_moves;
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_levelup_move_free(
    struct pkmn_levelup_move* p_levelup_move
);

PKMN_C_API enum pkmn_error pkmn_levelup_moves_free(
    struct pkmn_levelup_moves* p_levelup_moves
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_LEVELUP_MOVE_H */
