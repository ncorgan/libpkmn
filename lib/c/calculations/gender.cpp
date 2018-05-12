/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "enum_maps.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/gender.h>

#include <pkmn/calculations/gender.hpp>

#include <boost/assert.hpp>
#include <boost/assign.hpp>

enum pkmn_error pkmn_calculations_gen2_pokemon_gender(
    const char* p_species,
    int IV_attack,
    enum pkmn_gender* p_gender_out
)
{
    PKMN_CHECK_NULL_PARAM(p_species);
    PKMN_CHECK_NULL_PARAM(p_gender_out);

    PKMN_CPP_TO_C(
        const pkmn::c::gender_bimap_t& gender_bimap = pkmn::c::get_gender_bimap();
        auto gender_iter = gender_bimap.left.find(
                               pkmn::calculations::gen2_pokemon_gender(
                                   p_species, IV_attack
                               )
                           );
        BOOST_ASSERT(gender_iter != gender_bimap.left.end());

        *p_gender_out = gender_iter->second;
    )
}

enum pkmn_error pkmn_calculations_modern_pokemon_gender(
    const char* p_species,
    uint32_t personality,
    enum pkmn_gender* p_gender_out
)
{
    PKMN_CHECK_NULL_PARAM(p_species);
    PKMN_CHECK_NULL_PARAM(p_gender_out);

    PKMN_CPP_TO_C(
        const pkmn::c::gender_bimap_t& gender_bimap = pkmn::c::get_gender_bimap();
        auto gender_iter = gender_bimap.left.find(
                               pkmn::calculations::modern_pokemon_gender(
                                   p_species, personality
                               )
                           );
        BOOST_ASSERT(gender_iter != gender_bimap.left.end());

        *p_gender_out = gender_iter->second;
    )
}
