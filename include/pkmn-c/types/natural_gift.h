/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_NATURAL_GIFT_H
#define PKMN_C_TYPES_NATURAL_GIFT_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

/*!
 * @brief A struct containing the values relevant to Natural Gift.
 *
 * These values are populated by ::pkmn_calculations_natural_gift_stats
 * and must be freed with ::pkmn_natural_gift_free.
 */
struct pkmn_natural_gift
{
    //! The type of the move when used by a Pokémon holding a specific item.
    char* type;
    //! The base power of hte move when used by a Pokémon holding a specific item.
    int base_power;
};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Frees memory dynamically allocated in a pkmn_natural_gift_t.
 *
 * \param natural_gift_ptr The Natural Gift struct whose memory to free
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if natural_gift_ptr is NULL
 */
PKMN_C_API enum pkmn_error pkmn_natural_gift_free(
    struct pkmn_natural_gift* natural_gift_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_NATURAL_GIFT_H */
