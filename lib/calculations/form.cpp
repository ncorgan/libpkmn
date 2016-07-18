/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/calculations/form.hpp>

namespace pkmn { namespace calculations {

    std::string gen2_unown_form(
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_special
    ) {
        (void)IV_attack;
        (void)IV_defense;
        (void)IV_speed;
        (void)IV_special;
        return "";
    }

    std::string gen3_unown_form(
        uint32_t personality
    ) {
        (void)personality;
        return "";
    }

}}
