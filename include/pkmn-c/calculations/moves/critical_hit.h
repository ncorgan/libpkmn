/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_MOVES_CRITICAL_HIT_H
#define PKMN_C_CALCULATIONS_MOVES_CRITICAL_HIT_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_calculations_gen1_critical_hit_chance(
    int speed,
    bool rate_increased,
    bool high_rate_move,
    float* critical_hit_chance_out
);

PKMN_API pkmn_error_t pkmn_calculations_gen1_critical_hit_modifier(
    int attacker_level,
    float* critical_hit_modifier_out
);

PKMN_API pkmn_error_t pkmn_calculations_critical_hit_modifier(
    int generation,
    float* critical_hit_modifier_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_MOVES_CRITICAL_HIT_H */
