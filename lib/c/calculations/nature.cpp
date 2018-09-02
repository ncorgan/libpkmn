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
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_nature_out,
                                "p_nature_out"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            *p_nature_out = static_cast<enum pkmn_nature>(
                                pkmn::calculations::nature(personality)
                            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}
