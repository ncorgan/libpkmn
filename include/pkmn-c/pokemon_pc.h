/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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

typedef struct
{
    char* game;
    size_t capacity;

    void* _internal;
} pkmn_pokemon_pc_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API pkmn_error_t pkmn_pokemon_pc_init(
    const char* game,
    pkmn_pokemon_pc_t* pokemon_pc_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_pc_free(
    pkmn_pokemon_pc_t* pokemon_pc_ptr
);

PKMN_C_API const char* pkmn_pokemon_pc_strerror(
    pkmn_pokemon_pc_t* pokemon_pc_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon_pc_get_box(
    pkmn_pokemon_pc_t* pokemon_pc_ptr,
    size_t index,
    pkmn_pokemon_box_t* pokemon_box_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_pc_get_box_names(
    pkmn_pokemon_pc_t* pokemon_pc_ptr,
    pkmn_string_list_t* box_names_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_pc_as_list(
    pkmn_pokemon_pc_t* pokemon_box_ptr,
    pkmn_pokemon_box_list_t* pokemon_box_list_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_PC_H */
