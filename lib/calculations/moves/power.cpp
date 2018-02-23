/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/exception.hpp>
#include <pkmn/calculations/moves/power.hpp>

#include "exception_internal.hpp"
#include "database/database_common.hpp"

namespace pkmn { namespace calculations {

    int brine_power(
        int target_current_hp,
        int target_max_hp
    )
    {
        pkmn::enforce_comparator(
            "Target current HP",
            target_current_hp,
            0,
            pkmn::value_comparator::GT
        );
        pkmn::enforce_comparator(
            "Target max HP",
            target_max_hp,
            0,
            pkmn::value_comparator::GT
        );
        pkmn::enforce_bounds(
            "Target current HP",
            target_current_hp,
            0,
            target_max_hp
        );

        float hp_percentage = float(target_current_hp) / float(target_max_hp);
        return (hp_percentage <= 0.5f) ? 130 : 65;
    }

    int crush_grip_power(
        int target_current_hp,
        int target_max_hp,
        int generation
    )
    {
        pkmn::enforce_comparator(
            "Target max HP",
            target_max_hp,
            1,
            pkmn::value_comparator::GT
        );
        pkmn::enforce_bounds(
            "Target current HP",
            target_current_hp,
            1,
            target_max_hp
        );
        pkmn::enforce_bounds(
            "Generation",
            generation,
            4,
            6
        );

        float hp_percentage = float(target_current_hp) / float(target_max_hp);
        int ret = int(120.0f * hp_percentage);

        if(generation == 4)
        {
            ++ret;
        }
        else
        {
            ret = std::max<int>(1, ret);
        }

        return ret;
    }

    int electro_ball_power(
        int attacker_speed,
        int target_speed
    )
    {
        pkmn::enforce_comparator(
            "Attacker speed",
            attacker_speed,
            0,
            pkmn::value_comparator::GT
        );
        pkmn::enforce_comparator(
            "Target speed",
            target_speed,
            0,
            pkmn::value_comparator::GT
        );

        float speed_percentage = float(target_speed) / float(attacker_speed);
        int ret = 0;

        if(speed_percentage <= 0.25f)
        {
            ret = 150;
        }
        else if(speed_percentage <= 0.3333f)
        {
            ret = 120;
        }
        else if(speed_percentage <= 0.5f)
        {
            ret = 80;
        }
        else if(speed_percentage <= 1.0f)
        {
            ret = 60;
        }
        else
        {
            ret = 40;
        }

        return ret;
    }

    int eruption_power(
        int attacker_current_hp,
        int attacker_max_hp
    )
    {
        pkmn::enforce_comparator(
            "Attacker current HP",
            attacker_current_hp,
            0,
            pkmn::value_comparator::GT
        );
        pkmn::enforce_comparator(
            "Attacker max HP",
            attacker_max_hp,
            0,
            pkmn::value_comparator::GT
        );
        pkmn::enforce_bounds(
            "Attacker current HP",
            attacker_current_hp,
            0,
            attacker_max_hp
        );

        return std::max<int>(
                   1,
                   int(150.0f * (float(attacker_current_hp) / float(attacker_max_hp)))
               );
    }

    int flail_power(
        int attacker_current_hp,
        int attacker_max_hp
    )
    {
        pkmn::enforce_comparator(
            "Attacker current HP",
            attacker_current_hp,
            0,
            pkmn::value_comparator::GT
        );
        pkmn::enforce_comparator(
            "Attacker max HP",
            attacker_max_hp,
            0,
            pkmn::value_comparator::GT
        );
        pkmn::enforce_bounds(
            "Attacker current HP",
            attacker_current_hp,
            0,
            attacker_max_hp
        );

        float hp_percentage = float(attacker_current_hp) / float(attacker_max_hp);
        int ret = 0;

        if(pkmn::fp_compare_less(hp_percentage, 0.0417f))
        {
            ret = 200;
        }
        else if(pkmn::fp_compare_less(hp_percentage, 0.1042f))
        {
            ret = 150;
        }
        else if(pkmn::fp_compare_less(hp_percentage, 0.2083f))
        {
            ret = 100;
        }
        else if(pkmn::fp_compare_less(hp_percentage, 0.3542f))
        {
            ret = 80;
        }
        else if(pkmn::fp_compare_less(hp_percentage, 0.6875f))
        {
            ret = 40;
        }
        else
        {
            ret = 20;
        }

        return ret;
    }

    // TODO: fail if item doesn't appear in games with Fling
    int fling_power(
        const std::string& item
    )
    {
        static const char* query = "SELECT fling_power FROM items WHERE id="
                                   "(SELECT item_id FROM item_names WHERE name=?)";

        std::string error_message = "Invalid item: ";
        error_message += item;

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   query, item, error_message
               );
    }

