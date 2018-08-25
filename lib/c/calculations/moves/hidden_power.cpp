/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/moves/hidden_power.h>

#include <pkmn/calculations/moves/hidden_power.hpp>

enum pkmn_error pkmn_calculations_gen2_hidden_power(
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_special,
    struct pkmn_hidden_power* p_hidden_power_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_hidden_power_out,
                "p_hidden_power_out"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::hidden_power_cpp_to_c(
                pkmn::calculations::gen2_hidden_power(
                    IV_attack, IV_defense,
                    IV_speed, IV_special
                ),
                p_hidden_power_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_calculations_modern_hidden_power(
    int IV_HP,
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_spatk,
    int IV_spdef,
    struct pkmn_hidden_power* p_hidden_power_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_hidden_power_out,
                "p_hidden_power_out"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::hidden_power_cpp_to_c(
                pkmn::calculations::modern_hidden_power(
                    IV_HP, IV_attack,
                    IV_defense, IV_speed,
                    IV_spatk, IV_spdef
                ),
                p_hidden_power_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}
