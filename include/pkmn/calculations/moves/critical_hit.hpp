/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_MOVES_CRITICAL_HIT_HPP
#define PKMN_CALCULATIONS_MOVES_CRITICAL_HIT_HPP

#include <pkmn/config.hpp>

#include <string>

namespace pkmn { namespace calculations {

    /*!
     * @brief Returns the rate of a move getting a critical hit in Generation I games.
     *
     * Some moves inherently have a higher critical hit rate; using these moves increases
     * the critical hit rate by 8x. Other moves are meant to increase the Pokémon's
     * general critical hit rate. However, due to a bug, it actually lowers the rate by
     * 4x, so that is reflected here.
     *
     * \param speed the Pokémon's speed (valid values [1-255])
     * \param rate_increased Whether the general critical hit rate was increased
     * \param high_rate_move Whether move inherently has a higher critical hit rate
     * \throws std::out_of_range if speed is outside the range [1-255]
     * \returns A number in the range [0.0, 1.0]
     */
    PKMN_API float gen1_critical_hit_chance(
        int speed,
        bool rate_increased,
        bool high_rate_move
    );

    /*!
     * @brief Returns the rate of a move getting a critical hit in Generation II+ games.
     *
     * In Generation II, the chance of a critical hit is based on an in-battle stage
     * value. Moves that increase the Pokémon's general critical hit range increment this
     * value by 1, until the value reaches its peak when the stage value is 4. This value
     * varies between generations.
     *
     * \param generation Which generation the battle is taking place in
     * \param critical_hit_stage The in-game battle stage value (valid values: 0+)
     * \throws std::out_of_range if the generation is not in the range [2-6]
     * \throws std::out_of_range if the critical hit stage is negative
     * \returns A number in the range [0.0, 1.0]
     */
    PKMN_API float critical_hit_chance(
        int generation,
        int critical_hit_stage
    );

    /*!
     * @brief Returns the damage modifier for when a move gets a critical hit in Generation I.
     *
     * The damage modifier increases with the level.
     *
     * \param attacker_level The attacker's level (valid values: [1-255])
     * \throws std::out_of_range if attacker_level is not in the range [1-255]
     * \returns a number in the range [1.0, 2.0]
     */
    PKMN_API float gen1_critical_hit_modifier(
        int attacker_level
    );

    /*!
     * @brief Returns the damage modifier for when a move gets a critical hit in Generation II+.
     *
     * The damage modifier is a generation-dependent flat value.
     *
     * \param generation Which generation the battle is taking place in
     * \throws std::out_of_range if generation is not in the range [2-6]
     * \returns Generation II-V: 1.5
     * \returns Generation VI: 2.0
     */
    PKMN_API float critical_hit_modifier(
        int generation
    );

}}
#endif /* PKMN_CALCULATIONS_MOVES_CRITICAL_HIT_HPP */
