/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_POKEMON_LIST_H
#define PKMN_C_TYPES_POKEMON_LIST_H

#include <pkmn-c/error.h>
#include <pkmn-c/pokemon.h>

#include <stdlib.h>

typedef struct
{
    pkmn_pokemon_t* pokemon;
    size_t length;
} pkmn_pokemon_list_t;

#ifdef __cplusplus
extern "C" {
#endif

static inline pkmn_error_t pkmn_pokemon_list_free(
    pkmn_pokemon_list_t* pokemon_list
)
{
    if(!pokemon_list)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    pkmn_error_t error = PKMN_ERROR_NONE;

    for(size_t index = 0; (index < pokemon_list->length) && !error; ++index)
    {
        error = pkmn_pokemon_free(&pokemon_list->pokemon[index]);
    }

    if(!error)
    {
        pokemon_list->pokemon = NULL;
        pokemon_list->length = 0;
    }

    return PKMN_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_POKEMON_LIST_H */
