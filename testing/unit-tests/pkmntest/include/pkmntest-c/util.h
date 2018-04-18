/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMNTEST_C_UTIL_H
#define PKMNTEST_C_UTIL_H

#include <pkmntest-c/config.h>

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
        if(!strcmp(string_list->strings[i], string))
        {
            return true;
        }
    }

    return false;
}

static inline int game_to_generation(
    const char* game
)
{
    int ret = 0;
    if(!strcmp(game, "Red") || !strcmp(game, "Blue") || !strcmp(game, "Yellow"))
    {
        ret = 1;
    }
    else if(!strcmp(game, "Gold") || !strcmp(game, "Silver") || !strcmp(game, "Crystal"))
    {
        ret = 2;
    }
    else if(!strcmp(game, "Ruby") || !strcmp(game, "Sapphire") || !strcmp(game, "Emerald") ||
            !strcmp(game, "FireRed") || !strcmp(game, "LeafGreen") ||
            !strcmp(game, "Colosseum") || !strcmp(game, "XD"))
    {
        ret = 3;
    }
    else if(!strcmp(game, "Diamond") || !strcmp(game, "Pearl") || !strcmp(game, "Platinum") ||
              !strcmp(game, "HeartGold") || !strcmp(game, "SoulSilver"))
    {
        ret = 4;
    }
    else if(!strcmp(game, "Black") || !strcmp(game, "White") ||
              !strcmp(game, "Black 2") || !strcmp(game, "White 2"))
    {
        ret = 5;
    }
    else if(!strcmp(game, "X") || !strcmp(game, "Y") ||
            !strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire"))
    {
        ret = 6;
    }

    return ret;
}

#endif /* PKMNTEST_C_UTIL_H */
