/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/hidden_power.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

enum pkmn_error pkmn_hidden_power_free(
    struct pkmn_hidden_power* p_hidden_power
)
{
    PKMN_CHECK_NULL_PARAM(p_hidden_power);

    pkmn::c::free_pointer_and_set_to_null(&p_hidden_power->p_type);
    p_hidden_power->base_power = 0;

    return PKMN_ERROR_NONE;
}
