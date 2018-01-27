/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEMON_PC2_H
#define PKMN_C_POKEMON_PC2_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/pokemon_box2.h>

#include <pkmn-c/types/pokemon_box_list.h>

typedef struct
{
    char* game;
    size_t capacity;

    void* _internal;
} pkmn_pokemon_pc2_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API pkmn_error_t pkmn_pokemon_pc2_init(
    const char* game,
    pkmn_pokemon_pc2_t* pokemon_pc_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_pc2_free(
    pkmn_pokemon_pc2_t* pokemon_pc_ptr
);

PKMN_C_API const char* pkmn_pokemon_pc2_strerror(
    pkmn_pokemon_pc2_t* pokemon_pc_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon_pc2_get_pokemon_box(
    pkmn_pokemon_pc2_t* pokemon_pc_ptr,
    size_t index,
    pkmn_pokemon_box2_t* pokemon_box_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_pc2_as_list(
    pkmn_pokemon_pc2_t* pokemon_box_ptr,
    pkmn_pokemon_box_list2_t* pokemon_box_list_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_PC2_H */
