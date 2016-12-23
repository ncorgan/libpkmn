/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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
) {
    PKMN_CHECK_NULL_PARAM(move_name);
    PKMN_CHECK_NULL_PARAM(move_game);
    PKMN_CHECK_NULL_PARAM(move_entry_out);

    PKMN_CPP_TO_C(
        pkmn::database::move_entry move_entry_cpp(
                                       move_name,
                                       move_game
                                   );

        pkmn::pkmn_move_entry_cpp_to_c(
            move_entry_cpp,
            move_entry_out
        );
    )
}

pkmn_error_t pkmn_database_move_entry_free(
    pkmn_database_move_entry_t* move_entry
) {
    PKMN_CHECK_NULL_PARAM(move_entry);

    std::free(move_entry->name);
    move_entry->name = NULL;

    std::free(move_entry->game);
    move_entry->game = NULL;

    std::free(move_entry->description);
    move_entry->description = NULL;

    std::free(move_entry->target);
    move_entry->target = NULL;

    std::free(move_entry->damage_class);
    move_entry->damage_class = NULL;

    move_entry->base_power = 0;
    for(int i = 0; i < 4; ++i) {
        move_entry->pp[i] = 0;
    }
    move_entry->accuracy = 0.0f;

    std::free(move_entry->effect);
    move_entry->effect = NULL;

    std::free(move_entry->contest_type);
    move_entry->contest_type = NULL;

    std::free(move_entry->contest_effect);
    move_entry->contest_effect = NULL;

    std::free(move_entry->super_contest_effect);
    move_entry->super_contest_effect = NULL;

    return PKMN_ERROR_NONE;
}
