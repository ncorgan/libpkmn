/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_MOVES_DAMAGE_HPP
#define PKMN_CALCULATIONS_MOVES_DAMAGE_HPP

#include <pkmn/config.hpp>

#include <string>

namespace pkmn { namespace calculations {

    PKMN_API int damage(
        int attacker_level,
        int move_base_power,
        int attack_stat,
        int defense_stat,
        float modifier
    );

}}
#endif /* PKMN_CALCULATIONS_MOVES_DAMAGE_HPP */
