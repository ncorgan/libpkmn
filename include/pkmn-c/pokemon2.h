/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEMON2_H
#define PKMN_C_POKEMON2_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/database/pokemon_entry.h>

#include <pkmn-c/types/pokemon_info.h>
#include <pkmn-c/types/trainer_id.h>
#include <pkmn-c/types/trainer_info.h>

typedef struct
{
    char* species;
    char* game;

    void* _internal;
} pkmn_pokemon2_t;

#ifdef __cplusplus
extern "C" {
#endif

pkmn_trainer_id2_t pkmn_pokemon2_default_trainer_id();

const char* pkmn_pokemon2_default_trainer_name();

PKMN_C_API pkmn_error_t pkmn_pokemon2_init(
    const char* species,
    const char* game,
    const char* form,
    int level,
    pkmn_pokemon2_t* pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_init_from_file(
    const char* filepath,
    pkmn_pokemon2_t* pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_free(
    pkmn_pokemon2_t* pokemon_ptr
);

PKMN_C_API const char* pkmn_pokemon2_strerror(
    pkmn_pokemon2_t* pokemon_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_to_game(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* game,
    pkmn_pokemon2_t* new_pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_export_to_file(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* filepath
);

// Pokémon info

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_pokemon_info(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_pokemon_info_t* pokemon_info_ptr
);

// Original trainer info

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_original_trainer_info(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_trainer_info2_t* trainer_info_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_original_trainer_name(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* original_trainer_name
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_original_trainer_id(
    pkmn_pokemon2_t* pokemon_ptr,
    uint32_t original_trainer_id
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_original_trainer_public_id(
    pkmn_pokemon2_t* pokemon_ptr,
    uint16_t original_trainer_public_id
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_original_trainer_secret_id(
    pkmn_pokemon2_t* pokemon_ptr,
    uint16_t original_trainer_secret_id
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON2_H */
