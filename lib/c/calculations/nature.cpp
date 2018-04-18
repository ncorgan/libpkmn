/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
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
    char* nature_out,
    size_t buffer_len,
    size_t* nature_length_out
)
{
    PKMN_CHECK_NULL_PARAM(nature_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_cpp_to_c(
            pkmn::calculations::nature(personality),
            nature_out,
            buffer_len,
            nature_length_out
        );
    )
}
