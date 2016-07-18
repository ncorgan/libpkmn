/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/calculations/shininess.hpp>

namespace pkmn { namespace calculations {

    bool gen2_shiny(
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_special
    ) {
        (void)IV_attack;
        (void)IV_defense;
        (void)IV_speed;
        (void)IV_special;
        return true;
    }

    bool modern_shiny(
        uint32_t personality,
        uint32_t trainer_id
    ) {
        (void)personality;
        (void)trainer_id;
        return true;
    }

}}
