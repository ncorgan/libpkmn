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

PKMN_C_API enum pkmn_error pkmn_calculations_brine_power(
    int target_current_hp,
    int target_max_hp,
    int* p_brine_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_crush_grip_power(
    int target_current_hp,
    int target_max_hp,
    int generation,
    int* p_crush_grip_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_echoed_voice_powers(
    int* p_powers_buffer,
    size_t buffer_length,
    size_t* p_num_powers_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_electro_ball_power(
    int attacker_speed,
    int target_speed,
    int* p_electro_ball_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_eruption_power(
    int attacker_hp,
    int target_hp,
    int* p_eruption_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_flail_power(
    int attacker_current_hp,
    int attacker_max_hp,
    int* p_flail_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_fling_power(
    const char* p_held_item,
    int* p_fling_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_frustration_power(
    int friendship,
    int* p_frustration_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_fury_cutter_powers(
    int generation,
    int* p_powers_buffer,
    size_t buffer_length,
    size_t* p_num_powers_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_grass_knot_power(
    float target_weight,
    int* p_grass_knot_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_gyro_ball_power(
    int attacker_speed,
    int target_speed,
    int* p_gyro_ball_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_heat_crash_power(
    float attacker_weight,
    float target_weight,
    int* p_heat_crash_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_heavy_slam_power(
    float attacker_weight,
    float target_weight,
    int* p_heavy_slam_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_ice_ball_powers(
    int* p_powers_buffer,
    size_t buffer_length,
    size_t* p_num_powers_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_low_kick_power(
    float target_weight,
    int generation,
    int* p_low_kick_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_power_trip_power(
    int attack_stat_stage,
    int defense_stat_stage,
    int special_attack_stat_stage,
    int special_defense_stat_stage,
    int speed_stat_stage,
    int evasion_stat_stage,
    int accuracy_stat_stage,
    int* p_power_trip_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_punishment_power(
    int attack_stat_stage,
    int defense_stat_stage,
    int special_attack_stat_stage,
    int special_defense_stat_stage,
    int speed_stat_stage,
    int evasion_stat_stage,
    int accuracy_stat_stage,
    int* p_punishment_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_return_power(
    int friendship,
    int* p_return_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_reversal_power(
    int attacker_current_hp,
    int attacker_max_hp,
    int* p_reversal_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_rollout_powers(
    int* p_powers_buffer,
    size_t buffer_length,
    size_t* p_num_powers_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_spit_up_power(
    int stockpile_amount,
    int* p_spit_up_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_stored_power_power(
    int attack_stat_stage,
    int defense_stat_stage,
    int special_attack_stat_stage,
    int special_defense_stat_stage,
    int speed_stat_stage,
    int evasion_stat_stage,
    int accuracy_stat_stage,
    int* p_stored_power_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_triple_kick_powers(
    int* p_powers_buffer,
    size_t buffer_length,
    size_t* p_num_powers_out
);


PKMN_C_API enum pkmn_error pkmn_calculations_trump_card_power(
    int pp_remaining_after_use,
    int* p_trump_card_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_water_spout_power(
    int attacker_current_hp,
    int attacker_max_hp,
    int* p_water_spout_power_out
);

PKMN_C_API enum pkmn_error pkmn_calculations_wring_out_power(
    int target_current_hp,
    int target_max_hp,
    int generation,
    int* p_wring_out_power_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_MODIFIERS_POWER_H */
