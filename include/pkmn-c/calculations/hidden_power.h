/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_HIDDEN_POWER_H
#define PKMN_C_CALCULATIONS_HIDDEN_POWER_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/types/hidden_power.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_calculations_gen2_hidden_power(
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_special,
    pkmn_hidden_power_t* hidden_power_out
);

PKMN_API pkmn_error_t pkmn_calculations_modern_hidden_power(
    int IV_HP,
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_spatk,
    int IV_spdef,
    pkmn_hidden_power_t* hidden_power_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_HIDDEN_POWER_H */
