/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_PERSONALITY_H
#define PKMN_C_CALCULATIONS_PERSONALITY_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/types/gender.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_calculations_generate_personality(
    const char* species,
    uint32_t trainer_id,
    bool shiny,
    const char* ability,
    pkmn_gender_t gender,
    const char* nature,
    uint32_t* personality_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_PERSONALITY_H */
