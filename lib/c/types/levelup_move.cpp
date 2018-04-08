/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/levelup_move.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

pkmn_error_t pkmn_levelup_move_free(
    pkmn_levelup_move_t* levelup_move_ptr
)
{
    PKMN_CHECK_NULL_PARAM(levelup_move_ptr);

    pkmn::c::free_pointer_and_set_to_null(&levelup_move_ptr->move);
    levelup_move_ptr->level = 0;

    return PKMN_ERROR_NONE;
}

pkmn_error_t pkmn_levelup_moves_free(
    pkmn_levelup_moves_t* levelup_moves_ptr
)
{
    PKMN_CHECK_NULL_PARAM(levelup_moves_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    if(levelup_moves_ptr->length > 0)
    {
        for(size_t move_index = 0;
            (move_index < levelup_moves_ptr->length) && !error;
            ++move_index)
        {
            error = pkmn_levelup_move_free(&levelup_moves_ptr->levelup_moves[move_index]);
        }

        if(!error)
        {
            std::free(levelup_moves_ptr->levelup_moves);
        }
    }

    if(!error)
    {
        levelup_moves_ptr->levelup_moves = nullptr;
        levelup_moves_ptr->length = 0;
    }

    return error;
}
