/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_MOVES_MODIFIERS_HPP
#define PKMN_CALCULATIONS_MOVES_MODIFIERS_HPP

#include <pkmn/config.hpp>

#include <pkmn/enums/type.hpp>

#include <string>

namespace pkmn { namespace calculations {

    /*!
     * @brief The damage modifier when a Pokémon uses a move of a matching type.
     */
    PKMN_CONSTEXPR float STAB_MODIFIER = 1.5f;

    /*!
     * @brief The damage modifier when a move of a given type is used against a
     *        Pokémon of another type.
     *
     * The type charts vary between generations for balancing purposes. Some
     * types also did not exist in earlier generations.
     *
     * \param generation the generation whose type chart to check
     * \param attacking_type the type of the move
     * \param defending_type The type of the defending Pokémon
     * \throws std::out_of_range if the given generation is not in the range [1-6]
     * \throws std::invalid_argument if a given type is invalid or not present
     *                               in the given generation
     * \returns A number in the set {0.5, 1.0, 2.0}
     */
    PKMN_API float type_damage_modifier(
        int generation,
        pkmn::e_type attacking_type,
        pkmn::e_type defending_type
    );

}}
#endif /* PKMN_CALCULATIONS_MOVES_MODIFIERS_HPP */
