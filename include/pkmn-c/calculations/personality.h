/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_PERSONALITY_H
#define PKMN_C_CALCULATIONS_PERSONALITY_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/enums/ability.h>
#include <pkmn-c/enums/gender.h>
#include <pkmn-c/enums/nature.h>
#include <pkmn-c/enums/species.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Generate a personality based on the given p_species and values.
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
 * \param p_personality_out Where to return the generated personality
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if ability, p_nature, or p_personality_out is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the given p_ability or gender is invalid for the p_species
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the given p_nature is invalid
 */
PKMN_C_API enum pkmn_error pkmn_calculations_generate_personality(
    enum pkmn_species species,
    uint32_t trainer_id,
    bool shiny,
    enum pkmn_ability ability,
    enum pkmn_gender gender,
    enum pkmn_nature nature,
    uint32_t* p_personality_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_PERSONALITY_H */
