/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_HIDDEN_POWER_H
#define PKMN_C_TYPES_HIDDEN_POWER_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

typedef struct
{
    char* type;
    int base_power;
} pkmn_hidden_power_t;

#ifdef __cplusplus
extern "C" {
#endif

static inline pkmn_error_t pkmn_hidden_power_free(
    pkmn_hidden_power_t* hidden_power_ptr
)
{
    if(!hidden_power_ptr)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    free(hidden_power_ptr->type);
    hidden_power_ptr->type = NULL;
    hidden_power_ptr->base_power = 0;

    return PKMN_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_HIDDEN_POWER_H */
