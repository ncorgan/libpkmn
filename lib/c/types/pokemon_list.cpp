/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/pokemon_list.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include "common/misc.hpp"

enum pkmn_error pkmn_pokemon_list_free(
    struct pkmn_pokemon_list* p_pokemon_list
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_pokemon_list,
                                "p_pokemon_list"
                            );

    if(!error && (p_pokemon_list->length > 0))
    {
        for(size_t pokemon_index = 0;
            (pokemon_index < p_pokemon_list->length) && !error;
            ++pokemon_index)
        {
            error = pkmn_pokemon_free(&p_pokemon_list->p_pokemon[pokemon_index]);
        }

        if(!error)
        {
            std::free(p_pokemon_list->p_pokemon);
        }
    }

    if(!error)
    {
        p_pokemon_list->p_pokemon = nullptr;
        p_pokemon_list->length = 0;
    }

    return error;
}
