/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/moves/power.h>

#include <pkmn/calculations/moves/power.hpp>

pkmn_error_t pkmn_calculations_brine_power(
    int target_current_hp,
    int target_max_hp,
    int* brine_power_out
)
{
    PKMN_CHECK_NULL_PARAM(brine_power_out);

    PKMN_CPP_TO_C(
        *brine_power_out = pkmn::calculations::brine_power(
                               target_current_hp,
                               target_max_hp
                           );
    )
}

pkmn_error_t pkmn_calculations_crush_grip_power(
    int target_current_hp,
    int target_max_hp,
    int generation,
    int* crush_grip_power_out
)
{
    PKMN_CHECK_NULL_PARAM(crush_grip_power_out);

    PKMN_CPP_TO_C(
        *crush_grip_power_out = pkmn::calculations::crush_grip_power(
                                    target_current_hp,
                                    target_max_hp,
                                    generation
                                );
    )
}

pkmn_error_t pkmn_calculations_echoed_voice_powers(
    int* powers_buffer,
    size_t buffer_len,
    size_t* num_powers_out
)
{
    PKMN_CHECK_NULL_PARAM(powers_buffer);
    // num_powers_out can be NULL

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c(
            pkmn::calculations::echoed_voice_powers(),
            powers_buffer,
            buffer_len,
            num_powers_out
        );
    )
}

pkmn_error_t pkmn_calculations_electro_ball_power(
    int attacker_speed,
    int target_speed,
    int* electro_ball_power_out
)
{
    PKMN_CHECK_NULL_PARAM(electro_ball_power_out);

    PKMN_CPP_TO_C(
        *electro_ball_power_out = pkmn::calculations::electro_ball_power(
                                      attacker_speed,
                                      target_speed
                                  );
    )
}

pkmn_error_t pkmn_calculations_eruption_power(
    int attacker_hp,
    int target_hp,
    int* eruption_power_out
)
{
    PKMN_CHECK_NULL_PARAM(eruption_power_out);

    PKMN_CPP_TO_C(
        *eruption_power_out = pkmn::calculations::eruption_power(
                                  attacker_hp,
                                  target_hp
                              );
    )
}

pkmn_error_t pkmn_calculations_fury_cutter_powers(
    int generation,
    int* powers_buffer,
    size_t buffer_len,
    size_t* num_powers_out
)
{
    PKMN_CHECK_NULL_PARAM(powers_buffer);
    // num_powers_out can be NULL

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c(
            pkmn::calculations::fury_cutter_powers(generation),
            powers_buffer,
            buffer_len,
            num_powers_out
        );
    )
}

pkmn_error_t pkmn_calculations_flail_power(
    int attacker_current_hp,
    int attacker_max_hp,
    int* flail_power_out
)
{
    PKMN_CHECK_NULL_PARAM(flail_power_out);

    PKMN_CPP_TO_C(
        *flail_power_out = pkmn::calculations::flail_power(
                               attacker_current_hp,
                               attacker_max_hp
                           );
    )
}

pkmn_error_t pkmn_calculations_fling_power(
    const char* held_item,
    int* fling_power_out
)
{
    PKMN_CHECK_NULL_PARAM(held_item);
    PKMN_CHECK_NULL_PARAM(fling_power_out);

    PKMN_CPP_TO_C(
        *fling_power_out = pkmn::calculations::fling_power(
                               held_item
                           );
    )
}

pkmn_error_t pkmn_calculations_frustration_power(
    int friendship,
    int* frustration_power_out
)
{
    PKMN_CHECK_NULL_PARAM(frustration_power_out);

    PKMN_CPP_TO_C(
        *frustration_power_out = pkmn::calculations::frustration_power(
                                     friendship
                                 );
    )
}

