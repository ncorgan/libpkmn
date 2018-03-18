/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_MODIFIERS_POWER_H
#define PKMN_C_CALCULATIONS_MODIFIERS_POWER_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Returns the base power of the move Brine, given the target's HP.
 *
 * The move's power increases when the target is at half HP or less.
 *
 * \param target_current_hp The target's current HP
 * \param target_max_hp The target's max HP
 * \param brine_power_out Where to return Brine's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if brine_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if either parameter is zero or negative
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if target_current_hp > target_max_hp
 */
PKMN_C_API pkmn_error_t pkmn_calculations_brine_power(
    int target_current_hp,
    int target_max_hp,
    int* brine_power_out
);

/*!
 * @brief Returns the base power of the move Crush Grip, given the target's HP.
 *
 * The move's power is proportional to its remaining HP.
 *
 * NOTE: This is functionally equivalent to Wring Out.
 *
 * \param target_current_hp The target's current HP
 * \param target_max_hp The target's max HP
 * \param generation The generation in which the move is being used
 * \param crush_grip_power_out Where to return Crush Grip's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if crush_grip_power_out
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if generation is not in the range [4-6]
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if any parameter is zero or negative
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if target_current_hp > target_max_hp
 */
PKMN_C_API pkmn_error_t pkmn_calculations_crush_grip_power(
    int target_current_hp,
    int target_max_hp,
    int generation,
    int* crush_grip_power_out
);

/*!
 * @brief Returns the base powers of the move Echoed Voice as it's used in
 *        successive turns.
 *
 * The move's power increases by 40 every consecutive turn it's used,
 * capping at 200 on the fifth turn.
 *
 * \param powers_buffer The buffer in which to return Echoed Voice's base powers
 * \param buffer_length The size of the buffer passed into powers_buffer
 * \param num_powers_out An optional parameter for returning the actual number
 *                       of base powers returned
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if powers_buffer is NULL
 */
PKMN_C_API pkmn_error_t pkmn_calculations_echoed_voice_powers(
    int* powers_buffer,
    size_t buffer_length,
    size_t* num_powers_out
);

/*!
 * @brief Returns the base power of the move Electro Ball, given the attacker
 *        and defender's Speed stats.
 *
 * The move's power increases the faster the attacker is than the target.
 *
 * \param attacker_speed The attacker's Speed stat
 * \param target_speed The target's speed stat
 * \param electro_ball_power_out Where to return Electro Ball's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if electro_ball_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if either parameter is zero or negative
 */
PKMN_C_API pkmn_error_t pkmn_calculations_electro_ball_power(
    int attacker_speed,
    int target_speed,
    int* electro_ball_power_out
);

/*!
 * @brief Returns the base power of the move Eruption, given the attacker's HP.
 *
 * The move's power is proportional to the attacker's current HP relative
 * to their max HP.
 *
 * NOTE: This is the Fire-type equivalent of Water Spout.
 *
 * \param attacker_current_hp The attacker's current HP
 * \param attacker_max_hp The attacker's max HP
 * \param eruption_out Where to return Eruption's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if eruption_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if either parameter is zero or negative
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if attacker_current_hp > attacker_max_hp
 */
PKMN_C_API pkmn_error_t pkmn_calculations_eruption_power(
    int attacker_current_hp,
    int attacker_max_hp,
    int* eruption_out
);

/*!
 * @brief Returns the base power of the move Flail, given the attacker's HP.
 *
 * The lower the attacker's HP, the higher the move's base power.
 *
 * NOTE: This is the Normal-type equivalent of Reversal.
 *
 * \param attacker_current_hp The attacker's current HP
 * \param attacker_max_hp The attacker's max HP
 * \param flail_power_out Where to return Flail's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if flail_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if either parameter is zero or negative
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if attacker_current_hp > attacker_max_hp
 */
PKMN_C_API pkmn_error_t pkmn_calculations_flail_power(
    int attacker_current_hp,
    int attacker_max_hp,
    int* flail_power_out
);

/*!
 * @brief Returns the base power of the move Fling when the attacker holds the
 *        given item.
 *
 * \param held_item The item the Pokémon is holding when using Fling
 * \param fling_power_out Where to return Fling's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if iether parameter is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the given item doesn't exist
 */
PKMN_C_API pkmn_error_t pkmn_calculations_fling_power(
    const char* held_item,
    int* fling_power_out
);

/*!
 * @brief Returns the base power of the move Frustration, given the Pokémon's
 *        friendship with its trainer.
 *
 * The lower the friendship value, the higher the move's base power.
 *
 * \param friendship The attacker's friendship with its traing [0-255]
 * \param frustration_power_out Where to return Frustration's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if frustration_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if friendship is outside the range [0-255]
 */
