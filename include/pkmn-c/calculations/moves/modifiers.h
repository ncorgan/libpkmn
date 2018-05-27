/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_MODIFIERS_MODIFIERS_H
#define PKMN_C_CALCULATIONS_MODIFIERS_MODIFIERS_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief The damage modifier when a Pokémon uses a move of a matching type.
 */
#define PKMN_STAB_MODIFIER 1.5f

/*!
 * @brief The damage modifier when a move of a given type is used against a
 *        Pokémon of another type.
 *
 * The type charts vary between generations for balancing purposes. Some
 * types also did not exist in earlier generations.
 *
 * The number returned will be in the set {0.5, 1.0, 2.0}.
 *
 * \param generation the generation whose type chart to check
 * \param attacking_type the type of the move
 * \param defending_type The type of the defending Pokémon
 * \param type_damage_modifier_out Where to return the type damage modifier
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if type_damage_modifier_out is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if the given generation is not in the range [1-6]
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if a given type is invalid or not present
 *                                        in the given generation
 */
PKMN_C_API enum pkmn_error pkmn_calculations_type_damage_modifier(
    int generation,
    const char* p_attacking_type,
    const char* p_defending_type,
    float* p_type_damage_modifier_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_MODIFIERS_MODIFIERS_H */
