/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_MOVES_DAMAGE_HPP
#define PKMN_CALCULATIONS_MOVES_DAMAGE_HPP

#include <pkmn/config.hpp>

namespace pkmn { namespace calculations {

    /*!
     * @brief Damage calculation.
     *
     * \param attacker_level Attacker level (valid values: [1-255])
     * \param move_base_power The base power of the move being used
     * \param attack_stat The stat being used for attack (Attack or Special Attack)
     * \param defense_stat The stat being used for attack (Defense or Special Defense)
     * \param modifier Further damage modifiers
     * \throws std::out_of_range if attacker_level is not in the range [1-255]
     * \throws std::out_of_range if any parameter is negative
     */
    PKMN_API int damage(
        int attacker_level,
        int move_base_power,
        int attack_stat,
        int defense_stat,
        float modifier
    );

}}
#endif /* PKMN_CALCULATIONS_MOVES_DAMAGE_HPP */
