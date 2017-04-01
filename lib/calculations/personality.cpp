/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

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

static const std::vector<std::string> _natures = boost::assign::list_of
    ("Hardy")("Lonely")("Brave")("Adamant")("Naughty")
    ("Bold")("Docile")("Relaxed")("Impish")("Lax")
    ("Timid")("Hasty")("Serious")("Jolly")("Naive")
    ("Modest")("Mild")("Quiet")("Bashful")("Rash")
    ("Calm")("Gentle")("Sassy")("Careful")("Quirky")
;

static pkmn::rng<uint32_t> _rng;

BOOST_STATIC_CONSTEXPR unsigned long long ONE_SECOND = 1e6;

namespace pkmn { namespace calculations {

    static uint32_t get_gender_threshold(
        float chance_male
    ) {
        if(pkmn_floats_close(chance_male, 0.875f)) {
            return 31;
        } else if(pkmn_floats_close(chance_male, 0.75f)) {
            return 64;
        } else if(pkmn_floats_close(chance_male, 0.5f)) {
            return 127;
        } else {
            return 191;
        }
    }

    uint32_t generate_personality(
        const std::string &species,
        uint32_t trainer_id,
        bool shiny,
        const std::string &gender
    ) {
        unsigned long long before_timestamp = 0;
        unsigned long long after_timestamp = 0;
        uint32_t ret = 0;

        pkmn::database::pokemon_entry entry(species, "Omega Ruby", "");
        float chance_male = entry.get_chance_male();
        float chance_female = entry.get_chance_female();

        // Validate gender input
        if(pkmn_floats_close((chance_male + chance_female), 0.0f)) {
            if(gender != "Genderless") {
                throw std::invalid_argument("This Pokémon is genderless.");
            }
        } else if(pkmn_floats_close(chance_male, 1.0f)) {
            if(gender != "Male") {
                throw std::invalid_argument("This Pokémon is male-only.");
            }
        } else if(pkmn_floats_close(chance_female, 1.0f)) {
            if(gender != "Female") {
                throw std::invalid_argument("This Pokémon is female-only.");
            }
        } else if(gender != "Male" and gender != "Female") {
            throw std::invalid_argument("Valid genders: Male, Female");
        }

        before_timestamp = cal_highres_timestamp();
        uint32_t gender_threshold = get_gender_threshold(chance_male);
        bool found = false;
        do {
            ret = _rng.rand();

            // Set the gender if applicable.
            if(gender == "Male") {
                ret |= (_rng.rand() % (0xFF - gender_threshold) + gender_threshold);
            } else if(gender == "Female") {
                ret |= (_rng.rand() % gender_threshold);
            }

            if(modern_shiny(ret, trainer_id) == shiny) {
                found = true;
            }

            after_timestamp = cal_highres_timestamp();
        } while(not found and (after_timestamp - before_timestamp) < ONE_SECOND);

        if(not found) {
            throw std::runtime_error("Failed to generate a valid personality.");
        }

        return ret;
    }

    uint32_t personality_with_gen3_gen4_nature(
        const std::string &nature
    ) {
        auto iter = std::find(_natures.begin(), _natures.end(), nature);
        if(iter == _natures.end()) {
            throw std::invalid_argument("Invalid nature.");
        }

        uint32_t index = uint32_t(iter - _natures.begin());
        uint32_t ret = _rng.rand();
        while((ret % 25) != index) {
            ++ret;
        }

        return ret;
    }

    uint32_t personality_with_gender(
        const std::string &species,
        const std::string &gender
    ) {
        uint32_t ret = _rng.rand();

        pkmn::database::pokemon_entry entry(species, "Omega Ruby", "");
        float chance_male = entry.get_chance_male();
        float chance_female = entry.get_chance_female();

        if(pkmn_floats_close((chance_male + chance_female), 0.0f)) {
            if(gender != "Genderless") {
                throw std::invalid_argument("This Pokémon is genderless.");
            }
        } else if(pkmn_floats_close(chance_male, 1.0f)) {
            if(gender != "Male") {
                throw std::invalid_argument("This Pokémon is male-only.");
            }
        } else if(pkmn_floats_close(chance_female, 1.0f)) {
            if(gender != "Female") {
                throw std::invalid_argument("This Pokémon is female-only.");
            }
        } else {
            ret &= ~0xFF;
            uint32_t threshold = get_gender_threshold(chance_male);

            if(gender == "Male") {
                ret |= (_rng.rand() % (0xFF - threshold) + threshold);
            } else if(gender == "Female") {
                ret |= (_rng.rand() % threshold);
            } else {
                throw std::invalid_argument("Valid genders: Male, Female");
            }
        }

        return ret;
    }

    uint32_t personality_with_shininess(
        uint32_t trainer_id,
        bool shiny
    ) {
        uint32_t ret = _rng.rand();

        uint16_t* p = reinterpret_cast<uint16_t*>(&ret);
        const uint16_t* t = reinterpret_cast<const uint16_t*>(&trainer_id);

        if(shiny) {
            for(size_t i = 3; i < 16; ++i) {
                size_t num_ones = 0;
                if(p[0] & (1 << i)) ++num_ones;
                if(p[1] & (1 << i)) ++num_ones;
                if(t[0] & (1 << i)) ++num_ones;
                if(t[1] & (1 << i)) ++num_ones;
                if(num_ones % 2) {
                    p[0] ^= (1 << i);
                }
            }
        } else {
            size_t num_ones = 0;
            if(p[0] & (1 << 3)) ++num_ones;
            if(p[1] & (1 << 3)) ++num_ones;
            if(t[0] & (1 << 3)) ++num_ones;
            if(t[1] & (1 << 3)) ++num_ones;
            if(!(num_ones % 2)) {
                p[0] ^= (1 << 3);
            }
        }

        return ret;
    }

}}
