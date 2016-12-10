/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_POKEMON_LIST_H
#define PKMN_C_TYPES_POKEMON_LIST_H

#include <pkmn-c/config.h>
#include <pkmn-c/pokemon.h>

#include <stdlib.h>

typedef pkmn_pokemon_handle_t* pkmn_pokemon_list_t;

#ifdef __cplusplus
extern "C" {
#endif

static PKMN_INLINE void pkmn_pokemon_list_free(
    pkmn_pokemon_list_t* pokemon_list,
    size_t list_length
) {
    for(size_t i = 0; i < list_length; ++i) {
        pkmn_pokemon_free(pokemon_list[i]);
    }

    (*pokemon_list) = NULL;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_POKEMON_LIST_H */
