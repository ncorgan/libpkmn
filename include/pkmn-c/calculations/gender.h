/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_GENDER_H
#define PKMN_C_CALCULATIONS_GENDER_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/enums/gender.h>
#include <pkmn-c/enums/species.h>

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
 * \param p_gender_out Where to return the gender
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if species or p_gender_out is null
 * \returns ::PKMN_ERROR_RUNTIME_ERROR If Pokémon species is invalid
 * \returns ::PKMN_ERROR_OUT_OF_RANGE If IV_attack is out of range [0,15]
 */
PKMN_C_API enum pkmn_error pkmn_calculations_gen2_pokemon_gender(
    enum pkmn_species species,
    int IV_attack,
    enum pkmn_gender* p_gender_out
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
 * \param p_gender_out Where to return the gender
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if species or p_gender_out is null
 * \returns ::PKMN_ERROR_RUNTIME_ERROR If Pokémon species is invalid
 */
PKMN_C_API enum pkmn_error pkmn_calculations_modern_pokemon_gender(
    enum pkmn_species species,
    uint32_t personality,
    enum pkmn_gender* p_gender_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_GENDER_H */
