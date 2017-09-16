/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/damage/modifiers.h>

#include <pkmn/calculations/damage/modifiers.hpp>

pkmn_error_t pkmn_calculations_type_damage_modifier(
    int generation,
    const char* attacker_type,
    const char* defender_type1,
    const char* defender_type2,
    float* type_damage_modifier_out
)
{
    PKMN_CHECK_NULL_PARAM(attacker_type);
    PKMN_CHECK_NULL_PARAM(defender_type1);
    PKMN_CHECK_NULL_PARAM(defender_type2);
    PKMN_CHECK_NULL_PARAM(type_damage_modifier_out);

    PKMN_CPP_TO_C(
        *type_damage_modifier_out = pkmn::calculations::type_damage_modifier(
                                        generation,
                                        attacker_type,
                                        defender_type1,
                                        defender_type2
                                    );
    )
}
