/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/moves/critical_hit.h>

#include <pkmn/calculations/moves/critical_hit.hpp>

enum pkmn_error pkmn_calculations_gen1_critical_hit_chance(
    int speed,
    bool rate_increased,
    bool high_rate_move,
    float* p_critical_hit_chance_out
)
{
    PKMN_CHECK_NULL_PARAM(p_critical_hit_chance_out);

    PKMN_CPP_TO_C(
        *p_critical_hit_chance_out = pkmn::calculations::gen1_critical_hit_chance(
                                         speed,
                                         rate_increased,
                                         high_rate_move
                                     );
    )
}

enum pkmn_error pkmn_calculations_critical_hit_chance(
    int generation,
    int critical_hit_stage,
    float* p_critical_hit_chance_out
)
{
    PKMN_CHECK_NULL_PARAM(p_critical_hit_chance_out);

    PKMN_CPP_TO_C(
        *p_critical_hit_chance_out = pkmn::calculations::critical_hit_chance(
                                         generation,
                                         critical_hit_stage
                                     );
    )
}

enum pkmn_error pkmn_calculations_gen1_critical_hit_modifier(
    int attacker_level,
    float* p_critical_hit_modifier_out
)
{
    PKMN_CHECK_NULL_PARAM(p_critical_hit_modifier_out);

    PKMN_CPP_TO_C(
        *p_critical_hit_modifier_out = pkmn::calculations::gen1_critical_hit_modifier(
                                           attacker_level
                                       );
    )
}

enum pkmn_error pkmn_calculations_critical_hit_modifier(
    int generation,
    float* p_critical_hit_modifier_out
)
{
    PKMN_CHECK_NULL_PARAM(p_critical_hit_modifier_out);

    PKMN_CPP_TO_C(
        *p_critical_hit_modifier_out = pkmn::calculations::critical_hit_modifier(
                                           generation
                                       );
    )
}
