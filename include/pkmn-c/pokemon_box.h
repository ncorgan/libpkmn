/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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

typedef struct
{
    char* game;
    size_t capacity;

    void* _internal;
} pkmn_pokemon_box_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API pkmn_error_t pkmn_pokemon_box_init(
    const char* game,
    pkmn_pokemon_box_t* pokemon_box_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_free(
    pkmn_pokemon_box_t* pokemon_box_ptr
);

PKMN_C_API const char* pkmn_pokemon_box_strerror(
    pkmn_pokemon_box_t* pokemon_box_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_get_name(
    pkmn_pokemon_box_t* pokemon_box_ptr,
    char* name_buffer_out,
    size_t name_buffer_length,
    size_t* actual_name_length_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_set_name(
    pkmn_pokemon_box_t* pokemon_box_ptr,
    const char* name
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_get_num_pokemon(
    pkmn_pokemon_box_t* pokemon_box_ptr,
    size_t* num_pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_get_pokemon(
    pkmn_pokemon_box_t* pokemon_box_ptr,
    size_t position,
    pkmn_pokemon_t* pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_set_pokemon(
    pkmn_pokemon_box_t* pokemon_box_ptr,
    size_t position,
    pkmn_pokemon_t* pokemon_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon_box_as_list(
    pkmn_pokemon_box_t* pokemon_box_ptr,
    pkmn_pokemon_list_t* pokemon_list_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_BOX_H */
