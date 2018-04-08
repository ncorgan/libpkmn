/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/natural_gift.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

pkmn_error_t pkmn_natural_gift_free(
    struct pkmn_natural_gift* natural_gift_ptr
)
{
    PKMN_CHECK_NULL_PARAM(natural_gift_ptr);

    pkmn::c::free_pointer_and_set_to_null(&natural_gift_ptr->type);
    natural_gift_ptr->base_power = 0;

    return PKMN_ERROR_NONE;
}
