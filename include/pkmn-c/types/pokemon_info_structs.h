/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_POKEMON_INFO_H
#define PKMN_C_TYPES_POKEMON_INFO_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/types/condition.h>
#include <pkmn-c/types/gender.h>
#include <pkmn-c/types/trainer_info.h>

#include <stdint.h>

typedef struct
{
    char* icon_filepath;
    char* sprite_filepath;
} pkmn_pokemon_filepaths_t;

typedef struct
{
    bool is_egg;
    int pokerus_duration;

    uint32_t personality;
    pkmn_gender_t gender;
    char* ability;

    char* nickname;
    char* held_item;

    int experience;
} pkmn_pokemon_info_t;

typedef struct
{
    pkmn_trainer_info2_t trainer_info;

    char* original_game;
    char* ball;
    int level_met;

    char* location_met;
    char* location_met_as_egg;
} pkmn_pokemon_origin_info_t;

typedef struct
{
    int current_hp;

    pkmn_condition_t condition;
} pkmn_pokemon_battle_info_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API pkmn_error_t pkmn_pokemon_filepaths_free(
    pkmn_pokemon_filepaths_t* pokemon_filepaths_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon_info_free(
    pkmn_pokemon_info_t* pokemon_info_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon_origin_info_free(
    pkmn_pokemon_origin_info_t* pokemon_origin_info_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon_battle_info_free(
    pkmn_pokemon_battle_info_t* pokemon_battle_info_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_POKEMON_INFO_H */
