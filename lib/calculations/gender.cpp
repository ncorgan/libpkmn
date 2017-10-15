/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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
    )
    {
        // Input validation
        if(IV_attack < 0 or IV_attack > 15)
        {
            pkmn::throw_out_of_range("IV_attack", 0, 15);
        }

        pkmn::database::pokemon_entry entry(species, "Crystal", "");
        float chance_male = entry.get_chance_male();
        float chance_female = entry.get_chance_female();

        std::string ret;

        if(pkmn::floats_close(chance_male, 0.0f) and
           pkmn::floats_close(chance_female, 0.0f)
        )
        {
            ret = "Genderless";
        }
        else if(pkmn::floats_close(chance_male, 1.0f))
        {
            ret = "Male";
        }
        else if(pkmn::floats_close(chance_male, 0.875f))
        {
            ret = (IV_attack < 2) ? "Female" : "Male";
        }
        else if(pkmn::floats_close(chance_male, 0.75f))
        {
            ret = (IV_attack < 4) ? "Female" : "Male";
        }
        else if(pkmn::floats_close(chance_male, 0.5f))
        {
            ret = (IV_attack < 7) ? "Female" : "Male";
        }
        else if(pkmn::floats_close(chance_male, 0.25f))
        {
            ret = (IV_attack < 12) ? "Female" : "Male";
        }
        else
        {
            ret = "Female";
        }

        return ret;
    }

    std::string modern_pokemon_gender(
        const std::string &species,
        uint32_t personality
    )
    {
        uint8_t truncated_pid = uint8_t(personality & 0xFF);

        pkmn::database::pokemon_entry entry(species, "Omega Ruby", "");
        float chance_male = entry.get_chance_male();
        float chance_female = entry.get_chance_female();

        std::string ret;

        if(pkmn::floats_close(chance_male, 0.0f) and
           pkmn::floats_close(chance_female, 0.0f)
        )
        {
            ret = "Genderless";
        }
        else if(pkmn::floats_close(chance_male, 1.0f))
        {
            ret = "Male";
        }
        else if(pkmn::floats_close(chance_male, 0.875f))
        {
            ret = (truncated_pid < 31) ? "Female" : "Male";
        }
        else if(pkmn::floats_close(chance_male, 0.75f))
        {
            ret = (truncated_pid < 64) ? "Female" : "Male";
        }
        else if(pkmn::floats_close(chance_male, 0.5f))
        {
            ret = (truncated_pid < 127) ? "Female" : "Male";
        }
        else if(pkmn::floats_close(chance_male, 0.25f))
        {
            ret = (truncated_pid < 191) ? "Female" : "Male";
        }
        else
        {
            ret = "Female";
        }

        return ret;
    }

}}
