/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUMS_EGG_GROUP_H
#define PKMN_C_ENUMS_EGG_GROUP_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

enum pkmn_egg_group
{
    PKMN_EGG_GROUP_NONE = 0,
    PKMN_EGG_GROUP_MONSTER = 1,
    PKMN_EGG_GROUP_WATER1 = 2,
    PKMN_EGG_GROUP_BUG = 3,
    PKMN_EGG_GROUP_FLYING = 4,
    PKMN_EGG_GROUP_GROUND = 5,
    PKMN_EGG_GROUP_FAIRY = 6,
    PKMN_EGG_GROUP_PLANT = 7,
    PKMN_EGG_GROUP_HUMANSHAPE = 8,
    PKMN_EGG_GROUP_WATER3 = 9,
    PKMN_EGG_GROUP_MINERAL = 10,
    PKMN_EGG_GROUP_INDETERMINATE = 11,
    PKMN_EGG_GROUP_WATER2 = 12,
    PKMN_EGG_GROUP_DITTO = 13,
    PKMN_EGG_GROUP_DRAGON = 14,
    PKMN_EGG_GROUP_UNDISCOVERED = 15,
};

struct pkmn_egg_group_enum_list
{
    enum pkmn_egg_group* p_enums;
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_egg_group_enum_list_free(
    struct pkmn_egg_group_enum_list* p_egg_group_enum_list
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ENUMS_EGG_GROUP_H */
