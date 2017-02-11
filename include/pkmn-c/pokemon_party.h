/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
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

#if !defined(PKMN_POKEMON_PARTY_DECLARED) && !defined(__DOXYGEN__)
struct pkmn_pokemon_party_t;
typedef struct pkmn_pokemon_party_t pkmn_pokemon_party_t;
#define PKMN_POKEmon_party_PARTY_DECLARED
#endif

typedef pkmn_pokemon_party_t* pkmn_pokemon_party_handle_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_pokemon_party_make(
    pkmn_pokemon_party_handle_t* handle_ptr,
    const char* game
);

PKMN_API pkmn_error_t pkmn_pokemon_party_free(
    pkmn_pokemon_party_handle_t* handle_ptr
);

PKMN_API const char* pkmn_pokemon_party_strerror(
    pkmn_pokemon_party_handle_t handle
);

PKMN_API pkmn_error_t pkmn_pokemon_party_get_game(
    pkmn_pokemon_party_handle_t handle,
    char* game_out,
    size_t buffer_len
);

PKMN_API pkmn_error_t pkmn_pokemon_party_get_num_pokemon(
    pkmn_pokemon_party_handle_t handle,
    int* num_pokemon_out
);

PKMN_API pkmn_error_t pkmn_pokemon_party_get_pokemon(
    pkmn_pokemon_party_handle_t handle,
    int index,
    pkmn_pokemon_handle_t* pokemon_handle_out
);

PKMN_API pkmn_error_t pkmn_pokemon_party_set_pokemon(
    pkmn_pokemon_party_handle_t handle,
    int index,
    pkmn_pokemon_handle_t pokemon_handle
);

PKMN_API pkmn_error_t pkmn_pokemon_party_as_array(
    pkmn_pokemon_party_handle_t handle,
    pkmn_pokemon_list_t* pokemon_list_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_PARTY_H */
