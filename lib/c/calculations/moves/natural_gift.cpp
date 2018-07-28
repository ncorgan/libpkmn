/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/moves/natural_gift.h>

#include <pkmn/calculations/moves/natural_gift.hpp>

enum pkmn_error pkmn_calculations_natural_gift_stats(
    enum pkmn_item item,
    int generation,
    struct pkmn_natural_gift* p_natural_gift_stats_out
)
{
    PKMN_CHECK_NULL_PARAM(p_natural_gift_stats_out);

    PKMN_CPP_TO_C(
        pkmn::c::natural_gift_cpp_to_c(
            pkmn::calculations::natural_gift_stats(
                static_cast<pkmn::e_item>(item),
                generation
            ),
            p_natural_gift_stats_out
        );
    )
}
