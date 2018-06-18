/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/trainer_info.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

enum pkmn_error pkmn_trainer_info_free(
    struct pkmn_trainer_info* p_trainer_info
)
{
    PKMN_CHECK_NULL_PARAM(p_trainer_info);

    pkmn::c::free_pointer_and_set_to_null(&p_trainer_info->p_name);
    p_trainer_info->id.id = 0U;
    p_trainer_info->gender = PKMN_GENDER_NONE;

    return PKMN_ERROR_NONE;
}