    int frustration_power(
        int friendship
    )
    {
        pkmn::enforce_bounds(
            "Friendship",
            friendship,
            0,
            255
        );

        return std::max<int>(
                   1,
                   int((255.0f - float(friendship)) / 2.5f)
               );
    }

    std::vector<int> fury_cutter_powers(
        int generation
    )
    {
        pkmn::enforce_bounds(
            "Generation",
            generation,
            2,
            6
        );

        switch(generation)
        {
            case 2:
            case 3:
            case 4:
                return {10, 20, 40, 80, 160};

            case 5:
                return {20, 40, 80, 160};

            default:
                return {40, 80, 160};
        }
    }

    int grass_knot_power(
        float target_weight
    )
    {
        pkmn::enforce_comparator(
            "Target weight",
            target_weight,
            0.0f,
            pkmn::value_comparator::GT
        );

        int ret = 0;

        if(pkmn::fp_compare_less(target_weight, 10.0f))
        {
            ret = 20;
        }
        else if(pkmn::fp_compare_less(target_weight, 25.0f))
        {
            ret = 40;
        }
        else if(pkmn::fp_compare_less(target_weight, 50.0f))
        {
            ret = 60;
        }
        else if(pkmn::fp_compare_less(target_weight, 100.0f))
        {
            ret = 80;
        }
        else if(pkmn::fp_compare_less(target_weight, 200.0f))
        {
            ret = 100;
        }
        else
        {
            ret = 120;
        }

        return ret;
    }

    int gyro_ball_power(
        int attacker_speed,
        int target_speed
    )
    {
        pkmn::enforce_comparator(
            "Attacker speed",
            attacker_speed,
            0,
            pkmn::value_comparator::GT
        );
        pkmn::enforce_comparator(
            "Target speed",
            target_speed,
            0,
            pkmn::value_comparator::GT
        );

        int ret = int(25.0f * (float(target_speed) / float(attacker_speed)));
        ret = std::max<int>(ret, 1);
        ret = std::min<int>(ret, 150);

        return ret;
    }

    int heat_crash_power(
        float attacker_weight,
        float target_weight
    )
    {
        pkmn::enforce_comparator(
            "Attacker weight",
            attacker_weight,
            0.0f,
            pkmn::value_comparator::GT
        );
        pkmn::enforce_comparator(
            "Target weight",
            target_weight,
            0.0f,
            pkmn::value_comparator::GT
        );

        float weight_ratio = target_weight / attacker_weight;
        int ret = 0;

        if(pkmn::fp_compare_greater(weight_ratio, 0.5f))
        {
            ret = 40;
        }
        else if(pkmn::fp_compare_greater(weight_ratio, 0.3333f))
        {
            ret = 60;
        }
        else if(pkmn::fp_compare_greater(weight_ratio, 0.25f))
        {
            ret = 80;
        }
        else if(pkmn::fp_compare_greater(weight_ratio, 0.2f))
        {
            ret = 100;
        }
        else
        {
            ret = 120;
        }

        return ret;
    }

    // Heavy Slam is a variation of Heat Crash.
    int heavy_slam_power(
        float attacker_weight,
        float target_weight
    )
    {
        return heat_crash_power(
                   attacker_weight,
                   target_weight
               );
    }

    int low_kick_power(
        float target_weight,
        int generation
    )
    {
        pkmn::enforce_comparator(
            "Target weight",
            target_weight,
            0.0f,
            pkmn::value_comparator::GT
        );
        pkmn::enforce_bounds(
            "Generation",
            generation,
            1,
            6
        );

        int ret = 0;

        if(generation >= 3)
        {
            if(target_weight >= 200.0f)
            {
                ret = 120;
            }
            else if(target_weight >= 100.0f)
            {
                ret = 100;
            }
            else if(target_weight >= 50.0f)
            {
                ret = 80;
            }
            else if(target_weight >= 25.0f)
            {
                ret = 60;
            }
            else if(target_weight >= 10.0f)
            {
                ret = 40;
            }
            else
            {
                ret = 20;
            }
        }
        else
        {
            // Varying power was introduced in Generation III.
            ret = 50;
        }

        return ret;
    }

