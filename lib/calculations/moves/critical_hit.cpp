/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/exception.hpp>

#include <pkmn/calculations/moves/critical_hit.hpp>

namespace pkmn { namespace calculations {

    // Constexpr functions that assume bounds have been checked

    static PKMN_CONSTEXPR_OR_INLINE float _gen1_critical_hit_chance(
        float speed,
        bool rate_increased,
        bool high_rate_move
    )
    {
        return (speed / 512.0f) /
               (rate_increased ? 4.0f : 1.0f) *
               (high_rate_move ? 8.0f : 1.0f);
    }

    static PKMN_CONSTEXPR_OR_INLINE float _stadium_critical_hit_chance(
        float speed,
        bool rate_increased,
        bool high_rate_move
    )
    {
        return (speed + (rate_increased ? 236.0f : 76.0f)) /
               (1024.0f / (rate_increased ? 2.0f : 1.0f)
                        / (high_rate_move ? 8.0f : 1.0f));
    }

    static PKMN_CONSTEXPR_OR_INLINE float _gen1_critical_hit_modifier(
        float attacker_level
    )
    {
        return ((2.0f * attacker_level) + 5.0f) / (attacker_level + 5.0f);
    }

    // Exported functions

    float gen1_critical_hit_chance(
        int speed,
        bool rate_increased,
        bool high_rate_move
    )
    {
        // Validate input parameters.
        if(speed < 0)
        {
            throw std::out_of_range("speed must be > 0.");
        }

        return _gen1_critical_hit_chance(
                   float(speed),
                   rate_increased,
                   high_rate_move
               );
    }

    float stadium_critical_hit_chance(
        int speed,
        bool rate_increased,
        bool high_rate_move
    )
    {
        // Validate input parameters.
        if(speed < 0)
        {
            throw std::out_of_range("speed must be > 0.");
        }

        return _stadium_critical_hit_chance(
                   float(speed),
                   rate_increased,
                   high_rate_move
               );
    }

    float gen1_critical_hit_modifier(
        int attacker_level
    )
    {
        // Validate input parameters (allow 255 for glitch Pokémon).
        if(attacker_level < 1 or attacker_level > 255)
        {
            pkmn::throw_out_of_range("attacker_level", 1, 100);
        }

        return _gen1_critical_hit_modifier(float(attacker_level));
    }

}}
