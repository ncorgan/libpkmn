/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include "common/misc.hpp"

#include <pkmn-c/database/item_entry.h>

#include <pkmn/database/item_entry.hpp>

#include <cstdlib>
#include <cstring>

enum pkmn_error pkmn_database_get_item_entry(
    const char* p_item_name,
    const char* p_item_game,
    struct pkmn_database_item_entry* p_item_entry_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_item_name,
                "p_item_name"
            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_item_game,
                    "p_item_game"
                );
    }
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_item_entry_out,
                    "p_item_entry_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::database::item_entry item_entry_cpp(
                                           p_item_name,
                                           p_item_game
                                       );
            pkmn::c::item_entry_cpp_to_c(
                item_entry_cpp,
                p_item_entry_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_database_item_entry_free(
    struct pkmn_database_item_entry* p_item_entry
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_item_entry,
                "p_item_entry"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::free_pointer_and_set_to_null(&p_item_entry->p_name);
            pkmn::c::free_pointer_and_set_to_null(&p_item_entry->p_game);
            pkmn::c::free_pointer_and_set_to_null(&p_item_entry->p_category);
            pkmn::c::free_pointer_and_set_to_null(&p_item_entry->p_description);
            pkmn::c::free_pointer_and_set_to_null(&p_item_entry->p_pocket);
            pkmn::c::free_pointer_and_set_to_null(&p_item_entry->p_fling_effect);

            std::memset(p_item_entry, 0, sizeof(*p_item_entry));
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}
