/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_POKEMON_LIST_H
#define PKMN_C_TYPES_POKEMON_LIST_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>
#include <pkmn-c/pokemon.h>

#include <stdlib.h>

typedef struct {
    pkmn_pokemon_handle_t* pokemon_list;
    size_t length;
} pkmn_pokemon_list_t;

#ifdef __cplusplus
extern "C" {
#endif

static PKMN_INLINE pkmn_error_t pkmn_pokemon_list_free(
    pkmn_pokemon_list_t* pokemon_list
) {
    for(size_t i = 0; i < pokemon_list->length; ++i) {
        pkmn_pokemon_free(&pokemon_list->pokemon_list[i]);
    }

    pokemon_list->pokemon_list = NULL;
    pokemon_list->length = 0;

    return PKMN_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_POKEMON_LIST_H */
