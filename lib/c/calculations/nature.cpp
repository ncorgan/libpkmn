/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/nature.h>

#include <pkmn/calculations/nature.hpp>

enum pkmn_error pkmn_calculations_nature(
    uint32_t personality,
    enum pkmn_nature* p_nature_out
)
{
    PKMN_CHECK_NULL_PARAM(p_nature_out);

    PKMN_CPP_TO_C(
        *p_nature_out = static_cast<enum pkmn_nature>(
                            pkmn::calculations::nature(personality)
                        );
    )
}
