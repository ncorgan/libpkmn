/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEMON_PARTY_H
#define PKMN_C_POKEMON_PARTY_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/types/pokemon_list.h>

typedef struct
{
    char* game;
    size_t capacity;

    void* _internal;
} pkmn_pokemon_party_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API pkmn_error_t pkmn_pokemon_party_init(
    const char* game,
    pkmn_pokemon_party_t* pokemon_party_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_party_free(
    pkmn_pokemon_party_t* pokemon_party_ptr
);

PKMN_C_API const char* pkmn_pokemon_party_strerror(
    pkmn_pokemon_party_t* pokemon_party_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon_party_get_num_pokemon(
    pkmn_pokemon_party_t* pokemon_party_ptr,
    size_t* num_pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_party_get_pokemon(
    pkmn_pokemon_party_t* pokemon_party_ptr,
    size_t position,
    pkmn_pokemon_t* pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_party_set_pokemon(
    pkmn_pokemon_party_t* pokemon_party_ptr,
    size_t position,
    pkmn_pokemon_t* pokemon_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon_party_as_list(
    pkmn_pokemon_party_t* pokemon_party_ptr,
    pkmn_pokemon_list_t* pokemon_list_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_PARTY_H */
