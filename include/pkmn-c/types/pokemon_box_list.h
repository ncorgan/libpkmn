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

typedef struct
{
    pkmn_pokemon_box_t* boxes;
    size_t length;
} pkmn_pokemon_box_list_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API pkmn_error_t pkmn_pokemon_box_list_free(
    pkmn_pokemon_box_list_t* pokemon_box_list_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_POKEMON_BOX_LIST_H */
