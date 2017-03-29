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

pkmn_error_t pkmn_calculations_gen3_gen4_nature(
    uint32_t personality,
    char* nature_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(nature_out);

    PKMN_CPP_TO_C(
        return pkmn::std_string_to_c_str(
                   pkmn::calculations::gen3_gen4_nature(
                       personality
                   ),
                   nature_out,
                   buffer_len
               );
    )
}
