/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../misc_common.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/gender.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#include <boost/assign.hpp>
#include <boost/config.hpp>

#include <iostream>
#include <stdexcept>

namespace pkmn { namespace calculations {

    std::string gen2_pokemon_gender(
        const std::string &species,
        int IV_attack
    ) {
        // Input validation
        if(IV_attack < 0 or IV_attack > 15) {
            throw pkmn::range_error("IV_attack", 0, 15);
        }

        pkmn::database::pokemon_entry entry(species, "Crystal", "");
        float chance_male = entry.get_chance_male();
        float chance_female = entry.get_chance_female();

        if(pkmn_floats_close(chance_male, 0.0f) and
           pkmn_floats_close(chance_female, 0.0f)
        ) {
            return "Genderless";
        } else if(pkmn_floats_close(chance_male, 1.0f)) {
            return "Male";
        } else if(pkmn_floats_close(chance_male, 0.875f)) {
            return (IV_attack < 2) ? "Female" : "Male";
        } else if(pkmn_floats_close(chance_male, 0.75f)) {
            return (IV_attack < 4) ? "Female" : "Male";
        } else if(pkmn_floats_close(chance_male, 0.5f)) {
            return (IV_attack < 7) ? "Female" : "Male";
        } else if(pkmn_floats_close(chance_male, 0.25f)) {
            return (IV_attack < 12) ? "Female" : "Male";
        } else {
            return "Female";
        }
    }

    std::string modern_pokemon_gender(
        const std::string &species,
        uint32_t personality
    ) {
        uint8_t truncated_pid = uint8_t(personality & 0xFF);

        pkmn::database::pokemon_entry entry(species, "Omega Ruby", "");
        float chance_male = entry.get_chance_male();
        float chance_female = entry.get_chance_female();

        if(pkmn_floats_close(chance_male, 0.0f) and
           pkmn_floats_close(chance_female, 0.0f)
        ) {
            return "Genderless";
        } else if(pkmn_floats_close(chance_male, 1.0f)) {
            return "Male";
        } else if(pkmn_floats_close(chance_male, 0.875f)) {
            return (truncated_pid < 31) ? "Female" : "Male";
        } else if(pkmn_floats_close(chance_male, 0.75f)) {
            return (truncated_pid < 64) ? "Female" : "Male";
        } else if(pkmn_floats_close(chance_male, 0.5f)) {
            return (truncated_pid < 127) ? "Female" : "Male";
        } else if(pkmn_floats_close(chance_male, 0.25f)) {
            return (truncated_pid < 191) ? "Female" : "Male";
        } else {
            return "Female";
        }
    }

}}
