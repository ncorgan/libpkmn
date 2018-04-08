/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/item_slot.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

pkmn_error_t pkmn_item_slot_free(
    struct pkmn_item_slot* item_slot_ptr
)
{
    PKMN_CHECK_NULL_PARAM(item_slot_ptr);

    pkmn::c::free_pointer_and_set_to_null(&item_slot_ptr->item);
    item_slot_ptr->amount = 0;

    return PKMN_ERROR_NONE;
}

pkmn_error_t pkmn_item_slots_free(
    struct pkmn_item_slots* item_slots_ptr
)
{
    PKMN_CHECK_NULL_PARAM(item_slots_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    if(item_slots_ptr->length > 0)
    {
        for(size_t item_index = 0;
            (item_index < item_slots_ptr->length) && !error;
            ++item_index)
        {
            error = pkmn_item_slot_free(&item_slots_ptr->item_slots[item_index]);
        }

        if(!error)
        {
            std::free(item_slots_ptr->item_slots);
        }
    }

    if(!error)
    {
        item_slots_ptr->item_slots = nullptr;
        item_slots_ptr->length = 0;
    }

    return error;
}
