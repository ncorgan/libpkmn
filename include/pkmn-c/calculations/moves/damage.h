/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_MOVES_DAMAGE_H
#define PKMN_C_CALCULATIONS_MOVES_DAMAGE_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_calculations_damage(
    int attacker_level,
    int move_base_power,
    int attack_stat,
    int defense_stat,
    float modifier,
    int* damage_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_MOVES_DAMAGE_H */
