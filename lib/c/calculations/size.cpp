/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "error_internal.hpp"

#include <pkmn-c/calculations/size.h>

#include <pkmn/calculations/size.hpp>

enum pkmn_error pkmn_calculations_pokemon_size(
    const char* p_species,
    uint32_t personality,
    int IV_HP,
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_spatk,
    int IV_spdef,
    float* p_size_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_species,
                "p_species"
            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_size_out,
                    "p_size_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            *p_size_out = pkmn::calculations::pokemon_size(
                              p_species,
                              personality,
                              IV_HP,
                              IV_attack,
                              IV_defense,
                              IV_speed,
                              IV_spatk,
                              IV_spdef
                          );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}
