/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_MODIFIERS_MODIFIERS_H
#define PKMN_C_CALCULATIONS_MODIFIERS_MODIFIERS_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PKMN_STAB_MODIFIER 1.5f

PKMN_API pkmn_error_t pkmn_calculations_type_damage_modifier(
    int generation,
    const char* attacking_type,
    const char* defending_type1,
    const char* defending_type2,
    float* type_damage_modifier_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_MODIFIERS_MODIFIERS_H */
