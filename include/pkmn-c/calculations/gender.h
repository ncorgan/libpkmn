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

/*!
 * @brief Returns the gender of a Generation II Pokémon of the given species
 *        with the given Attack IV.
 *
 * In Generation II, unless the Pokémon is genderless, the Pokémon's gender depends
 * on its Attack IV. Below a species-dependent threshold, the Pokémon is female. Above
 * it, it is male.
 *
 * \param species Pokémon species
 * \param IV_attack Attack IV (1-15)
 * \param gender_out Where to return the gender
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if species or gender_out is null
 * \returns ::PKMN_ERROR_RUNTIME_ERROR If Pokémon species is invalid
 * \returns ::PKMN_ERROR_OUT_OF_RANGE If IV_attack is out of range [0,15]
 */
PKMN_C_API pkmn_error_t pkmn_calculations_gen2_pokemon_gender(
    const char* species,
    int IV_attack,
    pkmn_gender_t* gender_out
);

/*!
 * @brief Returns the gender of a Generation III+ Pokémon of the given species
 *        with the given personality.
 *
 * In Generation III+, unless the Pokémon is genderless, the Pokémon's gender
 * depends on the lowest byte of its personality. Below a species-dependent
 * threshold, the Pokémon is female. Above it, it is male.
 *
 * \param species Pokémon species
 * \param personality Pokémon personality
 * \param gender_out Where to return the gender
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if species or gender_out is null
 * \returns ::PKMN_ERROR_RUNTIME_ERROR If Pokémon species is invalid
 */
PKMN_C_API pkmn_error_t pkmn_calculations_modern_pokemon_gender(
    const char* species,
    uint32_t personality,
    pkmn_gender_t* gender_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_GENDER_H */
