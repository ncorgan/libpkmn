/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_NATURAL_GIFT_H
#define PKMN_C_TYPES_NATURAL_GIFT_H

#include <pkmn-c/config.h>

#include <stdlib.h>

typedef struct {
    char* type;
    int base_power;
} pkmn_natural_gift_t;

#ifdef __cplusplus
extern "C" {
#endif

static PKMN_INLINE void pkmn_natural_gift_free(
    pkmn_natural_gift_t* natural_gift
) {
    free(natural_gift->type);
    natural_gift->type = NULL;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_NATURAL_GIFT_H */
