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
#include "time.h"
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

static pkmn::rng<uint32_t> _rng;

BOOST_STATIC_CONSTEXPR unsigned long long ONE_SECOND = (unsigned long long)1e6;

namespace pkmn { namespace calculations {

    static uint32_t get_gender_threshold(
        float chance_male
    ) {
        if(pkmn::floats_close(chance_male, 0.875f)) {
            return 31;
        } else if(pkmn::floats_close(chance_male, 0.75f)) {
            return 64;
        } else if(pkmn::floats_close(chance_male, 0.5f)) {
            return 127;
        } else {
            return 191;
        }
    }

    uint32_t generate_personality(
        const std::string &species,
        uint32_t trainer_id,
        bool shiny,
        const std::string &ability,
        const std::string &gender,
        const std::string &nature
    ) {
        unsigned long long before_timestamp = 0;
        unsigned long long after_timestamp = 0;
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
        before_timestamp = cal_highres_timestamp();
        uint32_t gender_threshold = get_gender_threshold(chance_male);
        bool found = false;
        size_t count = 0;
        do {
            ret = _rng.rand();

            // Set the gender if applicable.
            if(gender == "Male") {
                ret &= ~0xFF;
                ret |= (_rng.rand() % (0xFF - gender_threshold) + gender_threshold);
            } else if(gender == "Female") {
                ret &= ~0xFF;
                ret |= (_rng.rand() % gender_threshold);
            }

            if(modern_shiny(ret, trainer_id) == shiny and
               (ret % natures.size()) == index and
               (ret % 2) == ability_modulo
            ) {
                found = true;
            }

            after_timestamp = cal_highres_timestamp();
            ++count;
        } while(not found and (after_timestamp - before_timestamp) < ONE_SECOND);

        if(not found) {
            throw std::runtime_error("Failed to generate a valid personality.");
        }

        return ret;
    }

}}
