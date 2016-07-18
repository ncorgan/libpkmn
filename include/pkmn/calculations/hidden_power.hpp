/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_HIDDEN_POWER_HPP
#define PKMN_CALCULATIONS_HIDDEN_POWER_HPP

#include <pkmn/config.hpp>

#include <string>

namespace pkmn { namespace calculations {

    typedef struct {
        std::string type;
        int base_power;
    } hidden_power_t;

    PKMN_API hidden_power_t gen2_hidden_power(
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_special
    );

    PKMN_API hidden_power_t modern_hidden_power(
        int IV_HP,
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_spatk,
        int IV_spdef
    );

}}
#endif /* PKMN_CALCULATIONS_HIDDEN_POWER_HPP */