PKMN_C_API pkmn_error_t pkmn_calculations_frustration_power(
    int friendship,
    int* frustration_power_out
);

/*!
 * @brief Returns the base powers of the move Fury Cutter when used in successive
 * turns.
 *
 * In later generations, the base power of Fury Cutter capped in fewer
 * turns.
 *
 * \param generation The generation in which the move is used
 * \param powers_buffer The buffer in which to return Fury Cutter's base powers
 * \param buffer_length The length of the buffer passed into powers_buffer
 * \param num_powers_out An optional parameter for returning the actual number
 *                       of base powers returned
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if powers_buffer is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if generation is out of the range [2-6]
 */
PKMN_C_API pkmn_error_t pkmn_calculations_fury_cutter_powers(
    int generation,
    int* powers_buffer,
    size_t buffer_length,
    size_t* num_powers_out
);

/*!
 * @brief Returns the base power of the move Grass Knot, given the target's
 *        weight.
 *
 * The heavier the target, the more damage the move does.
 *
 * NOTE: This move is the Grass-type equivalent of a Generation III+
 *       Low Kick.
 *
 * \param target_weight The target Pokémon's weight in kilograms.
 * \param grass_knot_power_out Where to return Grass Knot's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if grass_knot_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if target_weight is zero or negative
 */
PKMN_C_API pkmn_error_t pkmn_calculations_grass_knot_power(
    float target_weight,
    int* grass_knot_power_out
);

/*!
 * @brief Returns the base power of the move Gyro Ball, given the attacker
 *        and defender's Speed stats.
 *
 * The move's power increases the faster the attacker is than the target.
 *
 * \param attacker_speed The attacker's Speed stat
 * \param target_speed The target's speed stat
 * \param gyro_ball_power_out Where to return Gyro Ball's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if gyro_ball_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if either parameter is zero or negative
 */
PKMN_C_API pkmn_error_t pkmn_calculations_gyro_ball_power(
    int attacker_speed,
    int target_speed,
    int* gyro_ball_power_out
);

/*!
 * @brief Returns the base power of the move Heat Crash, given the attacker
 *        and target's weights.
 *
 * The lighter the attacker is compared to the target, the more damage
 * the move does.
 *
 * NOTE: This move is the Fire-type equivalent of Heavy Slam.
 *
 * \param attacker_weight The attacker Pokémon's weight in kilograms.
 * \param target_weight The target Pokémon's weight in kilograms.
 * \param heat_crash_power_out Where to return Heat Crash's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if heat_crash_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if target_weight is zero or negative
 */
PKMN_C_API pkmn_error_t pkmn_calculations_heat_crash_power(
    float attacker_weight,
    float target_weight,
    int* heat_crash_power_out
);

/*!
 * @brief Returns the base power of the move Heavy Slam, given the attacker
 *        and target's weights.
 *
 * The lighter the attacker is compared to the target, the more damage
 * the move does.
 *
 * NOTE: This move is the Steel-type equivalent of Heat Crash.
 *
 * \param attacker_weight The attacker Pokémon's weight in kilograms.
 * \param target_weight The target Pokémon's weight in kilograms.
 * \param heavy_slam_power_out Where to return Heavy Slam's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if heavy_slam_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if target_weight is zero or negative
 */
PKMN_C_API pkmn_error_t pkmn_calculations_heavy_slam_power(
    float attacker_weight,
    float target_weight,
    int* heavy_slam_power_out
);

/*!
 * @brief Returns the base powers of the move Ice Ball as it's used in
 *        successive turns.
 *
 * The move's power doubles every consecutive turn it's used, capping
 * at 480 on the fifth turn.
 *
 * NOTE: This move is the Ice-type equivalent of Rollout.
 *
 * \param powers_buffer The buffer in which to return Ice Ball's base powers
 * \param buffer_length The length of the buffer passed into powers_buffer
 * \param num_powers_out An optional parameter for returning the actual number
 *                       of base powers returned
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if powers_buffer is NULL
 */
PKMN_C_API pkmn_error_t pkmn_calculations_ice_ball_powers(
    int* powers_buffer,
    size_t buffer_length,
    size_t* num_powers_out
);

/*!
 * @brief Returns the base power of the move Low Kick, given the target's
 *        weight.
 *
 * The heavier the target, the more damage the move does.
 *
 * NOTE: In Generation III+, this move is the Fighting-type equivalent
 *       of Grass Knot.
 *
 * \param target_weight The target Pokémon's weight in kilograms.
 * \param generation The generation in which the move is used
 * \param low_kick_power_out Where to return Low Kick's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if low_kick_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if target_weight is zero or negative
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if generation is not in the range [1-6]
 */
