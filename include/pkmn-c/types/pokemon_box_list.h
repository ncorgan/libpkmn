/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_POKEMON_BOX_LIST_H
#define PKMN_C_TYPES_POKEMON_BOX_LIST_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>
#include <pkmn-c/pokemon_box.h>

#include <stdlib.h>

struct pkmn_pokemon_box_list
{
    struct pkmn_pokemon_box* boxes;
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_pokemon_box_list_free(
    struct pkmn_pokemon_box_list* pokemon_box_list_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_POKEMON_BOX_LIST_H */
