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
    char* p_nature_out,
    size_t buffer_len,
    size_t* p_nature_length_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_nature_out,
                "p_nature_out"
            );
    if(!error)
    {
        pkmn::c::string_cpp_to_c(
            pkmn::calculations::nature(personality),
            p_nature_out,
            buffer_len,
            p_nature_length_out
        );
    }

    return error;
}
