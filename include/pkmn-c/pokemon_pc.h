/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEMON_PC_H
#define PKMN_C_POKEMON_PC_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/pokemon_box.h>
#include <pkmn-c/types/pokemon_box_list.h>
#include <pkmn-c/types/string_types.h>

#if !defined(PKMN_POKEMON_PC_DECLARED) && !defined(__DOXYGEN__)
struct pkmn_pokemon_pc_t;
typedef struct pkmn_pokemon_pc_t pkmn_pokemon_pc_t;
#define PKMN_POKEmon_pc_PC_DECLARED
#endif

typedef pkmn_pokemon_pc_t* pkmn_pokemon_pc_handle_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_pokemon_pc_make(
    pkmn_pokemon_pc_handle_t* handle_ptr,
    const char* game
);

PKMN_API pkmn_error_t pkmn_pokemon_pc_free(
    pkmn_pokemon_pc_handle_t* handle_ptr
);

PKMN_API const char* pkmn_pokemon_pc_strerror(
    pkmn_pokemon_pc_handle_t handle
);

PKMN_API pkmn_error_t pkmn_pokemon_pc_get_game(
    pkmn_pokemon_pc_handle_t handle,
    char* game_out,
    size_t buffer_len
);

PKMN_API pkmn_error_t pkmn_pokemon_pc_get_num_boxes(
    pkmn_pokemon_pc_handle_t handle,
    int* num_boxes_out
);

PKMN_API pkmn_error_t pkmn_pokemon_pc_get_box(
    pkmn_pokemon_pc_handle_t handle,
    int index,
    pkmn_pokemon_box_handle_t* pokemon_box_handle_out
);

PKMN_API pkmn_error_t pkmn_pokemon_pc_as_array(
    pkmn_pokemon_pc_handle_t handle,
    pkmn_pokemon_box_list_t* pokemon_box_list_out
);

PKMN_API pkmn_error_t pkmn_pokemon_pc_get_box_names(
    pkmn_pokemon_pc_handle_t handle,
    pkmn_string_list_t* box_names_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_PC_H */
