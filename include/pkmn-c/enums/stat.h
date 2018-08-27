/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUMS_STAT_H
#define PKMN_C_ENUMS_STAT_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

enum pkmn_stat
{
    PKMN_STAT_NONE = 0,
    PKMN_STAT_HP = 1,
    PKMN_STAT_ATTACK = 2,
    PKMN_STAT_DEFENSE = 3,
    PKMN_STAT_SPEED = 4,
    PKMN_STAT_SPECIAL = 5,
    PKMN_STAT_SPECIAL_ATTACK = 6,
    PKMN_STAT_SPECIAL_DEFENSE = 7,
};

struct pkmn_stat_enum_list
{
    enum pkmn_stat* p_enums;
    size_t length;
};

struct pkmn_stat_enum_map
{
    int* p_values;
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_stat_enum_list_free(
    struct pkmn_stat_enum_list* p_stat_enum_list
);

PKMN_C_API enum pkmn_error pkmn_stat_enum_map_free(
    struct pkmn_stat_enum_map* p_stat_enum_map
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ENUMS_STAT_H */
