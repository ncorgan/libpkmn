/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_SIZE_HPP
#define PKMN_CALCULATIONS_SIZE_HPP

#include <pkmn/config.hpp>

#include <cstdint>
#include <string>

namespace pkmn { namespace calculations {

    PKMN_API float get_pokemon_size(
        const std::string &species,
        uint32_t personality,
        int IV_HP,
        int IV_defense,
        int IV_speed,
        int IV_spatk,
        int IV_spdef
    );

}}
#endif /* PKMN_CALCULATIONS_SIZE_HPP */
