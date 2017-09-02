/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_ITEM_POCKETS_H
#define PKMN_C_TYPES_ITEM_POCKETS_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>
#include <pkmn-c/item_list.h>

#include <pkmn-c/types/string_types.h>

#include <stdlib.h>

typedef struct
{
    pkmn_item_list2_t* pockets;
    pkmn_string_list_t pocket_names;

    size_t num_pockets;
} pkmn_item_pockets_t;

#ifdef __cplusplus
extern "C" {
#endif

static PKMN_INLINE pkmn_error_t pkmn_item_pockets_free(
    pkmn_item_pockets_t* item_pockets
)
{
    if(!item_pockets)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    for(size_t i = 0; i < item_pockets->num_pockets; ++i)
    {
        pkmn_item_list2_free(&item_pockets->pockets[i]);
    }
    free(item_pockets->pockets);
    pkmn_string_list_free(&item_pockets->pocket_names);

    return PKMN_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_ITEM_POCKETS_H */
