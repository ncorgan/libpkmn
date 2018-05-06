/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/levelup_move.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

enum pkmn_error pkmn_levelup_move_free(
    struct pkmn_levelup_move* p_levelup_move
)
{
    PKMN_CHECK_NULL_PARAM(p_levelup_move);

    pkmn::c::free_pointer_and_set_to_null(&p_levelup_move->p_move);
    p_levelup_move->level = 0;

    return PKMN_ERROR_NONE;
}

enum pkmn_error pkmn_levelup_moves_free(
    struct pkmn_levelup_moves* p_levelup_moves
)
{
    PKMN_CHECK_NULL_PARAM(p_levelup_moves);

    enum pkmn_error error = PKMN_ERROR_NONE;

    if(p_levelup_moves->length > 0)
    {
        for(size_t move_index = 0;
            (move_index < p_levelup_moves->length) && !error;
            ++move_index)
        {
            error = pkmn_levelup_move_free(&p_levelup_moves->p_levelup_moves[move_index]);
        }

        if(!error)
        {
            std::free(p_levelup_moves->p_levelup_moves);
        }
    }

    if(!error)
    {
        p_levelup_moves->p_levelup_moves = nullptr;
        p_levelup_moves->length = 0;
    }

    return error;
}
