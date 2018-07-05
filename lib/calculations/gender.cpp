/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "utils/misc.hpp"
#include "utils/floating_point_comparison.hpp"

#include <pkmn/calculations/gender.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#include <boost/assert.hpp>

namespace pkmn { namespace calculations {

    pkmn::e_gender gen2_pokemon_gender(
        pkmn::e_species species,
        int IV_attack
    )
    {
        pkmn::enforce_IV_bounds(
            "Attack",
            IV_attack,
            false
        );
        if((species == pkmn::e_species::NONE) || (species == pkmn::e_species::INVALID))
        {
            throw std::invalid_argument("Species cannot be none or invalid.");
        }

        pkmn::database::pokemon_entry entry(species, pkmn::e_game::CRYSTAL, "");
        float chance_male = entry.get_chance_male();
        float chance_female = entry.get_chance_female();

        pkmn::e_gender ret = pkmn::e_gender::NONE;

        if(pkmn::fp_compare_equal(chance_male, 0.0f) and
           pkmn::fp_compare_equal(chance_female, 0.0f)
        )
        {
            ret = pkmn::e_gender::GENDERLESS;
        }
        else if(pkmn::fp_compare_equal(chance_male, 1.0f))
        {
            ret = pkmn::e_gender::MALE;
        }
        else if(pkmn::fp_compare_equal(chance_male, 0.875f))
        {
            ret = (IV_attack < 2) ? pkmn::e_gender::FEMALE
                                  : pkmn::e_gender::MALE;
        }
        else if(pkmn::fp_compare_equal(chance_male, 0.75f))
        {
            ret = (IV_attack < 4) ? pkmn::e_gender::FEMALE
                                  : pkmn::e_gender::MALE;
        }
        else if(pkmn::fp_compare_equal(chance_male, 0.5f))
        {
            ret = (IV_attack < 7) ? pkmn::e_gender::FEMALE
                                  : pkmn::e_gender::MALE;
        }
        else if(pkmn::fp_compare_equal(chance_male, 0.25f))
        {
            ret = (IV_attack < 12) ? pkmn::e_gender::FEMALE
                                   : pkmn::e_gender::MALE;
        }
        else
        {
            ret = pkmn::e_gender::FEMALE;
        }

        BOOST_ASSERT(ret != pkmn::e_gender::NONE);

        return ret;
    }

    pkmn::e_gender modern_pokemon_gender(
        pkmn::e_species species,
        uint32_t personality
    )
    {
        if((species == pkmn::e_species::NONE) || (species == pkmn::e_species::INVALID))
        {
            throw std::invalid_argument("Species cannot be none or invalid.");
        }

        uint8_t truncated_pid = uint8_t(personality & 0xFF);

        pkmn::database::pokemon_entry entry(species, pkmn::e_game::OMEGA_RUBY, "");
        float chance_male = entry.get_chance_male();
        float chance_female = entry.get_chance_female();

        pkmn::e_gender ret = pkmn::e_gender::NONE;

        if(pkmn::fp_compare_equal(chance_male, 0.0f) and
           pkmn::fp_compare_equal(chance_female, 0.0f)
        )
        {
            ret = pkmn::e_gender::GENDERLESS;
        }
        else if(pkmn::fp_compare_equal(chance_male, 1.0f))
        {
            ret = pkmn::e_gender::MALE;
        }
        else if(pkmn::fp_compare_equal(chance_male, 0.875f))
        {
            ret = (truncated_pid < 31) ? pkmn::e_gender::FEMALE
                                       : pkmn::e_gender::MALE;
        }
        else if(pkmn::fp_compare_equal(chance_male, 0.75f))
        {
            ret = (truncated_pid < 64) ? pkmn::e_gender::FEMALE
                                       : pkmn::e_gender::MALE;
        }
        else if(pkmn::fp_compare_equal(chance_male, 0.5f))
        {
            ret = (truncated_pid < 127) ? pkmn::e_gender::FEMALE
                                        : pkmn::e_gender::MALE;
        }
        else if(pkmn::fp_compare_equal(chance_male, 0.25f))
        {
            ret = (truncated_pid < 191) ? pkmn::e_gender::FEMALE
                                        : pkmn::e_gender::MALE;
        }
        else
        {
            ret = pkmn::e_gender::FEMALE;
        }

        BOOST_ASSERT(ret != pkmn::e_gender::NONE);

        return ret;
    }

}}
