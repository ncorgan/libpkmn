/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/config.hpp>
 
// http://stackoverflow.com/q/6884093
#ifdef PKMN_PLATFORM_WIN32
#define NOMINMAX
#include <windows.h>
#endif

#include "exception_internal.hpp"
#include "utils/misc.hpp"
#include "types/rng.hpp"
#include "utils/floating_point_comparison.hpp"

#include <pkmn/calculations/personality.hpp>

#include <pkmn/calculations/shininess.hpp>
#include <pkmn/database/pokemon_entry.hpp>
#include <pkmn/enums/gender.hpp>

#include <boost/assign/list_of.hpp>

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace pkmn { namespace calculations {

    static uint32_t get_gender_threshold(
        float chance_male
    )
    {
        uint32_t ret = 0;

        if(pkmn::fp_compare_equal(chance_male, 0.875f))
        {
            ret = 31;
        }
        else if(pkmn::fp_compare_equal(chance_male, 0.75f))
        {
            ret = 64;
        }
        else if(pkmn::fp_compare_equal(chance_male, 0.5f))
        {
            ret = 127;
        }
        else
        {
            ret = 191;
        }

        return ret;
    }

    uint32_t generate_personality(
        pkmn::e_species species,
        uint32_t trainer_id,
        bool shiny,
        pkmn::e_ability ability,
        pkmn::e_gender gender,
        pkmn::e_nature nature
    )
    {
        pkmn::enforce_value_in_vector(
            "Gender",
            gender,
            {pkmn::e_gender::MALE, pkmn::e_gender::FEMALE, pkmn::e_gender::GENDERLESS}
        );

        if((species == pkmn::e_species::NONE) || (species == pkmn::e_species::INVALID))
        {
            throw std::invalid_argument("Species cannot be None or Invalid.");
        }
        if(nature == pkmn::e_nature::NONE)
        {
            throw std::invalid_argument("Nature cannot be None.");
        }

        uint32_t ret = 0;

        pkmn::database::pokemon_entry entry(species, pkmn::e_game::OMEGA_RUBY, "");
        pkmn::ability_pair_t abilities = entry.get_abilities();
        pkmn::e_ability hidden_ability = entry.get_hidden_ability();
        float chance_male = entry.get_chance_male();
        float chance_female = entry.get_chance_female();

        // Validate ability input.
        uint32_t ability_modulo = 0; // If first or hidden ability, keep this
        if(ability == abilities.second)
        {
            if(ability != pkmn::e_ability::NONE)
            {
                ability_modulo = 1;
            }
            else
            {
                throw std::invalid_argument("You cannot use NONE.");
            }
        }
        else if((ability != abilities.first) && (ability != hidden_ability))
        {
            throw std::invalid_argument("Invalid ability.");
        }

        // Validate gender input.
        if(pkmn::fp_compare_equal((chance_male + chance_female), 0.0f))
        {
            if(gender != pkmn::e_gender::GENDERLESS)
            {
                throw std::invalid_argument("This Pokémon is genderless.");
            }
        }
        else if(pkmn::fp_compare_equal(chance_male, 1.0f))
        {
            if(gender != pkmn::e_gender::MALE)
            {
                throw std::invalid_argument("This Pokémon is male-only.");
            }
        }
        else if(pkmn::fp_compare_equal(chance_female, 1.0f))
        {
            if(gender != pkmn::e_gender::FEMALE)
            {
                throw std::invalid_argument("This Pokémon is female-only.");
            }
        }
        else if(gender != pkmn::e_gender::MALE and gender != pkmn::e_gender::FEMALE)
        {
            throw std::invalid_argument("Valid genders: Male, Female");
        }

        // TODO: validate
        uint32_t nature_index = static_cast<uint32_t>(nature) - 1;
        static const size_t NUM_NATURES = 25;

        // Start trying to find a valid value.
        uint32_t gender_threshold = get_gender_threshold(chance_male);
        bool found = false;
        pkmn::rng<uint32_t> rng;
        do
        {
            ret = rng.rand();

            // Set the gender if applicable.
            if(gender == pkmn::e_gender::MALE)
            {
                ret &= ~0xFF;
                ret |= (rng.rand() % (0xFF - gender_threshold) + gender_threshold);
            }
            else if(gender == pkmn::e_gender::FEMALE)
            {
                ret &= ~0xFF;
                ret |= (rng.rand() % gender_threshold);
            }

            if((modern_shiny(ret, trainer_id) == shiny) &&
               ((ret % NUM_NATURES) == nature_index) &&
               ((ret % 2) == ability_modulo)
            )
            {
                found = true;
            }
        }
        while(!found);

        return ret;
    }

}}
