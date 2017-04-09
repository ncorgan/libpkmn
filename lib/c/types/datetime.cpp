/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/datetime.h>

#include "cpp_to_c.hpp"

pkmn_error_t pkmn_current_datetime(
    pkmn_datetime_t* datetime_out
) {
    PKMN_CHECK_NULL_PARAM(datetime_out);

    PKMN_CPP_TO_C(
        pkmn::pkmn_datetime_cpp_to_c(
            pkmn::current_datetime(),
            datetime_out
        );
    )
}
