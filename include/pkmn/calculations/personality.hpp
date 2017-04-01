/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_PERSONALITY_HPP
#define PKMN_CALCULATIONS_PERSONALITY_HPP

#include <pkmn/config.hpp>

#include <cstdint>
#include <string>

namespace pkmn { namespace calculations {

    PKMN_API uint32_t generate_personality(
        const std::string &species,
        uint32_t trainer_id,
        bool shiny,
        const std::string &gender
    );

    PKMN_API uint32_t personality_with_gen3_gen4_nature(
        const std::string &nature
    );

    PKMN_API uint32_t personality_with_gender(
        const std::string &species,
        const std::string &gender
    );

    PKMN_API uint32_t personality_with_shininess(
        uint32_t trainer_id,
        bool shiny
    );

}}
#endif /* PKMN_CALCULATIONS_PERSONALITY_HPP */
