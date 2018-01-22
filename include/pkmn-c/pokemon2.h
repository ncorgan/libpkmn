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

typedef struct
{
    char* species;
    char* game;

    void* _internal;
} pkmn_pokemon2_t;

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON2_H */
