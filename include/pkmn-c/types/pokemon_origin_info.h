/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_POKEMON_ORIGIN_INFO_H
#define PKMN_C_TYPES_POKEMON_ORIGIN_INFO_H

#include <pkmn-c/error.h>

#include <pkmn-c/types/trainer_info.h>

#include <stdlib.h>

typedef struct
{
    pkmn_trainer_info2_t trainer_info;

    char* original_game;
    char* ball;
    char* location_met;
    char* location_met_as_egg;
    int level_met;
} pkmn_pokemon_origin_info_t;

#ifdef __cplusplus
extern "C" {
#endif

static inline pkmn_error_t pkmn_pokemon_origin_info_free(
    pkmn_pokemon_origin_info_t* pokemon_origin_info_ptr
)
{
    if(!pokemon_origin_info_ptr)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    pkmn_trainer_info_free(&pokemon_origin_info_ptr->trainer_info);

    free(pokemon_origin_info_ptr->original_game);
    pokemon_origin_info_ptr->original_game = NULL;

    free(pokemon_origin_info_ptr->ball);
    pokemon_origin_info_ptr->ball = NULL;

    free(pokemon_origin_info_ptr->location_met);
    pokemon_origin_info_ptr->location_met = NULL;

    free(pokemon_origin_info_ptr->location_met_as_egg);
    pokemon_origin_info_ptr->location_met_as_egg = NULL;

    pokemon_origin_info_ptr->level_met = 0;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_POKEMON_ORIGIN_INFO_H */
