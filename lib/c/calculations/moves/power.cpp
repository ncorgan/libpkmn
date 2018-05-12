/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/moves/power.h>

#include <pkmn/calculations/moves/power.hpp>

enum pkmn_error pkmn_calculations_brine_power(
    int target_current_hp,
    int target_max_hp,
    int* p_brine_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_brine_power_out);

    PKMN_CPP_TO_C(
        *p_brine_power_out = pkmn::calculations::brine_power(
                                 target_current_hp,
                                 target_max_hp
                             );
    )
}

enum pkmn_error pkmn_calculations_crush_grip_power(
    int target_current_hp,
    int target_max_hp,
    int generation,
    int* p_crush_grip_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_crush_grip_power_out);

    PKMN_CPP_TO_C(
        *p_crush_grip_power_out = pkmn::calculations::crush_grip_power(
                                      target_current_hp,
                                      target_max_hp,
                                      generation
                                  );
    )
}

enum pkmn_error pkmn_calculations_echoed_voice_powers(
    int* p_powers_buffer,
    size_t buffer_len,
    size_t* p_num_powers_out
)
{
    PKMN_CHECK_NULL_PARAM(p_powers_buffer);
    // p_num_powers_out can be NULL

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c(
            pkmn::calculations::echoed_voice_powers(),
            p_powers_buffer,
            buffer_len,
            p_num_powers_out
        );
    )
}

enum pkmn_error pkmn_calculations_electro_ball_power(
    int attacker_speed,
    int target_speed,
    int* p_electro_ball_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_electro_ball_power_out);

    PKMN_CPP_TO_C(
        *p_electro_ball_power_out = pkmn::calculations::electro_ball_power(
                                        attacker_speed,
                                        target_speed
                                    );
    )
}

enum pkmn_error pkmn_calculations_eruption_power(
    int attacker_hp,
    int target_hp,
    int* p_eruption_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_eruption_power_out);

    PKMN_CPP_TO_C(
        *p_eruption_power_out = pkmn::calculations::eruption_power(
                                    attacker_hp,
                                    target_hp
                                );
    )
}

enum pkmn_error pkmn_calculations_fury_cutter_powers(
    int generation,
    int* p_powers_buffer,
    size_t buffer_len,
    size_t* p_num_powers_out
)
{
    PKMN_CHECK_NULL_PARAM(p_powers_buffer);
    // num_powers_out can be NULL

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c(
            pkmn::calculations::fury_cutter_powers(generation),
            p_powers_buffer,
            buffer_len,
            p_num_powers_out
        );
    )
}

enum pkmn_error pkmn_calculations_flail_power(
    int attacker_current_hp,
    int attacker_max_hp,
    int* p_flail_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_flail_power_out);

    PKMN_CPP_TO_C(
        *p_flail_power_out = pkmn::calculations::flail_power(
                                 attacker_current_hp,
                                 attacker_max_hp
                             );
    )
}

enum pkmn_error pkmn_calculations_fling_power(
    const char* p_held_item,
    int* p_fling_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_held_item);
    PKMN_CHECK_NULL_PARAM(p_fling_power_out);

    PKMN_CPP_TO_C(
        *p_fling_power_out = pkmn::calculations::fling_power(
                                 p_held_item
                             );
    )
}

enum pkmn_error pkmn_calculations_frustration_power(
    int friendship,
    int* p_frustration_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_frustration_power_out);

    PKMN_CPP_TO_C(
        *p_frustration_power_out = pkmn::calculations::frustration_power(
                                       friendship
                                   );
    )
}

enum pkmn_error pkmn_calculations_grass_knot_power(
    float target_weight,
    int* p_grass_knot_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_grass_knot_power_out);

    PKMN_CPP_TO_C(
        *p_grass_knot_power_out = pkmn::calculations::grass_knot_power(
                                      target_weight
                                  );
    )
}

enum pkmn_error pkmn_calculations_gyro_ball_power(
    int attacker_speed,
    int target_speed,
    int* p_gyro_ball_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_gyro_ball_power_out);

    PKMN_CPP_TO_C(
        *p_gyro_ball_power_out = pkmn::calculations::gyro_ball_power(
                                     attacker_speed,
                                     target_speed
                                 );
    )
}

enum pkmn_error pkmn_calculations_ice_ball_powers(
    int* p_powers_buffer,
    size_t buffer_len,
    size_t* p_num_powers_out
)
{
    PKMN_CHECK_NULL_PARAM(p_powers_buffer);
    // p_num_powers_out can be NULL

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c(
            pkmn::calculations::ice_ball_powers(),
            p_powers_buffer,
            buffer_len,
            p_num_powers_out
        );
    )
}

enum pkmn_error pkmn_calculations_low_kick_power(
    float target_weight,
    int generation,
    int* p_low_kick_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_low_kick_power_out);

    PKMN_CPP_TO_C(
        *p_low_kick_power_out = pkmn::calculations::low_kick_power(
                                    target_weight,
                                    generation
                                );
    )
}

