/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_MODIFIERS_MOVES_H
#define PKMN_C_CALCULATIONS_MODIFIERS_MOVES_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/types/natural_gift.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Calculate Natural Gift stats.
 *
 * Base powers can vary for the same item between generations.
 *
 * The pkmn_natural_gift_t instance returned will need to be freed with
 * ::pkmn_natural_gift_free.
 *
 * \param item_name The name of the item held by the Pokémon
 * \param generation The generation of the game
 * \param natural_gift_stats_out Where to return the Natural Gift info
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if item_name or natural_gift_stats_out is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the item is not a Berry
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if the generation is not in the range [4-6]
 */
PKMN_C_API enum pkmn_error pkmn_calculations_natural_gift_stats(
    const char* item_name,
    int generation,
    struct pkmn_natural_gift* natural_gift_stats_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_MODIFIERS_MOVES_H */
