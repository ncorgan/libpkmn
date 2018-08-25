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
    const char* p_item_name,
    int generation,
    struct pkmn_natural_gift* p_natural_gift_stats_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_item_name,
                "p_item_name"
            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_natural_gift_stats_out,
                    "p_natural_gift_stats_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::natural_gift_cpp_to_c(
                pkmn::calculations::natural_gift_stats(
                    p_item_name,
                    generation
                ),
                p_natural_gift_stats_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}
