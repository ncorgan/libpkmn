/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_MOVES_CRITICAL_HIT_H
#define PKMN_C_CALCULATIONS_MOVES_CRITICAL_HIT_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Returns the rate of a move getting a critical hit in Generation I games.
 *
 * Some moves inherently have a higher critical hit rate; using these moves increases
 * the critical hit rate by 8x. Other moves are meant to increase the Pokémon's
 * general critical hit rate. However, due to a bug, it actually lowers the rate by
 * 4x, so that is reflected here.
 *
 * The value returned will be in the range [0.0, 1.0].
 *
 * \param speed the Pokémon's speed (valid values [1-255])
 * \param rate_increased Whether the general critical hit rate was increased
 * \param high_rate_move Whether move inherently has a higher critical hit rate
 * \param critical_hit_chance_out Where to return the critical hit chance
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if critical_hit_chance_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if speed is outside the range [1-255]
 */
PKMN_C_API pkmn_error_t pkmn_calculations_gen1_critical_hit_chance(
    int speed,
    bool rate_increased,
    bool high_rate_move,
    float* critical_hit_chance_out
);

/*!
 * @brief Returns the rate of a move getting a critical hit in Generation II+ games.
 *
 * In Generation II, the chance of a critical hit is based on an in-battle stage
 * value. Moves that increase the Pokémon's general critical hit range increment this
 * value by 1, until the value reaches its peak when the stage value is 4. This value
 * varies between generations.
 *
 * The value returned will be in the range [0.0, 1.0].
 *
 * \param generation Which generation the battle is taking place in
 * \param critical_hit_stage The in-game battle stage value (valid values: 0+)
 * \param critical_hit_chance_out Where to return the critical hit chance
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if critical_hit_chance_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if the generation is not in the range [2-6]
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if the critical hit stage is negative
 */
PKMN_C_API pkmn_error_t pkmn_calculations_critical_hit_chance(
    int generation,
    int critical_hit_stage,
    float* critical_hit_chance_out
);

/*!
 * @brief Returns the damage modifier for when a move gets a critical hit in Generation I.
 *
 * The damage modifier increases with the level.
 *
 * The value returned will be in the range [1.0, 2.0].
 *
 * \param attacker_level The attacker's level (valid values: [1-255])
 * \param critical_hit_modifier_out Where to return the critical hit modifier
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if critical_hit_modifier_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if attacker_level is not in the range [1-255]
 */
PKMN_C_API pkmn_error_t pkmn_calculations_gen1_critical_hit_modifier(
    int attacker_level,
    float* critical_hit_modifier_out
);

/*!
 * @brief Returns the damage modifier for when a move gets a critical hit in Generation II+.
 *
 * The damage modifier is a generation-dependent flat value (1.5 for Generations
 * II-V, 2.0 for Generation VI+).
 *
 * \param generation Which generation the battle is taking place in
 * \param critical_hit_modifier_out Where to return the critical hit modifier
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if critical_hit_modifier_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if generation is not in the range [2-6]
 */
PKMN_C_API pkmn_error_t pkmn_calculations_critical_hit_modifier(
    int generation,
    float* critical_hit_modifier_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_MOVES_CRITICAL_HIT_H */
