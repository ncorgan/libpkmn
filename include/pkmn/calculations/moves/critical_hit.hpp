/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_MOVES_CRITICAL_HIT_HPP
#define PKMN_CALCULATIONS_MOVES_CRITICAL_HIT_HPP

#include <pkmn/config.hpp>

#include <string>

namespace pkmn { namespace calculations {

    PKMN_API float gen1_critical_hit_chance(
        int speed,
        bool rate_increased,
        bool high_rate_move
    );

    PKMN_API float stadium_critical_hit_chance(
        int speed,
        bool rate_increased,
        bool high_rate_move
    );

    PKMN_API float gen1_critical_hit_modifier(
        int attacker_level
    );

    PKMN_CONSTEXPR_OR_INLINE float critical_hit_modifier(
        bool before_gen6
    ) {
        return before_gen6 ? 2.0f : 1.5f;
    }

}}
#endif /* PKMN_CALCULATIONS_MOVES_CRITICAL_HIT_HPP */
