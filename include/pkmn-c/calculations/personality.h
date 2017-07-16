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

//! Generate a personality based on the given species and values.
/*!
 * This function is RNG-based and takes an average of 30 milliseconds
 * to generate a valid value.
 *
 * \param species The Pokémon species
 * \param trainer_id The trainer ID to use in the shininess calculation
 * \param shiny Whether or not the Pokémon should be shiny
 * \param ability Which ability the Pokémon should have
 * \param gender What gender the Pokémon should be
 * \param nature What nature the Pokémon should have
 * \param personality_out Where to return the generated personality
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if ability, nature, or personality_out is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the given ability or gender is invalid for the species
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the given nature is invalid
 */
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
