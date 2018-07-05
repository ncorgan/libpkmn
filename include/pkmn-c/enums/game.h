/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_ENUMS_GAME_H
#define PKMN_C_ENUMS_GAME_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

enum pkmn_game
{
    PKMN_GAME_NONE = 0,

    PKMN_GAME_RED,
    PKMN_GAME_BLUE,
    PKMN_GAME_YELLOW,

    PKMN_GAME_GOLD,
    PKMN_GAME_SILVER,
    PKMN_GAME_CRYSTAL,

    PKMN_GAME_RUBY,
    PKMN_GAME_SAPPHIRE,
    PKMN_GAME_EMERALD,
    PKMN_GAME_FIRERED,
    PKMN_GAME_LEAFGREEN,

    PKMN_GAME_COLOSSEUM,
    PKMN_GAME_XD,

    PKMN_GAME_DIAMOND,
    PKMN_GAME_PEARL,
    PKMN_GAME_PLATINUM,

    PKMN_GAME_HEARTGOLD,
    PKMN_GAME_SOULSILVER,

    PKMN_GAME_BLACK,
    PKMN_GAME_WHITE,
    PKMN_GAME_BLACK2,
    PKMN_GAME_WHITE2,

    PKMN_GAME_X,
    PKMN_GAME_Y,
    PKMN_GAME_OMEGA_RUBY,
    PKMN_GAME_ALPHA_SAPPHIRE
};

struct pkmn_game_enum_list
{
    enum pkmn_game* p_enums;
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_game_enum_list_free(
    struct pkmn_game_enum_list* p_game_enum_list
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ENUMS_GAME_H */
