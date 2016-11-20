/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_FORM_H
#define PKMN_C_CALCULATIONS_FORM_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_calculations_gen2_unown_form(
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_special,
    char* form_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_calculations_gen3_unown_form(
    uint32_t personality,
    char* form_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_calculations_wurmple_becomes_silcoon(
    uint32_t personality,
    bool before_gen5,
    bool* evolves_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_FORM_H */
