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

static inline pkmn_error_t pkmn_pokemon_box_list_free(
    pkmn_pokemon_box_list_t* pokemon_box_list_ptr
)
{
    if(!pokemon_box_list_ptr)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    pkmn_error_t error = PKMN_ERROR_NONE;

    for(size_t index = 0; (index < pokemon_box_list_ptr->length) && !error; ++index)
    {
        error = pkmn_pokemon_box_free(&pokemon_box_list_ptr->boxes[index]);
    }

    pokemon_box_list_ptr->boxes = NULL;
    pokemon_box_list_ptr->length = 0;

    return PKMN_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_POKEMON_BOX_LIST_H */
