/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/moves/damage.h>

#include <pkmn/calculations/moves/damage.hpp>

enum pkmn_error pkmn_calculations_damage(
    int attacker_level,
    int move_base_power,
    int attack_stat,
    int defense_stat,
    float modifier,
    int* p_damage_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_damage_out,
                "p_damage_out"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            *p_damage_out = pkmn::calculations::damage(
                                attacker_level,
                                move_base_power,
                                attack_stat,
                                defense_stat,
                                modifier
                            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}
