/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUMS_GAME_SAVE_TYPE_H
#define PKMN_C_ENUMS_GAME_SAVE_TYPE_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

enum pkmn_game_save_type
{
    PKMN_GAME_SAVE_TYPE_NONE = 0,
    PKMN_GAME_SAVE_TYPE_RED_BLUE = 1,
    PKMN_GAME_SAVE_TYPE_YELLOW = 2,
    PKMN_GAME_SAVE_TYPE_GOLD_SILVER = 3,
    PKMN_GAME_SAVE_TYPE_CRYSTAL = 4,
    PKMN_GAME_SAVE_TYPE_RUBY_SAPPHIRE = 5,
    PKMN_GAME_SAVE_TYPE_EMERALD = 6,
    PKMN_GAME_SAVE_TYPE_FIRERED_LEAFGREEN = 7,
    PKMN_GAME_SAVE_TYPE_COLOSSEUM_XD = 8,
};

struct pkmn_game_save_type_enum_list
{
    enum pkmn_game_save_type* p_enums;
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_game_save_type_enum_list_free(
    struct pkmn_game_save_type_enum_list* p_game_save_type_enum_list
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ENUMS_GAME_SAVE_TYPE_H */
