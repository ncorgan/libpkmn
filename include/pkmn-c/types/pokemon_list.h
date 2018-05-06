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

struct pkmn_pokemon_list
{
    struct pkmn_pokemon* p_pokemon;
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_pokemon_list_free(
    struct pkmn_pokemon_list* p_pokemon_list
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_POKEMON_LIST_H */
