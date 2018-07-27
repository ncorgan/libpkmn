/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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

enum pkmn_error pkmn_database_get_item_entry(
    enum pkmn_item item,
    enum pkmn_game game,
    struct pkmn_database_item_entry* p_item_entry_out
)
{
    PKMN_CHECK_NULL_PARAM(p_item_entry_out);

    PKMN_CPP_TO_C(
        pkmn::database::item_entry item_entry_cpp(
                                       static_cast<pkmn::e_item>(item),
                                       static_cast<pkmn::e_game>(game)
                                   );
        pkmn::c::item_entry_cpp_to_c(
            item_entry_cpp,
            p_item_entry_out
        );
    )
}

enum pkmn_error pkmn_database_item_entry_free(
    struct pkmn_database_item_entry* p_item_entry
)
{
    PKMN_CHECK_NULL_PARAM(p_item_entry);

    pkmn::c::free_pointer_and_set_to_null(&p_item_entry->p_name);
    pkmn::c::free_pointer_and_set_to_null(&p_item_entry->p_category);
    pkmn::c::free_pointer_and_set_to_null(&p_item_entry->p_description);
    pkmn::c::free_pointer_and_set_to_null(&p_item_entry->p_pocket);
    pkmn::c::free_pointer_and_set_to_null(&p_item_entry->p_fling_effect);

    std::memset(
        p_item_entry,
        0,
        sizeof(*p_item_entry)
    );

    return PKMN_ERROR_NONE;
}
