/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_MOVES_POWER_HPP
#define PKMN_CALCULATIONS_MOVES_POWER_HPP

#include <pkmn/config.hpp>

#include <pkmn/enums/item.hpp>

#include <algorithm>
#include <vector>

namespace pkmn { namespace calculations {

    /*!
     * @brief Returns the base power of the move Brine, given the target's HP.
     *
     * The move's power increases when the target is at half HP or less.
     *
     * \param target_current_hp The target's current HP
     * \param target_max_hp The target's max HP
     * \throws std::out_of_range If either parameter is zero or negative
     * \throws std::out_of_range If target_current_hp > target_max_hp
     */
    PKMN_API int brine_power(
        int target_current_hp,
        int target_max_hp
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
     * \throws std::out_of_range If generation is not in the range [4-6]
     * \throws std::out_of_range If any parameter is zero or negative
     * \throws std::out_of_range If target_current_hp > target_max_hp
     */
    PKMN_API int crush_grip_power(
        int target_current_hp,
        int target_max_hp,
        int generation
    );

    /*!
     * @brief Returns the base powers of the move Echoed Voice as it's used in
     *        successive turns.
     *
     * The move's power increases by 40 every consecutive turn it's used,
     * capping at 200 on the fifth turn.
     */
    inline std::vector<int> echoed_voice_powers()
    {
        return {40, 80, 120, 160, 200};
    }

    /*!
     * @brief Returns the base power of the move Electro Ball, given the attacker
     *        and defender's Speed stats.
     *
     * The move's power increases the faster the attacker is than the target.
     *
     * \param attacker_speed The attacker's Speed stat
     * \param target_speed The target's speed stat
     * \throws std::out_of_range If either parameter is zero or negative
     */
    PKMN_API int electro_ball_power(
        int attacker_speed,
        int target_speed
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
     * \throws std::out_of_range if either parameter is zero or negative
     * \throws std::out_of_range if attacker_current_hp > attacker_max_hp
     */
    PKMN_API int eruption_power(
        int attacker_current_hp,
        int attacker_max_hp
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
     * \throws std::out_of_range if either parameter is zero or negative
     * \throws std::out_of_range if attacker_current_hp > attacker_max_hp
     */
    PKMN_API int flail_power(
        int attacker_current_hp,
        int attacker_max_hp
    );

    /*!
     * @brief Returns the base power of the move Fling when the attacker holds the
     *        given item.
     *
     * \param held_item The item the Pokémon is holding when using Fling
     * \throws std::invalid_argument If the given item doesn't exist
     */
    PKMN_API int fling_power(
        pkmn::e_item held_item
    );

    /*!
     * @brief Returns the base power of the move Frustration, given the Pokémon's
     *        friendship with its trainer.
     *
     * The lower the friendship value, the higher the move's base power.
     *
     * \param friendship The attacker's friendship with its traing [0-255]
     * \throws std::out_of_range If friendship is outside the range [0-255]
     */
    PKMN_API int frustration_power(
        int friendship
    );

    /*!
     * @brief Returns the base powers of the move Fury Cutter when used in successive
     * turns.
     *
     * In later generations, the base power of Fury Cutter capped in fewer
     * turns.
     *
     * \param generation The generation in which the move is used
     * \throws std::out_of_range If generation is out of the range [2-6]
     */
    PKMN_API std::vector<int> fury_cutter_powers(
        int generation
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
     * \throws std::out_of_range If target_weight is zero or negative
     */
    PKMN_API int grass_knot_power(
        float target_weight
    );

    /*!
     * @brief Returns the base power of the move Gyro Ball, given the attacker
     *        and defender's Speed stats.
     *
     * The move's power increases the faster the attacker is than the target.
     *
     * \param attacker_speed The attacker's Speed stat
     * \param target_speed The target's speed stat
     * \throws std::out_of_range If either parameter is zero or negative
     */
    PKMN_API int gyro_ball_power(
        int attacker_speed,
        int target_speed
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
     * \throws std::out_of_range If target_weight is zero or negative
     */
    PKMN_API int heat_crash_power(
        float attacker_weight,
        float target_weight
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
     * \throws std::out_of_range If target_weight is zero or negative
     */
    PKMN_API int heavy_slam_power(
        float attacker_weight,
        float target_weight
    );

    /*!
     * @brief Returns the base powers of the move Ice Ball as it's used in
     *        successive turns.
     *
     * The move's power doubles every consecutive turn it's used, capping
     * at 480 on the fifth turn.
     *
     * NOTE: This move is the Ice-type equivalent of Rollout.
     */
    inline std::vector<int> ice_ball_powers()
    {
        return {30, 60, 120, 240, 480};
    }

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
     * \throws std::out_of_range If target_weight is zero or negative
     * \throws std::out_of_range If generation is not in the range [1-6]
     */
    PKMN_API int low_kick_power(
        float target_weight,
        int generation
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
     * \throws std::out_of_range If any parameter is not in the range [0-6]
     */
    PKMN_API int power_trip_power(
        int attack_stat_stage,
        int defense_stat_stage,
        int special_attack_stat_stage,
        int special_defense_stat_stage,
        int speed_stat_stage,
        int evasion_stat_stage,
        int accuracy_stat_stage
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
     * \throws std::out_of_range If any parameter is not in the range [0-6]
     */
    PKMN_API int punishment_power(
        int attack_stat_stage,
        int defense_stat_stage,
        int special_attack_stat_stage,
        int special_defense_stat_stage,
        int speed_stat_stage,
        int evasion_stat_stage,
        int accuracy_stat_stage
    );

    /*!
     * @brief Returns the base power of the move Return, given the Pokémon's
     *        friendship with its trainer.
     *
     * The higher the friendship value, the higher the move's base power.
     *
     * \param friendship The attacker's friendship with its traing [0-255]
     * \throws std::out_of_range If friendship is outside the range [0-255]
     */
    PKMN_API int return_power(
        int friendship
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
     * \throws std::out_of_range if either parameter is zero or negative
     * \throws std::out_of_range if attacker_current_hp > attacker_max_hp
     */
    PKMN_API int reversal_power(
        int attacker_current_hp,
        int attacker_max_hp
    );

    /*!
     * @brief Returns the base powers of the move Rollout as it's used in
     *        successive turns.
     *
     * The move's power doubles every consecutive turn it's used, capping
     * at 480 on the fifth turn.
     *
     * NOTE: This move is the Rock-type equivalent of Ice Ball.
     */
    inline std::vector<int> rollout_powers()
    {
        return {30, 60, 120, 240, 480};
    }

    /*!
     * @brief Returns the power of the move Spit Up, based on how many times
     *        the move Stockpile was used beforehand.
     *
     * The more Stockpile has been used before using Spit Up, the higher the
     * power, capping at 300 after three uses.
     *
     * \param stockpile_amount The number of times Stockpile was used before using Spit Up
     * \throws std::out_of_range If stockpile_amount is not in the range [0-3]
     */
    PKMN_API int spit_up_power(
        int stockpile_amount
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
     * \throws std::out_of_range If any parameter is not in the range [0-6]
     */
    PKMN_API int stored_power_power(
        int attack_stat_stage,
        int defense_stat_stage,
        int special_attack_stat_stage,
        int special_defense_stat_stage,
        int speed_stat_stage,
        int evasion_stat_stage,
        int accuracy_stat_stage
    );

    /*!
     * @brief Returns the base powers of the move Triple Kick, which hit in succession
     *        in a single turn.
     */
    inline std::vector<int> triple_kick_powers()
    {
        return {10, 20, 30};
    }

    /*!
     * @brief Returns the base power of the move Trump Card, depending on how much
     *        PP remains after the Pokémon uses it.
     *
     * The less PP there is after using the move, the more damage the move does.
     *
     * \param pp_remaining_after_use The PP remaining after the Pokémon uses the move
     * \throws std::out_of_range If pp_remaining_after_use is not in the range [0-8]
     */
    PKMN_API int trump_card_power(
        int pp_remaining_after_use
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
     * \throws std::out_of_range if either parameter is zero or negative
     * \throws std::out_of_range if attacker_current_hp > attacker_max_hp
     */
    PKMN_API int water_spout_power(
        int attacker_current_hp,
        int attacker_max_hp
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
     * \throws std::out_of_range If generation is not in the range [3-6]
     * \throws std::out_of_range If any parameter is zero or negative
     * \throws std::out_of_range If target_current_hp > target_max_hp
     */
    PKMN_API int wring_out_power(
        int target_current_hp,
        int target_max_hp,
        int generation
    );

}}
#endif /* PKMN_CALCULATIONS_MOVES_POWER_HPP */
