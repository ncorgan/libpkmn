/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
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

#include <pkmn/calculations/personality.hpp>

#include <pkmn/calculations/shininess.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#include "misc_common.hpp"
#include "types/rng.hpp"

#include <boost/assign/list_of.hpp>

#include <algorithm>
#include <stdexcept>
#include <vector>

static const std::vector<std::string> natures = boost::assign::list_of
    ("Hardy")("Lonely")("Brave")("Adamant")("Naughty")
    ("Bold")("Docile")("Relaxed")("Impish")("Lax")
    ("Timid")("Hasty")("Serious")("Jolly")("Naive")
    ("Modest")("Mild")("Quiet")("Bashful")("Rash")
    ("Calm")("Gentle")("Sassy")("Careful")("Quirky")
;

namespace pkmn { namespace calculations {

    static uint32_t get_gender_threshold(
        float chance_male
    ) {
        uint32_t ret = 0;

        if(pkmn::floats_close(chance_male, 0.875f)) {
            ret = 31;
        } else if(pkmn::floats_close(chance_male, 0.75f)) {
            ret = 64;
        } else if(pkmn::floats_close(chance_male, 0.5f)) {
            ret = 127;
        } else {
            ret = 191;
        }

        return ret;
    }

    uint32_t generate_personality(
        const std::string &species,
        uint32_t trainer_id,
        bool shiny,
        const std::string &ability,
        const std::string &gender,
        const std::string &nature
    ) {
        uint32_t ret = 0;

        pkmn::database::pokemon_entry entry(species, "Omega Ruby", "");
        std::pair<std::string, std::string> abilities = entry.get_abilities();
        std::string hidden_ability = entry.get_hidden_ability();
        float chance_male = entry.get_chance_male();
        float chance_female = entry.get_chance_female();

        // Validate ability input.
        uint32_t ability_modulo = 0; // If first or hiddenability, keep this
        if(ability == abilities.second) {
            if(ability != "None") {
                ability_modulo = 1;
            } else {
                throw std::invalid_argument("You cannot use \"None\".");
            }
        } else if(ability != abilities.first and ability != hidden_ability) {
            throw std::invalid_argument("Invalid ability.");
        }

        // Validate gender input.
        if(pkmn::floats_close((chance_male + chance_female), 0.0f)) {
            if(gender != "Genderless") {
                throw std::invalid_argument("This Pokémon is genderless.");
            }
        } else if(pkmn::floats_close(chance_male, 1.0f)) {
            if(gender != "Male") {
                throw std::invalid_argument("This Pokémon is male-only.");
            }
        } else if(pkmn::floats_close(chance_female, 1.0f)) {
            if(gender != "Female") {
                throw std::invalid_argument("This Pokémon is female-only.");
            }
        } else if(gender != "Male" and gender != "Female") {
            throw std::invalid_argument("Valid genders: Male, Female");
        }

        // Validate nature input.
        auto iter = std::find(natures.begin(), natures.end(), nature);
        if(iter == natures.end()) {
            throw std::invalid_argument("Invalid nature.");
        }
        uint32_t index = uint32_t(iter - natures.begin());

        // Start trying to find a valid value.
        uint32_t gender_threshold = get_gender_threshold(chance_male);
        bool found = false;
        size_t count = 0;
        pkmn::rng<uint32_t> rng;
        do {
            ret = rng.rand();

            // Set the gender if applicable.
            if(gender == "Male") {
                ret &= ~0xFF;
                ret |= (rng.rand() % (0xFF - gender_threshold) + gender_threshold);
            } else if(gender == "Female") {
                ret &= ~0xFF;
                ret |= (rng.rand() % gender_threshold);
            }

            if(modern_shiny(ret, trainer_id) == shiny and
               (ret % natures.size()) == index and
               (ret % 2) == ability_modulo
            ) {
                found = true;
            }

            ++count;
        } while(not found);

        return ret;
    }

}}
