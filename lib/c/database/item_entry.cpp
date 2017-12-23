/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/database/item_entry.h>

#include <pkmn/database/item_entry.hpp>

#include <cstdlib>
#include <cstring>

pkmn_error_t pkmn_database_get_item_entry(
    const char* item_name,
    const char* item_game,
    pkmn_database_item_entry_t* item_entry_out
)
{
    PKMN_CHECK_NULL_PARAM(item_name);
    PKMN_CHECK_NULL_PARAM(item_game);
    PKMN_CHECK_NULL_PARAM(item_entry_out);

    PKMN_CPP_TO_C(
        pkmn::database::item_entry item_entry_cpp(
                                       item_name,
                                       item_game
                                   );
        pkmn::pkmn_item_entry_cpp_to_c(
            item_entry_cpp,
            item_entry_out
        );
    )
}

pkmn_error_t pkmn_database_item_entry_free(
    pkmn_database_item_entry_t* item_entry
)
{
    PKMN_CHECK_NULL_PARAM(item_entry);

    pkmn::c::free_pointer_and_set_to_null(&item_entry->name);
    pkmn::c::free_pointer_and_set_to_null(&item_entry->game);
    pkmn::c::free_pointer_and_set_to_null(&item_entry->category);
    pkmn::c::free_pointer_and_set_to_null(&item_entry->description);
    pkmn::c::free_pointer_and_set_to_null(&item_entry->pocket);
    pkmn::c::free_pointer_and_set_to_null(&item_entry->fling_effect);

    item_entry->cost = 0;
    item_entry->holdable = false;
    item_entry->fling_power = 0;

    return PKMN_ERROR_NONE;
}
