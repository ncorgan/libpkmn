/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_FORM_H
#define PKMN_C_CALCULATIONS_FORM_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Returns a Generation II Unown's form based on IV's.
 *
 * Valid return values: "A"-"Z"
 *
 * \param IV_attack Attack IV (0-15)
 * \param IV_defense Defense IV (0-15)
 * \param IV_speed Speed IV (0-15)
 * \param IV_special Special IV (0-15)
 * \param form_out the buffer in which to return the form
 * \param buffer_len the size of the buffer passed into form_out
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if any IV is outside the range [0,15]
 * \returns ::PKMN_ERROR_NULL_POINTER if form_out is NULL
 * \returns ::PKMN_ERROR_BUFFER_TOO_SMALL if the buffer passed into form_out is too small
 */
PKMN_API pkmn_error_t pkmn_calculations_gen2_unown_form(
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_special,
    char* form_out,
    size_t buffer_len
);

/*!
 * @brief Returns a Generation III Unown's form based on its personality.
 *
 * Valid return values: "A"-"Z","?","!"
 *
 * \param personality Unown's personality value
 * \param form_out the buffer in which to return the form
 * \param buffer_len the size of the buffer passed into form_out
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if any IV is outside the range [0,15]
 * \returns ::PKMN_ERROR_NULL_POINTER if form_out is NULL
 * \returns ::PKMN_ERROR_BUFFER_TOO_SMALL if the buffer passed into form_out is too small
 */
PKMN_API pkmn_error_t pkmn_calculations_gen3_unown_form(
    uint32_t personality,
    char* form_out,
    size_t buffer_len
);

/*!
 * @brief Returns whether or not a Wurmple with a given personality will
 *        evolve into a Silcoon.
 *
 * \param personality Wurmple's personality (encryption constant in Generation VI)
 * \param before_gen5 Whether the Wurmple is in a Generation III-IV game
 * \param evolves_out where to return the result
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if evolves_out is NULL
 */
PKMN_API pkmn_error_t pkmn_calculations_wurmple_becomes_silcoon(
    uint32_t personality,
    bool before_gen5,
    bool* evolves_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_FORM_H */