pkmn_error_t pkmn_calculations_grass_knot_power(
    float target_weight,
    int* grass_knot_power_out
)
{
    PKMN_CHECK_NULL_PARAM(grass_knot_power_out);

    PKMN_CPP_TO_C(
        *grass_knot_power_out = pkmn::calculations::grass_knot_power(
                                    target_weight
                                );
    )
}

pkmn_error_t pkmn_calculations_gyro_ball_power(
    int attacker_speed,
    int target_speed,
    int* gyro_ball_power_out
)
{
    PKMN_CHECK_NULL_PARAM(gyro_ball_power_out);

    PKMN_CPP_TO_C(
        *gyro_ball_power_out = pkmn::calculations::gyro_ball_power(
                                   attacker_speed,
                                   target_speed
                               );
    )
}

pkmn_error_t pkmn_calculations_ice_ball_powers(
    int* powers_buffer,
    size_t buffer_len,
    size_t* num_powers_out
)
{
    PKMN_CHECK_NULL_PARAM(powers_buffer);
    // num_powers_out can be NULL

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c(
            pkmn::calculations::ice_ball_powers(),
            powers_buffer,
            buffer_len,
            num_powers_out
        );
    )
}

pkmn_error_t pkmn_calculations_low_kick_power(
    float target_weight,
    int generation,
    int* low_kick_power_out
)
{
    PKMN_CHECK_NULL_PARAM(low_kick_power_out);

    PKMN_CPP_TO_C(
        *low_kick_power_out = pkmn::calculations::low_kick_power(
                                  target_weight,
                                  generation
                              );
    )
}

pkmn_error_t pkmn_calculations_heat_crash_power(
    float attacker_weight,
    float target_weight,
    int* heat_crash_power_out
)
{
    PKMN_CHECK_NULL_PARAM(heat_crash_power_out);

    PKMN_CPP_TO_C(
        *heat_crash_power_out = pkmn::calculations::heat_crash_power(
                                    attacker_weight,
                                    target_weight
                                );
    )
}

pkmn_error_t pkmn_calculations_heavy_slam_power(
    float attacker_weight,
    float target_weight,
    int* heavy_slam_power_out
)
{
    PKMN_CHECK_NULL_PARAM(heavy_slam_power_out);

    PKMN_CPP_TO_C(
        *heavy_slam_power_out = pkmn::calculations::heavy_slam_power(
                                    attacker_weight,
                                    target_weight
                                );
    )
}

pkmn_error_t pkmn_calculations_power_trip_power(
    int attack_stat_stage,
    int defense_stat_stage,
    int special_attack_stat_stage,
    int special_defense_stat_stage,
    int speed_stat_stage,
    int evasion_stat_stage,
    int accuracy_stat_stage,
    int* power_trip_power_out
)
{
    PKMN_CHECK_NULL_PARAM(power_trip_power_out);

    PKMN_CPP_TO_C(
        *power_trip_power_out = pkmn::calculations::power_trip_power(
                                    attack_stat_stage,
                                    defense_stat_stage,
                                    special_attack_stat_stage,
                                    special_defense_stat_stage,
                                    speed_stat_stage,
                                    evasion_stat_stage,
                                    accuracy_stat_stage
                                );
    )
}

pkmn_error_t pkmn_calculations_punishment_power(
    int attack_stat_stage,
    int defense_stat_stage,
    int special_attack_stat_stage,
    int special_defense_stat_stage,
    int speed_stat_stage,
    int evasion_stat_stage,
    int accuracy_stat_stage,
    int* punishment_power_out
)
{
    PKMN_CHECK_NULL_PARAM(punishment_power_out);

    PKMN_CPP_TO_C(
        *punishment_power_out = pkmn::calculations::punishment_power(
                                    attack_stat_stage,
                                    defense_stat_stage,
                                    special_attack_stat_stage,
                                    special_defense_stat_stage,
                                    speed_stat_stage,
                                    evasion_stat_stage,
                                    accuracy_stat_stage
                                );
    )
}

