/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEMON_BOX_H
#define PKMN_C_POKEMON_BOX_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>
#include <pkmn-c/pokemon.h>

#include <pkmn-c/types/pokemon_list.h>

#if !defined(PKMN_POKEMON_BOX_DECLARED) && !defined(__DOXYGEN__)
struct pkmn_pokemon_box_t;
typedef struct pkmn_pokemon_box_t pkmn_pokemon_box_t;
#define PKMN_POKEMON_BOX_DECLARED
#endif

typedef pkmn_pokemon_box_t* pkmn_pokemon_box_handle_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API pkmn_error_t pkmn_pokemon_box_make(
    pkmn_pokemon_box_handle_t* handle_ptr,
    const char* game
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_free(
    pkmn_pokemon_box_handle_t* handle_ptr
);

PKMN_C_API const char* pkmn_pokemon_box_strerror(
    pkmn_pokemon_box_handle_t handle
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_get_name(
    pkmn_pokemon_box_handle_t handle,
    char* name_out,
    size_t buffer_len,
    size_t* name_length_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_set_name(
    pkmn_pokemon_box_handle_t handle,
    const char* name
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_get_game(
    pkmn_pokemon_box_handle_t handle,
    char* game_out,
    size_t buffer_len,
    size_t* game_length_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_get_num_pokemon(
    pkmn_pokemon_box_handle_t handle,
    int* num_pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_get_capacity(
    pkmn_pokemon_box_handle_t handle,
    int* capacity_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_get_pokemon(
    pkmn_pokemon_box_handle_t handle,
    int index,
    pkmn_pokemon_handle_t* pokemon_handle_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_set_pokemon(
    pkmn_pokemon_box_handle_t handle,
    int index,
    pkmn_pokemon_handle_t pokemon_handle
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_as_array(
    pkmn_pokemon_box_handle_t handle,
    pkmn_pokemon_list_t* pokemon_list_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_BOX_H */
