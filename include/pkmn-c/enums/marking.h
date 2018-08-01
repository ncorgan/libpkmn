/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUMS_MARKING_H
#define PKMN_C_ENUMS_MARKING_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

enum pkmn_marking
{
    PKMN_MARKING_NONE = 0,
    PKMN_MARKING_CIRCLE = 1,
    PKMN_MARKING_TRIANGLE = 2,
    PKMN_MARKING_SQUARE = 3,
    PKMN_MARKING_HEART = 4,
    PKMN_MARKING_STAR = 5,
    PKMN_MARKING_DIAMOND = 6,
};

struct pkmn_marking_enum_list
{
    enum pkmn_marking* p_enums;
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_marking_enum_list_free(
    struct pkmn_marking_enum_list* p_marking_enum_list
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ENUMS_MARKING_H */
