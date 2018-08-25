/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include "common/misc.hpp"

#include <pkmn-c/database/move_entry.h>

#include <pkmn/database/move_entry.hpp>

#include <cstdlib>
#include <cstring>

enum pkmn_error pkmn_database_get_move_entry(
    const char* p_move_name,
    const char* p_move_game,
    struct pkmn_database_move_entry* p_move_entry_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_move_name,
                "p_move_name"
            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_move_game,
                    "p_move_game"
                );
    }
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_move_entry_out,
                    "p_move_entry_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::database::move_entry move_entry_cpp(
                                           p_move_name,
                                           p_move_game
                                       );

            pkmn::c::move_entry_cpp_to_c(
                move_entry_cpp,
                p_move_entry_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_database_move_entry_free(
    struct pkmn_database_move_entry* p_move_entry
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_move_entry,
                "p_move_entry"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::free_pointer_and_set_to_null(&p_move_entry->p_name);
            pkmn::c::free_pointer_and_set_to_null(&p_move_entry->p_game);
            pkmn::c::free_pointer_and_set_to_null(&p_move_entry->p_description);
            pkmn::c::free_pointer_and_set_to_null(&p_move_entry->p_target);
            pkmn::c::free_pointer_and_set_to_null(&p_move_entry->p_damage_class);
            pkmn::c::free_pointer_and_set_to_null(&p_move_entry->p_effect);
            pkmn::c::free_pointer_and_set_to_null(&p_move_entry->p_contest_type);
            pkmn::c::free_pointer_and_set_to_null(&p_move_entry->p_contest_effect);
            pkmn::c::free_pointer_and_set_to_null(&p_move_entry->p_super_contest_effect);

            std::memset(p_move_entry, 0, sizeof(*p_move_entry));
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}
