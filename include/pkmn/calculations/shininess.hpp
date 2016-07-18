/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_SHININESS_HPP
#define PKMN_CALCULATIONS_SHININESS_HPP

#include <pkmn/config.hpp>

#include <cstdint>

namespace pkmn { namespace calculations {

    PKMN_API bool gen2_shiny(
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_special
    );

    PKMN_API bool modern_shiny(
        uint32_t personality,
        uint32_t trainer_id
    );

}}
#endif /* PKMN_CALCULATIONS_SHININESS_HPP */
