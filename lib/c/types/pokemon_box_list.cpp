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
    struct pkmn_pokemon_box_list* pokemon_box_list_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_box_list_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;

    if(pokemon_box_list_ptr->length > 0)
    {
        for(size_t pokemon_index = 0;
            (pokemon_index < pokemon_box_list_ptr->length) && !error;
            ++pokemon_index)
        {
            error = pkmn_pokemon_box_free(&pokemon_box_list_ptr->boxes[pokemon_index]);
        }

        if(!error)
        {
            std::free(pokemon_box_list_ptr->boxes);
        }
    }

    if(!error)
    {
        pokemon_box_list_ptr->boxes = nullptr;
        pokemon_box_list_ptr->length = 0;
    }

    return error;
}
