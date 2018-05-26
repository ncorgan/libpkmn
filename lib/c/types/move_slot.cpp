/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/move_slot.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

enum pkmn_error pkmn_move_slot_free(
    struct pkmn_move_slot* p_move_slot
)
{
    PKMN_CHECK_NULL_PARAM(p_move_slot);

    pkmn::c::free_pointer_and_set_to_null(&p_move_slot->p_move);
    p_move_slot->pp = 0;

    return PKMN_ERROR_NONE;
}

enum pkmn_error pkmn_move_slots_free(
    struct pkmn_move_slots* p_move_slots
)
{
    PKMN_CHECK_NULL_PARAM(p_move_slots);

    enum pkmn_error error = PKMN_ERROR_NONE;

    if(p_move_slots->length > 0)
    {
        for(size_t move_index = 0;
            (move_index < p_move_slots->length) && !error;
            ++move_index)
        {
            error = pkmn_move_slot_free(&p_move_slots->p_move_slots[move_index]);
        }

        if(!error)
        {
            std::free(p_move_slots->p_move_slots);
        }
    }

    if(!error)
    {
        p_move_slots->p_move_slots = nullptr;
        p_move_slots->length = 0;
    }

    return error;
}
