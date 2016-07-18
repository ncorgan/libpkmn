/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_FORM_HPP
#define PKMN_CALCULATIONS_FORM_HPP

#include <pkmn/config.hpp>

#include <cstdint>
#include <string>

namespace pkmn { namespace calculations {

    PKMN_API std::string gen2_unown_form(
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_special
    );

    PKMN_API std::string gen3_unown_form(
        uint32_t personality
    );

    PKMN_INLINE bool wurmple_becomes_silcoon(
        uint32_t personality,
        bool before_gen5
    ) {
        return before_gen5 ? ((personality % 10) < 5)
                           : (((personality >> 16) % 10) < 5);
    }

}}
#endif /* PKMN_CALCULATIONS_FORM_HPP */
