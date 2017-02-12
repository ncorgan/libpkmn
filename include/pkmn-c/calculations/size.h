/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_SIZE_H
#define PKMN_C_CALCULATIONS_SIZE_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_calculations_pokemon_size(
    const char* species,
    uint32_t personality,
    int IV_HP,
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_spatk,
    int IV_spdef,
    float* size_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_SIZE_H */
