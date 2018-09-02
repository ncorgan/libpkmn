/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMNTEST_C_UTIL_H
#define PKMNTEST_C_UTIL_H

#include <pkmntest-c/config.h>

#include <pkmn-c/enums/game.h>

#include <pkmn-c/types/string_types.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline bool file_exists(
    const char* filepath
)
{
    FILE* file = fopen(filepath, "rb");
    if(file)
    {
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}

static inline bool string_list_contains(
    struct pkmn_string_list* string_list,
    const char* string
)
{
    for(size_t i = 0; i < string_list->length; ++i)
    {
        if(!strcmp(string_list->pp_strings[i], string))
        {
            return true;
        }
    }

    return false;
}

static inline int game_to_generation(enum pkmn_game game)
{
    int ret = 0;

    switch(game)
    {
        case PKMN_GAME_RED:
        case PKMN_GAME_BLUE:
        case PKMN_GAME_YELLOW:
            ret = 1;
            break;

        case PKMN_GAME_GOLD:
        case PKMN_GAME_SILVER:
        case PKMN_GAME_CRYSTAL:
            ret = 2;
            break;

        case PKMN_GAME_RUBY:
        case PKMN_GAME_SAPPHIRE:
        case PKMN_GAME_EMERALD:
        case PKMN_GAME_FIRERED:
        case PKMN_GAME_LEAFGREEN:
        case PKMN_GAME_COLOSSEUM:
        case PKMN_GAME_XD:
            ret = 3;
            break;

        case PKMN_GAME_DIAMOND:
        case PKMN_GAME_PEARL:
        case PKMN_GAME_PLATINUM:
        case PKMN_GAME_HEARTGOLD:
        case PKMN_GAME_SOULSILVER:
            ret = 4;
            break;

        case PKMN_GAME_BLACK:
        case PKMN_GAME_WHITE:
        case PKMN_GAME_BLACK2:
        case PKMN_GAME_WHITE2:
            ret = 5;
            break;

        case PKMN_GAME_X:
        case PKMN_GAME_Y:
        case PKMN_GAME_OMEGA_RUBY:
        case PKMN_GAME_ALPHA_SAPPHIRE:
            ret = 6;
            break;

        default:
            break;
    }

    return ret;
}

#endif /* PKMNTEST_C_UTIL_H */