enum pkmn_error pkmn_calculations_heat_crash_power(
    float attacker_weight,
    float target_weight,
    int* p_heat_crash_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_heat_crash_power_out);

    PKMN_CPP_TO_C(
        *p_heat_crash_power_out = pkmn::calculations::heat_crash_power(
                                      attacker_weight,
                                      target_weight
                                  );
    )
}

enum pkmn_error pkmn_calculations_heavy_slam_power(
    float attacker_weight,
    float target_weight,
    int* p_heavy_slam_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_heavy_slam_power_out);

    PKMN_CPP_TO_C(
        *p_heavy_slam_power_out = pkmn::calculations::heavy_slam_power(
                                      attacker_weight,
                                      target_weight
                                  );
    )
}

enum pkmn_error pkmn_calculations_power_trip_power(
    int attack_stat_stage,
    int defense_stat_stage,
    int special_attack_stat_stage,
    int special_defense_stat_stage,
    int speed_stat_stage,
    int evasion_stat_stage,
    int accuracy_stat_stage,
    int* p_power_trip_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_power_trip_power_out);

    PKMN_CPP_TO_C(
        *p_power_trip_power_out = pkmn::calculations::power_trip_power(
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

enum pkmn_error pkmn_calculations_punishment_power(
    int attack_stat_stage,
    int defense_stat_stage,
    int special_attack_stat_stage,
    int special_defense_stat_stage,
    int speed_stat_stage,
    int evasion_stat_stage,
    int accuracy_stat_stage,
    int* p_punishment_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_punishment_power_out);

    PKMN_CPP_TO_C(
        *p_punishment_power_out = pkmn::calculations::punishment_power(
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

enum pkmn_error pkmn_calculations_return_power(
    int friendship,
    int* p_return_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_return_power_out);

    PKMN_CPP_TO_C(
        *p_return_power_out = pkmn::calculations::return_power(
                                  friendship
                              );
    )
}

enum pkmn_error pkmn_calculations_reversal_power(
    int attacker_current_hp,
    int attacker_max_hp,
    int* p_reversal_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_reversal_power_out);

    PKMN_CPP_TO_C(
        *p_reversal_power_out = pkmn::calculations::reversal_power(
                                    attacker_current_hp,
                                    attacker_max_hp
                                );
    )
}

enum pkmn_error pkmn_calculations_rollout_powers(
    int* p_powers_buffer,
    size_t buffer_len,
    size_t* p_num_powers_out
)
{
    PKMN_CHECK_NULL_PARAM(p_powers_buffer);
    // p_num_powers_out can be NULL

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c(
            pkmn::calculations::rollout_powers(),
            p_powers_buffer,
            buffer_len,
            p_num_powers_out
        );
    )
}

enum pkmn_error pkmn_calculations_spit_up_power(
    int stockpile_amount,
    int* p_spit_up_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_spit_up_power_out);

    PKMN_CPP_TO_C(
        *p_spit_up_power_out = pkmn::calculations::spit_up_power(
                                   stockpile_amount
                               );
    )
}

enum pkmn_error pkmn_calculations_stored_power_power(
    int attack_stat_stage,
    int defense_stat_stage,
    int special_attack_stat_stage,
    int special_defense_stat_stage,
    int speed_stat_stage,
    int evasion_stat_stage,
    int accuracy_stat_stage,
    int* p_stored_power_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_stored_power_power_out);

    PKMN_CPP_TO_C(
        *p_stored_power_power_out = pkmn::calculations::stored_power_power(
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

enum pkmn_error pkmn_calculations_triple_kick_powers(
    int* p_powers_buffer,
    size_t buffer_len,
    size_t* p_num_powers_out
)
{
    PKMN_CHECK_NULL_PARAM(p_powers_buffer);
    // p_num_powers_out can be NULL

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c(
            pkmn::calculations::triple_kick_powers(),
            p_powers_buffer,
            buffer_len,
            p_num_powers_out
        );
    )
}

enum pkmn_error pkmn_calculations_trump_card_power(
    int pp_remaining_after_use,
    int* p_trump_card_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_trump_card_power_out);

    PKMN_CPP_TO_C(
        *p_trump_card_power_out = pkmn::calculations::trump_card_power(
                                      pp_remaining_after_use
                                  );
    )
}

enum pkmn_error pkmn_calculations_water_spout_power(
    int attacker_current_hp,
    int attacker_max_hp,
    int* p_water_spout_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_water_spout_power_out);

    PKMN_CPP_TO_C(
        *p_water_spout_power_out = pkmn::calculations::water_spout_power(
                                       attacker_current_hp,
                                       attacker_max_hp
                                   );
    )
}

enum pkmn_error pkmn_calculations_wring_out_power(
    int target_current_hp,
    int target_max_hp,
    int generation,
    int* p_wring_out_power_out
)
{
    PKMN_CHECK_NULL_PARAM(p_wring_out_power_out);

    PKMN_CPP_TO_C(
        *p_wring_out_power_out = pkmn::calculations::wring_out_power(
                                     target_current_hp,
                                     target_max_hp,
                                     generation
                                 );
    )
}
