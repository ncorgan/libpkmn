/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/stat_list.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

enum pkmn_error pkmn_stat_list_free(
    struct pkmn_stat_list* p_stat_list
)
{
    PKMN_CHECK_NULL_PARAM(p_stat_list);

    enum pkmn_error error = PKMN_ERROR_NONE;

    pkmn::c::free_pointer_and_set_to_null(&p_stat_list->p_stats);
    p_stat_list->num_stats = 0;

    return error;
}
