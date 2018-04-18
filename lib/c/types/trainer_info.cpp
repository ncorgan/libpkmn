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
    struct pkmn_trainer_info* trainer_info_ptr
)
{
    PKMN_CHECK_NULL_PARAM(trainer_info_ptr);

    pkmn::c::free_pointer_and_set_to_null(&trainer_info_ptr->name);
    trainer_info_ptr->id.id = 0U;
    trainer_info_ptr->gender = PKMN_GENDER_GENDERLESS;

    return PKMN_ERROR_NONE;
}