PKMN_C_API pkmn_error_t pkmn_calculations_low_kick_power(
    float target_weight,
    int generation,
    int* low_kick_power_out
);

/*!
 * @brief Returns the base power of the move Power Trip, given how many
 *        of the attacker's stats have been raised.
 *
 * This move increases in power the more the attacker's stats have
 * been ranged. Each stat can be raised up to six times.
 *
 * NOTE: This type is the Dark-type equivalent of Stored Power.
 *
 * \param attack_stat_stage How many times the Attack stat has been raised in-battle
 * \param defense_stat_stage How many times the Defense stat has been raised in-battle
 * \param special_attack_stat_stage How many times the Special Attack stat has been raised in-battle
 * \param special_defense_stat_stage How many times the Special Defense stat has been raised in-battle
 * \param speed_stat_stage How many times the Speed stat has been raised in-battle
 * \param evasion_stat_stage How many times the Evasion stat has been raised in-battle
 * \param accuracy_stat_stage How many times the Accuracy stat has been raised in-battle
 * \param power_trip_power_out Where to return Power Trip's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if power_trip_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if any parameter is not in the range [0-6]
 */
PKMN_C_API pkmn_error_t pkmn_calculations_power_trip_power(
    int attack_stat_stage,
    int defense_stat_stage,
    int special_attack_stat_stage,
    int special_defense_stat_stage,
    int speed_stat_stage,
    int evasion_stat_stage,
    int accuracy_stat_stage,
    int* power_trip_power_out
);

/*!
 * @brief Returns the base power of the move Punishment, given how many
 *        of the attacker's stats have been raised.
 *
 * This move increases in power the more the attacker's stats have
 * been ranged. Each stat can be raised up to six times.
 *
 * \param attack_stat_stage How many times the Attack stat has been raised in-battle
 * \param defense_stat_stage How many times the Defense stat has been raised in-battle
 * \param special_attack_stat_stage How many times the Special Attack stat has been raised in-battle
 * \param special_defense_stat_stage How many times the Special Defense stat has been raised in-battle
 * \param speed_stat_stage How many times the Speed stat has been raised in-battle
 * \param evasion_stat_stage How many times the Evasion stat has been raised in-battle
 * \param accuracy_stat_stage How many times the Accuracy stat has been raised in-battle
 * \param punishment_power_out Where to return Punishment's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if punishment_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if any parameter is not in the range [0-6]
 */
PKMN_C_API pkmn_error_t pkmn_calculations_punishment_power(
    int attack_stat_stage,
    int defense_stat_stage,
    int special_attack_stat_stage,
    int special_defense_stat_stage,
    int speed_stat_stage,
    int evasion_stat_stage,
    int accuracy_stat_stage,
    int* punishment_power_out
);

/*!
 * @brief Returns the base power of the move Return, given the Pokémon's
 *        friendship with its trainer.
 *
 * The higher the friendship value, the higher the move's base power.
 *
 * \param friendship The attacker's friendship with its traing [0-255]
 * \param return_power_out Where to return Return's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if return_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if friendship is outside the range [0-255]
 */
PKMN_C_API pkmn_error_t pkmn_calculations_return_power(
    int friendship,
    int* return_power_out
);

/*!
 * @brief Returns the base power of the move Reversal, given the attacker's HP.
 *
 * The lower the attacker's HP, the higher the move's base power.
 *
 * NOTE: This is the Fighting-type equivalent of Flail.
 *
 * \param attacker_current_hp The attacker's current HP
 * \param attacker_max_hp The attacker's max HP
 * \param reversal_power_out Where to return Reversal's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if reversal_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if either parameter is zero or negative
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if attacker_current_hp > attacker_max_hp
 */
PKMN_C_API pkmn_error_t pkmn_calculations_reversal_power(
    int attacker_current_hp,
    int attacker_max_hp,
    int* reversal_power_out
);

/*!
 * @brief Returns the base powers of the move Rollout as it's used in
 *        successive turns.
 *
 * The move's power doubles every consecutive turn it's used, capping
 * at 480 on the fifth turn.
 *
 * NOTE: This move is the Rock-type equivalent of Ice Ball.
 *
 * \param powers_buffer The buffer in which to return Rollout's base powers
 * \param buffer_length The length of the buffer passed into powers_buffer
 * \param num_powers_out An optional parameter for returning the actual number
 *                       of base powers returned
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if powers_buffer is NULL
 */