    // Common for Power Trip and Punishment
    int get_stat_stage_multiplier(
        int attack_stat_stage,
        int defense_stat_stage,
        int special_attack_stat_stage,
        int special_defense_stat_stage,
        int speed_stat_stage,
        int evasion_stat_stage,
        int accuracy_stat_stage
    )
    {
        pkmn::enforce_bounds("Attack stat stage", attack_stat_stage, 0, 6);
        pkmn::enforce_bounds("Defense stat stage", defense_stat_stage, 0, 6);
        pkmn::enforce_bounds("Special Attack stat stage", special_attack_stat_stage, 0, 6);
        pkmn::enforce_bounds("Special Defense stat stage", special_defense_stat_stage, 0, 6);
        pkmn::enforce_bounds("Speed stat_stage", speed_stat_stage, 0, 6);
        pkmn::enforce_bounds("Evasion stat stage", evasion_stat_stage, 0, 6);
        pkmn::enforce_bounds("Accuracy stat stage", accuracy_stat_stage, 0, 6);

        int multiplier = attack_stat_stage
                       + defense_stat_stage
                       + special_attack_stat_stage
                       + special_defense_stat_stage
                       + speed_stat_stage
                       + evasion_stat_stage
                       + accuracy_stat_stage;

        return multiplier;
    }

    int power_trip_power(
        int attack_stat_stage,
        int defense_stat_stage,
        int special_attack_stat_stage,
        int special_defense_stat_stage,
        int speed_stat_stage,
        int evasion_stat_stage,
        int accuracy_stat_stage
    )
    {
        int multiplier = get_stat_stage_multiplier(
                             attack_stat_stage,
                             defense_stat_stage,
                             special_attack_stat_stage,
                             special_defense_stat_stage,
                             speed_stat_stage,
                             evasion_stat_stage,
                             accuracy_stat_stage
                        );

        return (20 + (20 * multiplier));
    }

    int punishment_power(
        int attack_stat_stage,
        int defense_stat_stage,
        int special_attack_stat_stage,
        int special_defense_stat_stage,
        int speed_stat_stage,
        int evasion_stat_stage,
        int accuracy_stat_stage
    )
    {
        int multiplier = get_stat_stage_multiplier(
                             attack_stat_stage,
                             defense_stat_stage,
                             special_attack_stat_stage,
                             special_defense_stat_stage,
                             speed_stat_stage,
                             evasion_stat_stage,
                             accuracy_stat_stage
                         );

        return std::min<int>(
                   200,
                   (60 + (20 * multiplier))
               );
    }

    int return_power(
        int friendship
    )
    {
        pkmn::enforce_bounds("Friendship", friendship, 0, 255);

        return std::max<int>(
                   1,
                   int(float(friendship) / 2.5f)
               );
    }

    // Reversal is a variation of Flail.
    int reversal_power(
        int attacker_current_hp,
        int attacker_max_hp
    )
    {
        return flail_power(attacker_current_hp, attacker_max_hp);
    }

    int spit_up_power(
        int num_stockpile_used
    )
    {
        pkmn::enforce_bounds("Number Stockpile used", num_stockpile_used, 0, 3);

        return (100 * num_stockpile_used);
    }

    // Stored Power is a variation of Power Trip.
    int stored_power_power(
        int attack_stat_stage,
        int defense_stat_stage,
        int special_attack_stat_stage,
        int special_defense_stat_stage,
        int speed_stat_stage,
        int evasion_stat_stage,
        int accuracy_stat_stage
    )
    {
        return power_trip_power(
                   attack_stat_stage,
                   defense_stat_stage,
                   special_attack_stat_stage,
                   special_defense_stat_stage,
                   speed_stat_stage,
                   evasion_stat_stage,
                   accuracy_stat_stage
               );
    }

    int trump_card_power(
        int pp_remaining_after_use
    )
    {
        pkmn::enforce_bounds("PP remaining after use", pp_remaining_after_use, 0, 8);

        static const int TRUMP_CARD_POWERS[] = {200, 80, 60, 50, 40};

        return TRUMP_CARD_POWERS[std::min<int>(4, pp_remaining_after_use)];
    }

    int water_spout_power(
        int attacker_current_hp,
        int attacker_max_hp
    )
    {
        pkmn::enforce_comparator(
            "Attacker max HP",
            attacker_max_hp,
            0,
            pkmn::value_comparator::GT
        );
        pkmn::enforce_bounds(
            "Attacker current HP",
            attacker_current_hp,
            1,
            attacker_max_hp
        );

        return std::max<int>(
                   1,
                   int(150.0f * (float(attacker_current_hp) / float(attacker_max_hp)))
               );
    }

    // Wring Out is a variation of Crush Grip.
    int wring_out_power(
        int target_current_hp,
        int target_max_hp,
        int generation
    )
    {
        return crush_grip_power(
                   target_current_hp,
                   target_max_hp,
                   generation
               );
    }

}}
