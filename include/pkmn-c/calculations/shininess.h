/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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

/*!
 * @brief Calculates whether a Generation II Pokémon with the given
 *        IVs is shiny.
 *
 * \param IV_attack Attack IV (0-15)
 * \param IV_defense Defense IV (0-15)
 * \param IV_speed Speed IV (0-15)
 * \param IV_special Special IV (0-15)
 * \param shiny_out Where to return shininess
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if shiny_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if any parameter is not in the range [0-15]
 */
PKMN_C_API enum pkmn_error pkmn_calculations_gen2_shiny(
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_special,
    bool* shiny_out
);

/*!
 * @brief Calculates whether a Generation III+ Pokémon with the given
 *        personality and original Trainer ID is shiny.
 *
 * \param personality Pokémon's personality
 * \param trainer_id Pokémon's full original trainer ID
 * \param shiny_out Where to return shininess
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if shiny_out is NULL
 */
PKMN_C_API enum pkmn_error pkmn_calculations_modern_shiny(
    uint32_t personality,
    uint32_t trainer_id,
    bool* shiny_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_SHININESS_H */
