/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUMS_NATURE_H
#define PKMN_C_ENUMS_NATURE_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

enum pkmn_nature
{
    PKMN_NATURE_NONE = 0,
    PKMN_NATURE_HARDY = 1,
    PKMN_NATURE_LONELY = 2,
    PKMN_NATURE_BRAVE = 3,
    PKMN_NATURE_ADAMANT = 4,
    PKMN_NATURE_NAUGHTY = 5,
    PKMN_NATURE_BOLD = 6,
    PKMN_NATURE_DOCILE = 7,
    PKMN_NATURE_RELAXED = 8,
    PKMN_NATURE_IMPISH = 9,
    PKMN_NATURE_LAX = 10,
    PKMN_NATURE_TIMID = 11,
    PKMN_NATURE_HASTY = 12,
    PKMN_NATURE_SERIOUS = 13,
    PKMN_NATURE_JOLLY = 14,
    PKMN_NATURE_NAIVE = 15,
    PKMN_NATURE_MODEST = 16,
    PKMN_NATURE_MILD = 17,
    PKMN_NATURE_QUIET = 18,
    PKMN_NATURE_BASHFUL = 19,
    PKMN_NATURE_RASH = 20,
    PKMN_NATURE_CALM = 21,
    PKMN_NATURE_GENTLE = 22,
    PKMN_NATURE_SASSY = 23,
    PKMN_NATURE_CAREFUL = 24,
    PKMN_NATURE_QUIRKY = 25,
};

struct pkmn_nature_enum_list
{
    enum pkmn_nature* p_enums;
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_nature_enum_list_free(
    struct pkmn_nature_enum_list* p_nature_enum_list
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ENUMS_NATURE_H */
