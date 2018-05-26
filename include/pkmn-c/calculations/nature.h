/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_NATURE_H
#define PKMN_C_CALCULATIONS_NATURE_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Returns the nature of a Pokémon with the given personality.
/*!
 * \param personality Personality
 * \param p_nature_out Where to return the nature
 * \param buffer_len The length of the buffer passed into p_nature_out
 * \param p_nature_length_out The length of the actual string (can be NULL)
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if p_nature_out is NULL
 */
PKMN_C_API enum pkmn_error pkmn_calculations_nature(
    uint32_t personality,
    char* p_nature_out,
    size_t buffer_len,
    size_t* p_nature_length_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_NATURE_H */
