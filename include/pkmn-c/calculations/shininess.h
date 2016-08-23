/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_SHININESS_H
#define PKMN_C_CALCULATIONS_SHININESS_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_calculations_gen2_shiny(
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_special,
    bool* shiny_out
);

PKMN_API pkmn_error_t pkmn_calculations_modern_shiny(
    uint32_t personality,
    uint32_t trainer_id,
    bool* shiny_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_SHININESS_H */
