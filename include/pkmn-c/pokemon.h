/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEMON_H
#define PKMN_C_POKEMON_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/item_list.h>
#include <pkmn-c/types/string_types.h>

#include <stdint.h>

#define LIBPKMN_OT_ID   2105214279
#define LIBPKMN_OT_NAME "LibPKMN"

#if !defined(PKMN_POKEMON_DECLARED) && !defined(__DOXYGEN__)
struct pkmn_pokemon_t;
typedef struct pkmn_pokemon_t pkmn_pokemon_t;
#define PKMN_POKEMON_DECLARED
#endif

typedef pkmn_pokemon_t* pkmn_pokemon_handle_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_pokemon_make(
    pkmn_pokemon_handle_t* handle_ptr,
    const char* species,
    const char* game,
    const char* form,
    int level
);

PKMN_API pkmn_error_t pkmn_pokemon_free(
    pkmn_pokemon_handle_t* handle_ptr
);

PKMN_API const char* pkmn_pokemon_strerror(
    pkmn_pokemon_handle_t handle
);

PKMN_API pkmn_error_t pkmn_pokemon_get_game(
    pkmn_pokemon_handle_t handle,
    char* game_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_H */
