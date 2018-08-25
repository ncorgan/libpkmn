/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/item_slot.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include "common/misc.hpp"

enum pkmn_error pkmn_item_slot_free(
    struct pkmn_item_slot* p_item_slot
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_item_slot,
                                "p_item_slot"
                            );
    if(!error)
    {
        pkmn::c::free_pointer_and_set_to_null(&p_item_slot->p_item);
        p_item_slot->amount = 0;
    }

    return error;
}

enum pkmn_error pkmn_item_slots_free(
    struct pkmn_item_slots* p_item_slots
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_item_slots,
                                "p_item_slots"
                            );

    if(!error && (p_item_slots->length > 0))
    {
        for(size_t item_index = 0;
            (item_index < p_item_slots->length) && !error;
            ++item_index)
        {
            error = pkmn_item_slot_free(&p_item_slots->p_item_slots[item_index]);
        }

        if(!error)
        {
            std::free(p_item_slots->p_item_slots);
        }
    }

    if(!error)
    {
        p_item_slots->p_item_slots = nullptr;
        p_item_slots->length = 0;
    }

    return error;
}
