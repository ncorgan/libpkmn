/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/form.h>

#include <pkmn/calculations/form.hpp>

pkmn_error_t pkmn_calculations_gen2_unown_form(
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_special,
    char* form_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(form_out);

    PKMN_CPP_TO_C(
        return pkmn::std_string_to_c_str(
                   pkmn::calculations::gen2_unown_form(
                       IV_attack, IV_defense,
                       IV_speed, IV_special
                   ),
                   form_out,
                   buffer_len
               );
    )
}

pkmn_error_t pkmn_calculations_gen3_unown_form(
    uint32_t personality,
    char* form_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(form_out);

    PKMN_CPP_TO_C(
        return pkmn::std_string_to_c_str(
                   pkmn::calculations::gen3_unown_form(
                       personality
                   ),
                   form_out,
                   buffer_len
               );
    )
}

pkmn_error_t pkmn_calculations_wurmple_becomes_silcoon(
    uint32_t personality,
    bool before_gen5,
    bool* evolves_out
) {
    PKMN_CHECK_NULL_PARAM(evolves_out);

    PKMN_CPP_TO_C(
        *evolves_out = pkmn::calculations::wurmple_becomes_silcoon(
                           personality, before_gen5
                       );
    )
}
