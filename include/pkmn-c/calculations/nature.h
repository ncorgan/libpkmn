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

#include <pkmn-c/enums/nature.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Returns the nature of a Pokémon with the given personality.
/*!
 * \param personality Personality
 * \param p_nature_out Where to return the nature
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if p_nature_out is NULL
 */
PKMN_C_API enum pkmn_error pkmn_calculations_nature(
    uint32_t personality,
    enum pkmn_nature* p_nature_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_NATURE_H */