PKMN_C_API pkmn_error_t pkmn_calculations_rollout_powers(
    int* powers_buffer,
    size_t buffer_length,
    size_t* num_powers_out
);

/*!
 * @brief Returns the power of the move Spit Up, based on how many times
 *        the move Stockpile was used beforehand.
 *
 * The more Stockpile has been used before using Spit Up, the higher the
 * power, capping at 300 after three uses.
 *
 * \param stockpile_amount The number of times Stockpile was used before using Spit Up
 * \param spit_up_power_out Where to return Spit Up's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if spit_up_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if stockpile_amount is not in the range [0-3]
 */
PKMN_C_API pkmn_error_t pkmn_calculations_spit_up_power(
    int stockpile_amount,
    int* spit_up_power_out
);

/*!
 * @brief Returns the base power of the move Stored Power, given how many
 *        of the attacker's stats have been raised.
 *
 * This move increases in power the more the attacker's stats have
 * been ranged. Each stat can be raised up to six times.
 *
 * NOTE: This type is the Psychic-type equivalent of Power Trip.
 *
 * \param attack_stat_stage How many times the Attack stat has been raised in-battle
 * \param defense_stat_stage How many times the Defense stat has been raised in-battle
 * \param special_attack_stat_stage How many times the Special Attack stat has been raised in-battle
 * \param special_defense_stat_stage How many times the Special Defense stat has been raised in-battle
 * \param speed_stat_stage How many times the Speed stat has been raised in-battle
 * \param evasion_stat_stage How many times the Evasion stat has been raised in-battle
 * \param accuracy_stat_stage How many times the Accuracy stat has been raised in-battle
 * \param stored_power_power_out Where to return Stored Power's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if stored_power_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if any parameter is not in the range [0-6]
 */
PKMN_C_API pkmn_error_t pkmn_calculations_stored_power_power(
    int attack_stat_stage,
    int defense_stat_stage,
    int special_attack_stat_stage,
    int special_defense_stat_stage,
    int speed_stat_stage,
    int evasion_stat_stage,
    int accuracy_stat_stage,
    int* stored_power_power_out
);

/*!
 * @brief Returns the base powers of the move Triple Kick, which hit in succession
 *        in a single turn.
 *
 * \param powers_buffer The buffer in which to return Triple Kick's base powers
 * \param buffer_length The length of the buffer passed into powers_buffer
 * \param num_powers_out An optional parameter for returning the actual number
 *                       of base powers returned
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if powers_buffer is NULL
 */
PKMN_C_API pkmn_error_t pkmn_calculations_triple_kick_powers(
    int* powers_buffer,
    size_t buffer_length,
    size_t* num_powers_out
);

/*!
 * @brief Returns the base power of the move Trump Card, depending on how much
 *        PP remains after the Pokémon uses it.
 *
 * The less PP there is after using the move, the more damage the move does.
 *
 * \param pp_remaining_after_use The PP remaining after the Pokémon uses the move
 * \param trump_card_power_out Where to return Trump Card's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if trump_card_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if pp_remaining_after_use is not in the range [0-8]
 */
PKMN_C_API pkmn_error_t pkmn_calculations_trump_card_power(
    int pp_remaining_after_use,
    int* trump_card_power_out
);

/*!
 * @brief Returns the base power of the move Water Spout, given the attacker's HP.
 *
 * The move's power is proportional to the attacker's current HP relative
 * to their max HP.
 *
 * NOTE: This is the Water-type equivalent of Eruption.
 *
 * \param attacker_current_hp The attacker's current HP
 * \param attacker_max_hp The attacker's max HP
 * \param water_spout_power_out Where to return Water Spout's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if water_spout_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if either parameter is zero or negative
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if attacker_current_hp > attacker_max_hp
 */
PKMN_C_API pkmn_error_t pkmn_calculations_water_spout_power(
    int attacker_current_hp,
    int attacker_max_hp,
    int* water_spout_power_out
);

/*!
 * @brief Returns the base power of the move Wring Out, given the target's HP.
 *
 * The move's power is proportional to its remaining HP.
 *
 * NOTE: This is functionally equivalent to Crush Grip.
 *
 * \param target_current_hp The target's current HP
 * \param target_max_hp The target's max HP
 * \param generation The generation in which the move is being used
 * \param wring_out_power_out Where to return Wring Out's base power
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if wring_out_power_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if generation is not in the range [3-6]
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if any parameter is zero or negative
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if target_current_hp > target_max_hp
 */
PKMN_C_API pkmn_error_t pkmn_calculations_wring_out_power(
    int target_current_hp,
    int target_max_hp,
    int generation,
    int* wring_out_power_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_MODIFIERS_POWER_H */
