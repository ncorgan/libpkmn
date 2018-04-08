/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/move_slot.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

pkmn_error_t pkmn_move_slot_free(
    struct pkmn_move_slot* move_slot_ptr
)
{
    PKMN_CHECK_NULL_PARAM(move_slot_ptr);

    pkmn::c::free_pointer_and_set_to_null(&move_slot_ptr->move);
    move_slot_ptr->pp = 0;

    return PKMN_ERROR_NONE;
}

pkmn_error_t pkmn_move_slots_free(
    struct pkmn_move_slots* move_slots_ptr
)
{
    PKMN_CHECK_NULL_PARAM(move_slots_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    if(move_slots_ptr->length > 0)
    {
        for(size_t move_index = 0;
            (move_index < move_slots_ptr->length) && !error;
            ++move_index)
        {
            error = pkmn_move_slot_free(&move_slots_ptr->move_slots[move_index]);
        }

        if(!error)
        {
            std::free(move_slots_ptr->move_slots);
        }
    }

    if(!error)
    {
        move_slots_ptr->move_slots = nullptr;
        move_slots_ptr->length = 0;
    }

    return error;
}
