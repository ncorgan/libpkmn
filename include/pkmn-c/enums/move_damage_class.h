/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUMS_MOVE_DAMAGE_CLASS_H
#define PKMN_C_ENUMS_MOVE_DAMAGE_CLASS_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

enum pkmn_move_damage_class
{
    PKMN_MOVE_DAMAGE_CLASS_NONE = 0,
    PKMN_MOVE_DAMAGE_CLASS_STATUS = 1,
    PKMN_MOVE_DAMAGE_CLASS_PHYSICAL = 2,
    PKMN_MOVE_DAMAGE_CLASS_SPECIAL = 3,
};

struct pkmn_move_damage_class_enum_list
{
    enum pkmn_move_damage_class* p_enums;
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_move_damage_class_enum_list_free(
    struct pkmn_move_damage_class_enum_list* p_move_damage_class_enum_list
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ENUMS_MOVE_DAMAGE_CLASS_H */
