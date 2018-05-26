/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/moves/modifiers.h>

#include <pkmn/calculations/moves/modifiers.hpp>

enum pkmn_error pkmn_calculations_type_damage_modifier(
    int generation,
    const char* p_attacking_type,
    const char* p_defending_type,
    float* p_type_damage_modifier_out
)
{
    PKMN_CHECK_NULL_PARAM(p_attacking_type);
    PKMN_CHECK_NULL_PARAM(p_defending_type);
    PKMN_CHECK_NULL_PARAM(p_type_damage_modifier_out);

    PKMN_CPP_TO_C(
        *p_type_damage_modifier_out = pkmn::calculations::type_damage_modifier(
                                          generation,
                                          p_attacking_type,
                                          p_defending_type
                                      );
    )
}
