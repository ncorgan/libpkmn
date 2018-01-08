/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/moves/modifiers.h>

#include <pkmn/calculations/moves/modifiers.hpp>

pkmn_error_t pkmn_calculations_type_damage_modifier(
    int generation,
    const char* attacking_type,
    const char* defending_type,
    float* type_damage_modifier_out
)
{
    PKMN_CHECK_NULL_PARAM(attacking_type);
    PKMN_CHECK_NULL_PARAM(defending_type);
    PKMN_CHECK_NULL_PARAM(type_damage_modifier_out);

    PKMN_CPP_TO_C(
        *type_damage_modifier_out = pkmn::calculations::type_damage_modifier(
                                        generation,
                                        attacking_type,
                                        defending_type
                                    );
    )
}
