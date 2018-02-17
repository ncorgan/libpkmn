/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_HIDDEN_POWER_H
#define PKMN_C_TYPES_HIDDEN_POWER_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

typedef struct
{
    char* type;
    int base_power;
} pkmn_hidden_power_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API pkmn_error_t pkmn_hidden_power_free(
    pkmn_hidden_power_t* hidden_power_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_HIDDEN_POWER_H */
