/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "error_internal.hpp"

#include <pkmn-c/enums/contest_stat.h>
#include <pkmn-c/enums/marking.h>
#include <pkmn-c/enums/stat.h>

#include <cstdlib>

#define PKMN_C_ENUM_MAP_FREE_FCN(name) \
\
enum pkmn_error pkmn_ ## name ## _enum_map_free( \
    struct pkmn_ ## name ## _enum_map* p_ ## name ## _enum_map \
) \
{ \
    PKMN_CHECK_NULL_PARAM(p_ ## name ## _enum_map); \
\
    enum pkmn_error error = PKMN_ERROR_NONE; \
\
    if(p_ ## name ## _enum_map->length > 0) \
    { \
        std::free(p_ ## name ## _enum_map->p_values); \
    } \
\
    p_ ## name ## _enum_map->p_values = nullptr; \
    p_ ## name ## _enum_map->length = 0; \
\
    return error; \
}

PKMN_C_ENUM_MAP_FREE_FCN(contest_stat)
PKMN_C_ENUM_MAP_FREE_FCN(marking)
PKMN_C_ENUM_MAP_FREE_FCN(stat)
