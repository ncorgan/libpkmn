/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "error_internal.hpp"

#include <pkmn-c/calculations/shininess.h>

#include <pkmn/calculations/shininess.hpp>

enum pkmn_error pkmn_calculations_gen2_shiny(
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_special,
    bool* p_is_shiny_out
)
{
    PKMN_CHECK_NULL_PARAM(p_is_shiny_out);

    PKMN_CPP_TO_C(
        *p_is_shiny_out = pkmn::calculations::gen2_shiny(
                              IV_attack, IV_defense,
                              IV_speed, IV_special
                          );
    )
}

enum pkmn_error pkmn_calculations_modern_shiny(
    uint32_t personality,
    uint32_t trainer_id,
    bool* p_is_shiny_out
)
{
    PKMN_CHECK_NULL_PARAM(p_is_shiny_out);

    PKMN_CPP_TO_C(
        *p_is_shiny_out = pkmn::calculations::modern_shiny(
                              personality,
                              trainer_id
                          );
    )
}
