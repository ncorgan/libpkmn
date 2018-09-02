/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/levelup_move.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include "common/misc.hpp"

enum pkmn_error pkmn_levelup_moves_free(
    struct pkmn_levelup_moves* p_levelup_moves
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_levelup_moves,
                                "p_levelup_moves"
                            );
    if(!error)
    {
        pkmn::c::free_pointer_and_set_to_null(&p_levelup_moves->p_levelup_moves);
        p_levelup_moves->length = 0;
    }

    return error;
}
