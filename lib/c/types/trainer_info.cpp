/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/trainer_info.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include "common/misc.hpp"

#include <cstring>

enum pkmn_error pkmn_trainer_info_free(
    struct pkmn_trainer_info* p_trainer_info
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_trainer_info,
                                "p_trainer_info"
                            );
    if(!error)
    {
        pkmn::c::free_pointer_and_set_to_null(&p_trainer_info->p_name);
        std::memset(p_trainer_info, 0, sizeof(*p_trainer_info));
    }

    return error;
}
