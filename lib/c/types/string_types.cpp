/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/string_types.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

enum pkmn_error pkmn_string_list_free(
    struct pkmn_string_list* p_string_list
)
{
    PKMN_CHECK_NULL_PARAM(p_string_list);

    enum pkmn_error error = PKMN_ERROR_NONE;

    if(p_string_list->length > 0)
    {
        for(size_t string_index = 0;
            string_index < p_string_list->length;
            ++string_index)
        {
            std::free(p_string_list->pp_strings[string_index]);
        }
        std::free(p_string_list->pp_strings);
    }

    p_string_list->pp_strings = nullptr;
    p_string_list->length = 0;

    return error;
}

enum pkmn_error pkmn_string_pair_free(
    struct pkmn_string_pair* p_string_pair
)
{
    PKMN_CHECK_NULL_PARAM(p_string_pair);

    pkmn::c::free_pointer_and_set_to_null(&p_string_pair->p_first);
    pkmn::c::free_pointer_and_set_to_null(&p_string_pair->p_second);

    return PKMN_ERROR_NONE;
}
