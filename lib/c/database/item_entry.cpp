/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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
) {
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
) {
    std::free(item_entry->name);
    item_entry->name = NULL;

    std::free(item_entry->game);
    item_entry->game = NULL;

    std::free(item_entry->category);
    item_entry->category = NULL;

    std::free(item_entry->description);
    item_entry->description = NULL;

    std::free(item_entry->pocket);
    item_entry->pocket = NULL;

    item_entry->cost = 0;
    item_entry->holdable = false;
    item_entry->fling_power = 0;

    std::free(item_entry->fling_effect);
    item_entry->fling_effect = NULL;

    return PKMN_ERROR_NONE;
}
