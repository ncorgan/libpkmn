/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_GENDER_HPP
#define PKMN_CALCULATIONS_GENDER_HPP

#include <pkmn/config.hpp>

#include <cstdint>
#include <string>

namespace pkmn { namespace calculations {

    PKMN_API std::string gen2_pokemon_gender(
        const std::string &species,
        int IV_attack
    );

    PKMN_API std::string modern_pokemon_gender(
        const std::string &species,
        uint32_t personality
    );

}}
#endif /* PKMN_CALCULATIONS_GENDER_HPP */
