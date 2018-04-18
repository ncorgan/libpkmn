/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "error_internal.hpp"

#include <pkmn-c/calculations/personality.h>

#include <pkmn/calculations/personality.hpp>

#include <boost/assign.hpp>

#include <vector>

static const std::vector<std::string> gender_strings = {
    "Male", "Female", "Genderless"
};

enum pkmn_error pkmn_calculations_generate_personality(
    const char* species,
    uint32_t trainer_id,
    bool shiny,
    const char* ability,
    enum pkmn_gender gender,
    const char* nature,
    uint32_t* personality_out
) {
    PKMN_CHECK_NULL_PARAM(species);
    PKMN_CHECK_NULL_PARAM(ability);
    PKMN_CHECK_NULL_PARAM(nature);
    PKMN_CHECK_NULL_PARAM(personality_out);

    PKMN_CPP_TO_C(
        *personality_out = pkmn::calculations::generate_personality(
                               species,
                               trainer_id,
                               shiny,
                               ability,
                               gender_strings.at(gender),
                               nature
                           );
    )
}
