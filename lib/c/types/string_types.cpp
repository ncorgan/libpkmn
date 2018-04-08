/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/string_types.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

pkmn_error_t pkmn_string_list_free(
    struct pkmn_string_list* string_list_ptr
)
{
    PKMN_CHECK_NULL_PARAM(string_list_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    if(string_list_ptr->length > 0)
    {
        for(size_t string_index = 0; string_index < string_list_ptr->length; ++string_index)
        {
            std::free(string_list_ptr->strings[string_index]);
        }
        std::free(string_list_ptr->strings);
    }

    string_list_ptr->strings = nullptr;
    string_list_ptr->length = 0;

    return error;
}

pkmn_error_t pkmn_string_pair_free(
    struct pkmn_string_pair* string_pair_ptr
)
{
    PKMN_CHECK_NULL_PARAM(string_pair_ptr);

    pkmn::c::free_pointer_and_set_to_null(&string_pair_ptr->first);
    pkmn::c::free_pointer_and_set_to_null(&string_pair_ptr->second);

    return PKMN_ERROR_NONE;
}
