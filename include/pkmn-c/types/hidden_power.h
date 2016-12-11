/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_HIDDEN_POWER_H
#define PKMN_C_TYPES_HIDDEN_POWER_H

#include <pkmn-c/config.h>

#include <stdlib.h>

typedef struct {
    char* type;
    int base_power;
} pkmn_hidden_power_t;

#ifdef __cplusplus
extern "C" {
#endif

static PKMN_INLINE void pkmn_hidden_power_free(
    pkmn_hidden_power_t* hidden_power
) {
    free(hidden_power->type);
    hidden_power->type = NULL;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_HIDDEN_POWER_H */
