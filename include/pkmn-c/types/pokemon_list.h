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

PKMN_C_API pkmn_error_t pkmn_pokemon_list_free(
    pkmn_pokemon_list_t* pokemon_list
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_POKEMON_LIST_H */
