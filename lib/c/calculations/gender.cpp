/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "error_internal.hpp"

#include <pkmn-c/calculations/gender.h>

#include <pkmn/calculations/gender.hpp>

#include <boost/assign.hpp>

#include <map>

static const std::map<std::string, pkmn_gender_t> genders = \
    boost::assign::map_list_of<std::string, pkmn_gender_t>
        ("Male", PKMN_GENDER_MALE)
        ("Female", PKMN_GENDER_FEMALE)
        ("Genderless", PKMN_GENDER_GENDERLESS)
    ;

pkmn_error_t pkmn_calculations_gen2_pokemon_gender(
    const char* species,
    int IV_attack,
    pkmn_gender_t* gender_out
) {
    PKMN_CHECK_NULL_PARAM(species);
    PKMN_CHECK_NULL_PARAM(gender_out);

    PKMN_CPP_TO_C(
        *gender_out = genders.at(
                          pkmn::calculations::gen2_pokemon_gender(
                              species, IV_attack
                          )
                      );
    )
}

pkmn_error_t pkmn_calculations_modern_pokemon_gender(
    const char* species,
    uint32_t personality,
    pkmn_gender_t* gender_out
) {
    PKMN_CHECK_NULL_PARAM(species);
    PKMN_CHECK_NULL_PARAM(gender_out);

    PKMN_CPP_TO_C(
        *gender_out = genders.at(
                          pkmn::calculations::modern_pokemon_gender(
                              species, personality
                          )
                      );
    )
}
