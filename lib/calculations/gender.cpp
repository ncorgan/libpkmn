/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/calculations/gender.hpp>

namespace pkmn { namespace calculations {

    std::string gen2_pokemon_gender(
        const std::string &species,
        int IV_attack
    ) {
        (void)species;
        (void)IV_attack;
        return "";
    }

    std::string modern_pokemon_gender(
        const std::string &species,
        uint32_t personality
    ) {
        (void)species;
        (void)personality;
        return "";
    }

}}
