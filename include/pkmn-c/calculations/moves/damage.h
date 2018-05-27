/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_MOVES_DAMAGE_H
#define PKMN_C_CALCULATIONS_MOVES_DAMAGE_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Damage calculation.
 *
 * \param attacker_level Attacker level (valid values: [1-255])
 * \param move_base_power The base power of the move being used
 * \param attack_stat The stat being used for attack (Attack or Special Attack)
 * \param defense_stat The stat being used for attack (Defense or Special Defense)
 * \param modifier Further damage modifiers
 * \param damage_out Where to return the final damage amount
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if damage_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if attacker_level is not in the range [1-255]
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if any parameter is negative
 */
PKMN_C_API enum pkmn_error pkmn_calculations_damage(
    int attacker_level,
    int move_base_power,
    int attack_stat,
    int defense_stat,
    float modifier,
    int* p_damage_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_MOVES_DAMAGE_H */
