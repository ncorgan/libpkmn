/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/item_slot.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

enum pkmn_error pkmn_item_slots_free(
    struct pkmn_item_slots* p_item_slots
)
{
    PKMN_CHECK_NULL_PARAM(p_item_slots);

    enum pkmn_error error = PKMN_ERROR_NONE;

    pkmn::c::free_pointer_and_set_to_null(&p_item_slots->p_item_slots);
    p_item_slots->length = 0;

    return error;
}
