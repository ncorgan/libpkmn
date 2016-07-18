/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/calculations/hidden_power.hpp>

namespace pkmn { namespace calculations {

    hidden_power_t gen2_hidden_power(
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_special
    ) {
        (void)IV_attack;
        (void)IV_defense;
        (void)IV_speed;
        (void)IV_special;
        return hidden_power_t();
    }

    hidden_power_t modern_hidden_power(
        int IV_HP,
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_spatk,
        int IV_spdef
    ) {
        (void)IV_HP;
        (void)IV_attack;
        (void)IV_defense;
        (void)IV_speed;
        (void)IV_spatk;
        (void)IV_spdef;
        return hidden_power_t();
    }

}}
