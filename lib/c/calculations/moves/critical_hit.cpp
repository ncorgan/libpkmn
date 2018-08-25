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
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_critical_hit_chance_out,
                "p_critical_hit_chance_out"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            *p_critical_hit_chance_out = pkmn::calculations::gen1_critical_hit_chance(
                                             speed,
                                             rate_increased,
                                             high_rate_move
                                         );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_calculations_critical_hit_chance(
    int generation,
    int critical_hit_stage,
    float* p_critical_hit_chance_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_critical_hit_chance_out,
                "p_critical_hit_chance_out"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            *p_critical_hit_chance_out = pkmn::calculations::critical_hit_chance(
                                             generation,
                                             critical_hit_stage
                                         );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_calculations_gen1_critical_hit_modifier(
    int attacker_level,
    float* p_critical_hit_modifier_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_critical_hit_modifier_out,
                "p_critical_hit_modifier_out"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            *p_critical_hit_modifier_out = pkmn::calculations::gen1_critical_hit_modifier(
                                               attacker_level
                                           );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_calculations_critical_hit_modifier(
    int generation,
    float* p_critical_hit_modifier_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_critical_hit_modifier_out,
                "p_critical_hit_modifier_out"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            *p_critical_hit_modifier_out = pkmn::calculations::critical_hit_modifier(
                                               generation
                                           );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}
