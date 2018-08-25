/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/natural_gift.h>

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include "common/misc.hpp"

enum pkmn_error pkmn_natural_gift_free(
    struct pkmn_natural_gift* p_natural_gift
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_natural_gift,
                                "p_natural_gift"
                            );
    if(!error)
    {
        pkmn::c::free_pointer_and_set_to_null(&p_natural_gift->p_type);
        p_natural_gift->base_power = 0;
    }

    return error;
}
