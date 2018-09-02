/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_SIZE_H
#define PKMN_C_CALCULATIONS_SIZE_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/enums/species.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Returns a Pokémon's size, as seen by Generation III-IV NPCs
 *        looking for "large" Pokémon.
 *
 * While this height is not reflected when looking at the Pokédex, this
 * calculation is performed when bringing a Pokémon to said NPC. This
 * function returns height in meters.
 *
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if species or size_out is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if species is invalid
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if any IV parameter is not in the range [0-31]
 */
PKMN_C_API enum pkmn_error pkmn_calculations_pokemon_size(
    enum pkmn_species species,
    uint32_t personality,
    int IV_HP,
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_spatk,
    int IV_spdef,
    float* p_size_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_SIZE_H */
