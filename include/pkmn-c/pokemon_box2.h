/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEMON_BOX2_H
#define PKMN_C_POKEMON_BOX2_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/pokemon2.h>

typedef struct
{
    char* game;
    size_t capacity;

    void* _internal;
} pkmn_pokemon_box2_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API pkmn_error_t pkmn_pokemon_box2_init(
    const char* game,
    pkmn_pokemon_box2_t* pokemon_box_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box2_free(
    pkmn_pokemon_box2_t* pokemon_box_ptr
);

PKMN_C_API const char* pkmn_pokemon_box2_strerror(
    pkmn_pokemon_box2_t* pokemon_box_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box2_get_name(
    pkmn_pokemon_box2_t* pokemon_box_ptr,
    char* name_buffer_out,
    size_t name_buffer_length,
    size_t* actual_name_length_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box2_set_name(
    pkmn_pokemon_box2_t* pokemon_box_ptr,
    const char* name
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box2_get_num_pokemon(
    pkmn_pokemon_box2_t* pokemon_box_ptr,
    size_t* num_pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box2_get_pokemon(
    pkmn_pokemon_box2_t* pokemon_box_ptr,
    size_t position,
    pkmn_pokemon2_t* pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box2_set_pokemon(
    pkmn_pokemon_box2_t* pokemon_box_ptr,
    size_t position,
    pkmn_pokemon2_t* pokemon_ptr
);

// TODO: if a Pokémon is set, what happens to the Pokémon
// returned here?
PKMN_C_API pkmn_error_t pkmn_pokemon_box2_as_list(
    pkmn_pokemon_box2_t* pokemon_box_ptr,
    pkmn_pokemon_list2_t* pokemon_list_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_BOX2_H */
