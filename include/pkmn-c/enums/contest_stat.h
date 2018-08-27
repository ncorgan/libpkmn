/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUMS_CONTEST_STAT_H
#define PKMN_C_ENUMS_CONTEST_STAT_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

enum pkmn_contest_stat
{
    PKMN_CONTEST_STAT_NONE = 0,
    PKMN_CONTEST_STAT_COOL = 1,
    PKMN_CONTEST_STAT_BEAUTY = 2,
    PKMN_CONTEST_STAT_CUTE = 3,
    PKMN_CONTEST_STAT_SMART = 4,
    PKMN_CONTEST_STAT_TOUGH = 5,
    PKMN_CONTEST_STAT_FEEL = 6,
    PKMN_CONTEST_STAT_SHEEN = 7,
};

struct pkmn_contest_stat_enum_list
{
    enum pkmn_contest_stat* p_enums;
    size_t length;
};

struct pkmn_contest_stat_enum_map
{
    int* p_values;
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_contest_stat_enum_list_free(
    struct pkmn_contest_stat_enum_list* p_contest_stat_enum_list
);

PKMN_C_API enum pkmn_error pkmn_contest_stat_enum_map_free(
    struct pkmn_contest_stat_enum_map* p_contest_stat_enum_map
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ENUMS_CONTEST_STAT_H */
