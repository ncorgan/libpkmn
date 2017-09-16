/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/damage/damage.h>

#include <pkmn/calculations/damage/damage.hpp>

pkmn_error_t pkmn_calculations_damage(
    int attacker_level,
    int move_base_power,
    int attack_stat,
    int defense_stat,
    float modifier,
    int* damage_out
)
{
    PKMN_CHECK_NULL_PARAM(damage_out);

    PKMN_CPP_TO_C(
        *damage_out = pkmn::calculations::damage(
                          attacker_level,
                          move_base_power,
                          attack_stat,
                          defense_stat,
                          modifier
                      );
    )
}
