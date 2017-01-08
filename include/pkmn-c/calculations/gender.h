/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_GENDER_H
#define PKMN_C_CALCULATIONS_GENDER_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/types/gender.h>

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_calculations_gen2_pokemon_gender(
    const char* species,
    int IV_attack,
    pkmn_gender_t* gender_out
);

PKMN_API pkmn_error_t pkmn_calculations_modern_pokemon_gender(
    const char* species,
    uint32_t personality,
    pkmn_gender_t* gender_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_GENDER_H */
