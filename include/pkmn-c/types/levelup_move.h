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

//! A struct with information on when a Pokémon learns a given move leveling up.
typedef struct
{
    //! The move name.
    char* move;
    //! The level at which the Pokémon learns the move.
    int level;
} pkmn_levelup_move_t;

//! A list of levelup moves.
typedef struct
{
    //! The level-up moves, dynamically allocated.
    pkmn_levelup_move_t* levelup_moves;
    //! The number of level-up moves in the list.
    size_t length;
} pkmn_levelup_moves_t;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Frees the dynamically-allocated memory in the given level-up move struct.
 *
 * \param levelup_move_ptr A pointer to the level-up move struct to free
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if levelup_move_ptr is NULL
 */
PKMN_C_API pkmn_error_t pkmn_levelup_move_free(
    pkmn_levelup_move_t* levelup_move_ptr
);

/*!
 * @brief Frees the dynamically-allocated memory in the given level-up move list.
 *
 * \param levelup_moves_ptr A pointer to the level-up move list to free
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if levelup_moves_ptr is NULL
 */
PKMN_C_API pkmn_error_t pkmn_levelup_moves_free(
    pkmn_levelup_moves_t* levelup_moves_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_LEVELUP_MOVE_H */
