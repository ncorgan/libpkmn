/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/pokemon_box_list.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

enum pkmn_error pkmn_pokemon_box_list_free(
    struct pkmn_pokemon_box_list* p_pokemon_box_list
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_box_list);

    enum pkmn_error error = PKMN_ERROR_NONE;

    if(p_pokemon_box_list->length > 0)
    {
        for(size_t pokemon_index = 0;
            (pokemon_index < p_pokemon_box_list->length) && !error;
            ++pokemon_index)
        {
            error = pkmn_pokemon_box_free(&p_pokemon_box_list->p_boxes[pokemon_index]);
        }

        if(!error)
        {
            std::free(p_pokemon_box_list->p_boxes);
        }
    }

    if(!error)
    {
        p_pokemon_box_list->p_boxes = nullptr;
        p_pokemon_box_list->length = 0;
    }

    return error;
}
