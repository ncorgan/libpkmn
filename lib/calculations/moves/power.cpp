/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/exception.hpp>
#include <pkmn/calculations/moves/power.hpp>
#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#include "../database/database_common.hpp"

namespace pkmn { namespace calculations {

    static pkmn::database::sptr _db;

    // Used when a database entry is needed
    static const std::string LATE_GEN_GAME = "Omega Ruby";

    int brine_power(
        int target_current_hp,
        int target_max_hp
    )
    {
        if(target_current_hp > target_max_hp)
        {
            pkmn::throw_out_of_range(
                "target_current_hp",
                0,
                target_max_hp
            );
        }

        float hp_percentage = float(target_current_hp) / float(target_max_hp);
        return (hp_percentage < 0.5f) ? 65 : 130;
    }

    int crush_grip_power(
        int target_current_hp,
        int target_max_hp,
        const std::string& game
    )
    {
        if(target_current_hp > target_max_hp)
        {
            pkmn::throw_out_of_range(
                "target_current_hp",
                0,
                target_max_hp
            );
        }

        int generation = pkmn::database::game_name_to_generation(game);

        float hp_percentage = float(target_current_hp) / float(target_max_hp);
        int ret = int(120.0f * hp_percentage);

        if(generation == 4)
        {
            ++ret;
        }
        else if(generation > 4)
        {
            ret = std::max<int>(1, ret);
        }
        else
        {
            throw std::invalid_argument("Crush Grip was introduced in Generation IV.");
        }

        return ret;
    }

    int electro_ball_power(
        int attacker_speed,
        int target_speed
    )
    {
        float speed_percentage = float(attacker_speed) / float(target_speed);

        if(speed_percentage <= 0.25f)
        {
            return 150;
        }
        else if(speed_percentage <= 0.3333f)
        {
            return 120;
        }
        else if(speed_percentage <= 0.5f)
        {
            return 80;
        }
        else if(speed_percentage <= 1.0f)
        {
            return 60;
        }
        else
        {
            return 40;
        }
    }

    int flail_power(
        int attacker_current_hp,
        int attacker_max_hp
    )
    {
        if(attacker_current_hp > attacker_max_hp)
        {
            pkmn::throw_out_of_range(
                "attacker_current_hp",
                0,
                attacker_max_hp
            );
        }

        float hp_percentage = float(attacker_current_hp) / float(attacker_max_hp);

        if(hp_percentage < 0.417)
        {
            return 200;
        }
        else if(hp_percentage < 0.1042)
        {
            return 150;
        }
        else if(hp_percentage < 0.2083)
        {
            return 100;
        }
        else if(hp_percentage < 0.3542)
        {
            return 80;
        }
        else if(hp_percentage < 0.6875)
        {
            return 40;
        }
        else
        {
            return 20;
        }
    }

    int fling_power(
        const std::string& item
    )
    {
        return pkmn::database::item_entry(item, LATE_GEN_GAME).get_fling_power();
    }

    int frustration_power(
        int friendship
    )
    {
        if(friendship < 0 or friendship > 255)
        {
            pkmn::throw_out_of_range("friendship", 0, 255);
        }

        return std::max<int>(
                   1,
                   int((255.0f - float(friendship)) / 2.5f)
               );
    }

    std::vector<int> fury_cutter_powers(
        const std::string& game
    )
    {
        int generation = pkmn::database::game_name_to_generation(game);

        switch(generation)
        {
            case 1:
                throw std::invalid_argument("Fury Cutter did not exist in Generation I.");

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
        if(target_weight < 10.0f)
        {
            return 20;
        }
        else if(target_weight < 25.0f)
        {
            return 40;
        }
        else if(target_weight < 50.0f)
        {
            return 60;
        }
        else if(target_weight < 100.0f)
        {
            return 80;
        }
        else if(target_weight < 200.0f)
        {
            return 100;
        }
        else
        {
            return 120;
        }
    }

    int heat_crash_power(
        float attacker_weight,
        float target_weight
    )
    {
        float weight_ratio = attacker_weight / target_weight;

        if(weight_ratio > 0.5f)
        {
            return 40;
        }
        else if(weight_ratio > 0.3333f)
        {
            return 60;
        }
        else if(weight_ratio > 0.25f)
        {
            return 80;
        }
        else if(weight_ratio > 20.0f)
        {
            return 100;
        }
        else
        {
            return 120;
        }
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
        const std::string& game,
        float target_weight
    )
    {
        int generation = pkmn::database::game_name_to_generation(game);

        if(generation >= 3)
        {
            if(target_weight >= 200.0f)
            {
                return 120;
            }
            else if(target_weight >= 100.0f)
            {
                return 100;
            }
            else if(target_weight >= 50.0f)
            {
                return 80;
            }
            else if(target_weight >= 25.0f)
            {
                return 60;
            }
            else if(target_weight >= 10.0f)
            {
                return 40;
            }
            else
            {
                return 20;
            }
        }
        else
        {
            // Varying power was introduced in Generation III.
            return 50;
        }

        // This shouldn't get here, but GCC gives us an error
        // without this.
        return 50;
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
        enforce_value_range("attack_stat_stage", 0, 6, attack_stat_stage);
        enforce_value_range("defense_stat_stage", 0, 6, defense_stat_stage);
        enforce_value_range("special_attack_stat_stage", 0, 6, special_attack_stat_stage);
        enforce_value_range("special_defense_stat_stage", 0, 6, special_defense_stat_stage);
        enforce_value_range("speed_stat_stage", 0, 6, speed_stat_stage);
        enforce_value_range("evasion_stat_stage", 0, 6, evasion_stat_stage);
        enforce_value_range("accuracy_stat_stage", 0, 6, accuracy_stat_stage);

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
        if(friendship < 0 or friendship > 255)
        {
            pkmn::throw_out_of_range("friendship", 0, 255);
        }

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
        enforce_value_range("num_stockpile_used", 0, 3, num_stockpile_used);

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
        enforce_value_range("pp_remaining_after_use", 0, 4, pp_remaining_after_use);

        static PKMN_CONSTEXPR int TRUMP_CARD_POWERS[] = {200, 80, 60, 50, 40};

        return TRUMP_CARD_POWERS[pp_remaining_after_use];
    }

    int water_spout_power(
        int attacker_current_hp,
        int attacker_max_hp
    )
    {
        enforce_value_range(
            "attacker_current_hp",
            0,
            attacker_max_hp,
            attacker_current_hp
        );

        return int(150.0f * (float(attacker_current_hp) / float(attacker_max_hp)));
    }

    // Wring Out is a variation of Crush Grip.
    int wring_out_power(
        int target_current_hp,
        int target_max_hp,
        const std::string& game
    )
    {
        return crush_grip_power(
                   target_current_hp,
                   target_max_hp,
                   game
               );
    }

}}
