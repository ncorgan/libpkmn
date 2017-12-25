/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_NATURAL_GIFT_H
#define PKMN_C_TYPES_NATURAL_GIFT_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

typedef struct
{
    char* type;
    int base_power;
} pkmn_natural_gift_t;

#ifdef __cplusplus
extern "C" {
#endif

static inline pkmn_error_t pkmn_natural_gift_free(
    pkmn_natural_gift_t* natural_gift_ptr
)
{
    if(!natural_gift_ptr)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    free(natural_gift_ptr->type);
    natural_gift_ptr->type = NULL;
    natural_gift_ptr->base_power = 0;

    return PKMN_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_NATURAL_GIFT_H */
