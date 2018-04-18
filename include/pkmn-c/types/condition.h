/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_CONDITION_H
#define PKMN_C_TYPES_CONDITION_H

/*!
 * @brief Status effects with which a Pokémon can be inflicted.
 *
 * This enum only includes conditions that persist after a battle and
 * does not include temporary conditions like confusion or infatuation.
 *
 * All of these values are valid inputs for ::pkmn_pokemon_set_condition
 * and valid outputs for ::pkmn_pokemon_get_condition.
 */
enum pkmn_condition
{
    //! No status effect.
    PKMN_CONDITION_NONE = 0,
    //! Asleep.
    PKMN_CONDITION_ASLEEP,
    //! Poison.
    PKMN_CONDITION_POISON,
    //! Burn.
    PKMN_CONDITION_BURN,
    //! Freeze.
    PKMN_CONDITION_FROZEN,
    //! Paralysis.
    PKMN_CONDITION_PARALYSIS,
    //! Bad poison (such as from Toxic).
    PKMN_CONDITION_BAD_POISON
};

#endif /* PKMN_C_TYPES_CONDITION_H */
