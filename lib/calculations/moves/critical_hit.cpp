/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/exception.hpp>

#include <pkmn/calculations/moves/critical_hit.hpp>

#include <cstdint>

namespace pkmn { namespace calculations {

    // Source: https://bulbapedia.bulbagarden.net/wiki/Critical_hit

    float gen1_critical_hit_chance(
        int speed,
        bool rate_increased,
        bool high_rate_move
    )
    {
        pkmn::enforce_bounds(
            "Speed",
            speed,
            1,
            255
        );

        uint8_t threshold = uint8_t(speed / 2);

        // Focus Energy and Dire Hit should increase the chance of a critical
        // hit, but due to a bug, they actually decrease it.
        if(rate_increased)
        {
            threshold /= 4;
        }

        if(high_rate_move)
        {
            threshold *= 8;
        }

        return (float(threshold) / 256.0f);
    }

    float critical_hit_chance(
        int generation,
        int critical_hit_stage
    )
    {
        pkmn::enforce_bounds(
            "Generation",
            generation,
            2,
            6
        );
        pkmn::enforce_comparator(
            "Critical hit stage",
            critical_hit_stage,
            0,
            pkmn::value_comparator::GE
        );

        bool is_early_game = (generation < 6);
        float ret = 0.0f;

        switch(critical_hit_stage)
        {
            case 0:
                ret = 0.0625f;
                break;

            case 1:
                ret = 0.125f;
                break;

            case 2:
                ret = is_early_game ? 0.25f : 0.5f;
                break;

            case 3:
                ret = is_early_game ? 0.333f : 1.0f;
                break;

            default:
                ret = is_early_game ? 0.5f : 1.0f;
                break;
        }

        return ret;
    }

    float gen1_critical_hit_modifier(
        int attacker_level
    )
    {
        pkmn::enforce_bounds(
            "Attacker level",
            attacker_level,
            1,
            255 // Account for "glitch Pokémon"
        );

        return ((2.0f * float(attacker_level)) + 5.0f) /
               (float(attacker_level) + 5.0f);
    }

    float critical_hit_modifier(
        int generation
    )
    {
        pkmn::enforce_bounds(
            "Generation",
            generation,
            2,
            6
        );

        return (generation >= 6) ? 1.5f : 2.0f;
    }

}}