pkmn_error_t pkmn_calculations_return_power(
    int friendship,
    int* return_power_out
)
{
    PKMN_CHECK_NULL_PARAM(return_power_out);

    PKMN_CPP_TO_C(
        *return_power_out = pkmn::calculations::return_power(
                                friendship
                            );
    )
}

pkmn_error_t pkmn_calculations_reversal_power(
    int attacker_current_hp,
    int attacker_max_hp,
    int* reversal_power_out
)
{
    PKMN_CHECK_NULL_PARAM(reversal_power_out);

    PKMN_CPP_TO_C(
        *reversal_power_out = pkmn::calculations::reversal_power(
                                  attacker_current_hp,
                                  attacker_max_hp
                              );
    )
}

pkmn_error_t pkmn_calculations_rollout_powers(
    int* powers_buffer,
    size_t buffer_len,
    size_t* num_powers_out
)
{
    PKMN_CHECK_NULL_PARAM(powers_buffer);
    // num_powers_out can be NULL

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c(
            pkmn::calculations::rollout_powers(),
            powers_buffer,
            buffer_len,
            num_powers_out
        );
    )
}

pkmn_error_t pkmn_calculations_spit_up_power(
    int stockpile_amount,
    int* spit_up_power_out
)
{
    PKMN_CHECK_NULL_PARAM(spit_up_power_out);

    PKMN_CPP_TO_C(
        *spit_up_power_out = pkmn::calculations::spit_up_power(
                                 stockpile_amount
                             );
    )
}

pkmn_error_t pkmn_calculations_stored_power_power(
    int attack_stat_stage,
    int defense_stat_stage,
    int special_attack_stat_stage,
    int special_defense_stat_stage,
    int speed_stat_stage,
    int evasion_stat_stage,
    int accuracy_stat_stage,
    int* stored_power_power_out
)
{
    PKMN_CHECK_NULL_PARAM(stored_power_power_out);

    PKMN_CPP_TO_C(
        *stored_power_power_out = pkmn::calculations::stored_power_power(
                                      attack_stat_stage,
                                      defense_stat_stage,
                                      special_attack_stat_stage,
                                      special_defense_stat_stage,
                                      speed_stat_stage,
                                      evasion_stat_stage,
                                      accuracy_stat_stage
                                  );
    )
}

pkmn_error_t pkmn_calculations_triple_kick_powers(
    int* powers_buffer,
    size_t buffer_len,
    size_t* num_powers_out
)
{
    PKMN_CHECK_NULL_PARAM(powers_buffer);
    // num_powers_out can be NULL

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c(
            pkmn::calculations::triple_kick_powers(),
            powers_buffer,
            buffer_len,
            num_powers_out
        );
    )
}

pkmn_error_t pkmn_calculations_trump_card_power(
    int pp_remaining_after_use,
    int* trump_card_power_out
)
{
    PKMN_CHECK_NULL_PARAM(trump_card_power_out);

    PKMN_CPP_TO_C(
        *trump_card_power_out = pkmn::calculations::trump_card_power(
                                    pp_remaining_after_use
                                );
    )
}

pkmn_error_t pkmn_calculations_water_spout_power(
    int attacker_current_hp,
    int attacker_max_hp,
    int* water_spout_power_out
)
{
    PKMN_CHECK_NULL_PARAM(water_spout_power_out);

    PKMN_CPP_TO_C(
        *water_spout_power_out = pkmn::calculations::water_spout_power(
                                     attacker_current_hp,
                                     attacker_max_hp
                                 );
    )
}

pkmn_error_t pkmn_calculations_wring_out_power(
    int target_current_hp,
    int target_max_hp,
    int generation,
    int* wring_out_power_out
)
{
    PKMN_CHECK_NULL_PARAM(wring_out_power_out);

    PKMN_CPP_TO_C(
        *wring_out_power_out = pkmn::calculations::wring_out_power(
                                   target_current_hp,
                                   target_max_hp,
                                   generation
                               );
    )
}
