/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUMS_MOVE_TARGET_H
#define PKMN_C_ENUMS_MOVE_TARGET_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

enum pkmn_move_target
{
    PKMN_MOVE_TARGET_NONE = 0,
    PKMN_MOVE_TARGET_SPECIFIC_MOVE = 1,
    PKMN_MOVE_TARGET_SELECTED_POKEMON_ME_FIRST = 2,
    PKMN_MOVE_TARGET_ALLY = 3,
    PKMN_MOVE_TARGET_USERS_FIELD = 4,
    PKMN_MOVE_TARGET_USER_OR_ALLY = 5,
    PKMN_MOVE_TARGET_OPPONENTS_FIELD = 6,
    PKMN_MOVE_TARGET_USER = 7,
    PKMN_MOVE_TARGET_RANDOM_OPPONENT = 8,
    PKMN_MOVE_TARGET_ALL_OTHER_POKEMON = 9,
    PKMN_MOVE_TARGET_SELECTED_POKEMON = 10,
    PKMN_MOVE_TARGET_ALL_OPPONENTS = 11,
    PKMN_MOVE_TARGET_ENTIRE_FIELD = 12,
    PKMN_MOVE_TARGET_USER_AND_ALLIES = 13,
    PKMN_MOVE_TARGET_ALL_POKEMON = 14,
};

struct pkmn_move_target_enum_list
{
    enum pkmn_move_target* p_enums;
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_move_target_enum_list_free(
    struct pkmn_move_target_enum_list* p_move_target_enum_list
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ENUMS_MOVE_TARGET_H */
