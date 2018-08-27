/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "error_internal.hpp"

#include <pkmn-c/enums/ability.h>
#include <pkmn-c/enums/ball.h>
#include <pkmn-c/enums/contest_stat.h>
#include <pkmn-c/enums/egg_group.h>
#include <pkmn-c/enums/game.h>
#include <pkmn-c/enums/item.h>
#include <pkmn-c/enums/marking.h>
#include <pkmn-c/enums/move.h>
#include <pkmn-c/enums/move_damage_class.h>
#include <pkmn-c/enums/move_target.h>
#include <pkmn-c/enums/nature.h>
#include <pkmn-c/enums/species.h>
#include <pkmn-c/enums/stat.h>
#include <pkmn-c/enums/type.h>

#include <cstdlib>

#define PKMN_C_ENUM_LIST_FREE_FCN(name) \
\
enum pkmn_error pkmn_ ## name ## _enum_list_free( \
    struct pkmn_ ## name ## _enum_list* p_ ## name ## _enum_list \
) \
{ \
    PKMN_CHECK_NULL_PARAM(p_ ## name ## _enum_list); \
\
    enum pkmn_error error = PKMN_ERROR_NONE; \
\
    if(p_ ## name ## _enum_list->length > 0) \
    { \
        std::free(p_ ## name ## _enum_list->p_enums); \
    } \
\
    p_ ## name ## _enum_list->p_enums = nullptr; \
    p_ ## name ## _enum_list->length = 0; \
\
    return error; \
}

PKMN_C_ENUM_LIST_FREE_FCN(ability)
PKMN_C_ENUM_LIST_FREE_FCN(ball)
PKMN_C_ENUM_LIST_FREE_FCN(contest_stat)
PKMN_C_ENUM_LIST_FREE_FCN(egg_group)
PKMN_C_ENUM_LIST_FREE_FCN(game)
PKMN_C_ENUM_LIST_FREE_FCN(item)
PKMN_C_ENUM_LIST_FREE_FCN(marking)
PKMN_C_ENUM_LIST_FREE_FCN(move)
PKMN_C_ENUM_LIST_FREE_FCN(move_damage_class)
PKMN_C_ENUM_LIST_FREE_FCN(move_target)
PKMN_C_ENUM_LIST_FREE_FCN(nature)
PKMN_C_ENUM_LIST_FREE_FCN(species)
PKMN_C_ENUM_LIST_FREE_FCN(stat)
PKMN_C_ENUM_LIST_FREE_FCN(type)
