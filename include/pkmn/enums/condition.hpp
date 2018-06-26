/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ENUMS_CONDITION_HPP
#define PKMN_ENUMS_CONDITION_HPP

namespace pkmn
{
    /*!
     * @brief Status effects with which a Pokémon can be inflicted.
     *
     * This enum only includes conditions that persist after a battle and
     * does not include temporary conditions like confusion or infatuation.
     *
     * All of these values are valid inputs for ::pkmn_pokemon_set_condition
     * and valid outputs for ::pkmn_pokemon_get_condition.
     */
    enum class e_condition
    {
        //! No status effect.
        NONE = 0,
        //! Asleep.
        ASLEEP,
        //! Poison.
        POISON,
        //! Burn.
        BURN,
        //! Freeze.
        FROZEN,
        //! Paralysis.
        PARALYSIS,
        //! Bad poison (such as from Toxic).
        BAD_POISON
    };
}

#endif /* PKMN_ENUMS_CONDITION_HPP */
