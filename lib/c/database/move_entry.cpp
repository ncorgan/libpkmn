/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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

enum pkmn_error pkmn_database_get_move_entry(
    enum pkmn_move move,
    enum pkmn_game game,
    struct pkmn_database_move_entry* p_move_entry_out
)
{
    PKMN_CHECK_NULL_PARAM(p_move_entry_out);

    PKMN_CPP_TO_C(
        pkmn::database::move_entry move_entry_cpp(
                                       static_cast<pkmn::e_move>(move),
                                       static_cast<pkmn::e_game>(game)
                                   );

        pkmn::c::move_entry_cpp_to_c(
            move_entry_cpp,
            p_move_entry_out
        );
    )
}

enum pkmn_error pkmn_database_move_entry_free(
    struct pkmn_database_move_entry* p_move_entry
)
{
    PKMN_CHECK_NULL_PARAM(p_move_entry);

    pkmn::c::free_pointer_and_set_to_null(&p_move_entry->p_name);
    pkmn::c::free_pointer_and_set_to_null(&p_move_entry->p_description);
    pkmn::c::free_pointer_and_set_to_null(&p_move_entry->p_effect);
    pkmn::c::free_pointer_and_set_to_null(&p_move_entry->p_contest_type);
    pkmn::c::free_pointer_and_set_to_null(&p_move_entry->p_contest_effect);
    pkmn::c::free_pointer_and_set_to_null(&p_move_entry->p_super_contest_effect);

    std::memset(p_move_entry, 0, sizeof(*p_move_entry));

    return PKMN_ERROR_NONE;
}
