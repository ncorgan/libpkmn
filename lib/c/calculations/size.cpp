/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "error_internal.hpp"

#include <pkmn-c/calculations/size.h>

#include <pkmn/calculations/size.hpp>

pkmn_error_t pkmn_calculations_pokemon_size(
    const char* species,
    uint32_t personality,
    int IV_HP,
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_spatk,
    int IV_spdef,
    float* size_out
) {
    PKMN_CHECK_NULL_PARAM(species);
    PKMN_CHECK_NULL_PARAM(size_out);

    PKMN_CPP_TO_C(
        *size_out = pkmn::calculations::pokemon_size(
                        species,
                        personality,
                        IV_HP,
                        IV_attack,
                        IV_defense,
                        IV_speed,
                        IV_spatk,
                        IV_spdef
                    );
    )
}
