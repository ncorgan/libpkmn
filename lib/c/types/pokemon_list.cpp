/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/pokemon_list.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

enum pkmn_error pkmn_pokemon_list_free(
    struct pkmn_pokemon_list* pokemon_list_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_list_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;

    if(pokemon_list_ptr->length > 0)
    {
        for(size_t pokemon_index = 0;
            (pokemon_index < pokemon_list_ptr->length) && !error;
            ++pokemon_index)
        {
            error = pkmn_pokemon_free(&pokemon_list_ptr->pokemon[pokemon_index]);
        }

        if(!error)
        {
            std::free(pokemon_list_ptr->pokemon);
        }
    }

    if(!error)
    {
        pokemon_list_ptr->pokemon = nullptr;
        pokemon_list_ptr->length = 0;
    }

    return error;
}
