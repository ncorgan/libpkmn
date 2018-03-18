/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_HIDDEN_POWER_H
#define PKMN_C_TYPES_HIDDEN_POWER_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

/*!
 * @brief A struct containing the values relevant to Hidden Power.
 *
 * These values are populated by ::pkmn_calculations_gen2_hidden_power
 * or ::pkmn_calculations_modern_hidden_power and must be freed with
 * ::pkmn_hidden_power_free.
 */
typedef struct
{
    //! The type of the move when used by a Pokémon with specific IVs.
    char* type;
    //! The base power of hte move when used by a Pokémon with specific IVs.
    int base_power;
} pkmn_hidden_power_t;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Frees memory dynamically allocated in a pkmn_hidden_power_t.
 *
 * \param hidden_power_ptr The Hidden Power struct whose memory to free
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if hidden_power_ptr is NULL
 */
PKMN_C_API pkmn_error_t pkmn_hidden_power_free(
    pkmn_hidden_power_t* hidden_power_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_HIDDEN_POWER_H */
