/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/hidden_power.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

pkmn_error_t pkmn_hidden_power_free(
    struct pkmn_hidden_power* hidden_power_ptr
)
{
    PKMN_CHECK_NULL_PARAM(hidden_power_ptr);

    pkmn::c::free_pointer_and_set_to_null(&hidden_power_ptr->type);
    hidden_power_ptr->base_power = 0;

    return PKMN_ERROR_NONE;
}
