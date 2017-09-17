/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/moves/natural_gift.h>

#include <pkmn/calculations/moves/natural_gift.hpp>

pkmn_error_t pkmn_calculations_natural_gift_stats(
    const char* item_name,
    const char* game,
    pkmn_natural_gift_t* natural_gift_stats_out
)
{
    PKMN_CHECK_NULL_PARAM(item_name);
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(natural_gift_stats_out);

    PKMN_CPP_TO_C(
        pkmn::pkmn_natural_gift_cpp_to_c(
            pkmn::calculations::natural_gift_stats(
                item_name,
                game
            ),
            natural_gift_stats_out
        );
    )
}
