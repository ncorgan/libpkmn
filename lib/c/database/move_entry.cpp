/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/database/move_entry.h>

#include <pkmn/database/move_entry.hpp>

#include <cstdlib>
#include <cstring>

pkmn_error_t pkmn_database_get_move_entry(
    const char* move_name,
    const char* move_game,
    pkmn_database_move_entry_t* move_entry_out
)
{
    PKMN_CHECK_NULL_PARAM(move_name);
    PKMN_CHECK_NULL_PARAM(move_game);
    PKMN_CHECK_NULL_PARAM(move_entry_out);

    PKMN_CPP_TO_C(
        pkmn::database::move_entry move_entry_cpp(
                                       move_name,
                                       move_game
                                   );

        pkmn::c::move_entry_cpp_to_c(
            move_entry_cpp,
            move_entry_out
        );
    )
}

pkmn_error_t pkmn_database_move_entry_free(
    pkmn_database_move_entry_t* move_entry
)
{
    PKMN_CHECK_NULL_PARAM(move_entry);

    pkmn::c::free_pointer_and_set_to_null(&move_entry->name);
    pkmn::c::free_pointer_and_set_to_null(&move_entry->game);
    pkmn::c::free_pointer_and_set_to_null(&move_entry->description);
    pkmn::c::free_pointer_and_set_to_null(&move_entry->target);
    pkmn::c::free_pointer_and_set_to_null(&move_entry->damage_class);
    pkmn::c::free_pointer_and_set_to_null(&move_entry->effect);
    pkmn::c::free_pointer_and_set_to_null(&move_entry->contest_type);
    pkmn::c::free_pointer_and_set_to_null(&move_entry->contest_effect);
    pkmn::c::free_pointer_and_set_to_null(&move_entry->super_contest_effect);

    move_entry->base_power = 0;
    move_entry->accuracy = 0.0f;
    for(size_t index = 0; index < 4; ++index)
    {
        move_entry->pp[index] = 0;
    }

    return PKMN_ERROR_NONE;
}
