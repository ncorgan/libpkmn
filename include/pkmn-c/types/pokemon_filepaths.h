/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_POKEMON_FILEPATHS_H
#define PKMN_C_TYPES_POKEMON_FILEPATHS_H

#include <pkmn-c/error.h>

#include <stdlib.h>

typedef struct
{
    char* icon_filepath;
    char* sprite_filepath;
} pkmn_pokemon_filepaths_t;

#ifdef __cplusplus
extern "C" {
#endif

static inline pkmn_error_t pkmn_pokemon_filepaths_free(
    pkmn_pokemon_filepaths_t* pokemon_filepaths_ptr
)
{
    if(!pokemon_filepaths_ptr)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    pkmn_error_t error = PKMN_ERROR_NONE;

    free(pokemon_filepaths_ptr->icon_filepath);
    pokemon_filepaths_ptr->icon_filepath = NULL;

    free(pokemon_filepaths_ptr->sprite_filepath);
    pokemon_filepaths_ptr->sprite_filepath = NULL;

    return error;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_POKEMON_FILEPATHS_H */
