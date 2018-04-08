/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/moves/hidden_power.h>

#include <pkmn/calculations/moves/hidden_power.hpp>

pkmn_error_t pkmn_calculations_gen2_hidden_power(
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_special,
    struct pkmn_hidden_power* hidden_power_out
)
{
    PKMN_CHECK_NULL_PARAM(hidden_power_out);

    PKMN_CPP_TO_C(
        pkmn::c::hidden_power_cpp_to_c(
            pkmn::calculations::gen2_hidden_power(
                IV_attack, IV_defense,
                IV_speed, IV_special
            ),
            hidden_power_out
        );
    )
}

pkmn_error_t pkmn_calculations_modern_hidden_power(
    int IV_HP,
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_spatk,
    int IV_spdef,
    struct pkmn_hidden_power* hidden_power_out
)
{
    PKMN_CHECK_NULL_PARAM(hidden_power_out);

    PKMN_CPP_TO_C(
        pkmn::c::hidden_power_cpp_to_c(
            pkmn::calculations::modern_hidden_power(
                IV_HP, IV_attack,
                IV_defense, IV_speed,
                IV_spatk, IV_spdef
            ),
            hidden_power_out
        );
    )
}
