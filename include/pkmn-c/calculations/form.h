/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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
 * \param p_form_out the buffer in which to return the form
 * \param buffer_len the size of the buffer passed into p_form_out
 * \param p_form_length_out the actual string length (can be NULL)
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if any IV is outside the range [0,15]
 * \returns ::PKMN_ERROR_NULL_POINTER if p_form_out is NULL
 */
PKMN_C_API enum pkmn_error pkmn_calculations_gen2_unown_form(
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_special,
    char* p_form_out,
    size_t buffer_len,
    size_t* p_form_length_out
);

/*!
 * @brief Returns a Generation III Unown's form based on its personality.
 *
 * Valid return values: "A"-"Z","?","!"
 *
 * \param personality Unown's personality value
 * \param p_form_out the buffer in which to return the form
 * \param buffer_len the size of the buffer passed into p_form_out
 * \param p_form_length_out the actual string length (can be NULL)
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if any IV is outside the range [0,15]
 * \returns ::PKMN_ERROR_NULL_POINTER if p_form_out is NULL
 */
PKMN_C_API enum pkmn_error pkmn_calculations_gen3_unown_form(
    uint32_t personality,
    char* p_form_out,
    size_t buffer_len,
    size_t* p_form_length_out
);

/*!
 * @brief Returns whether or not a Wurmple with a given personality will
 *        evolve into a Silcoon.
 *
 * \param personality Wurmple's personality (encryption constant in Generation VI)
 * \param before_gen5 Whether the Wurmple is in a Generation III-IV game
 * \param p_evolves_out where to return the result
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if p_evolves_out is NULL
 */
PKMN_C_API enum pkmn_error pkmn_calculations_wurmple_becomes_silcoon(
    uint32_t personality,
    bool before_gen5,
    bool* p_evolves_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_FORM_H */
