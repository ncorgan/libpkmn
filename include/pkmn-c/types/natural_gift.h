/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_NATURAL_GIFT_H
#define PKMN_C_TYPES_NATURAL_GIFT_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

struct pkmn_natural_gift
{
    char* p_type;
    int base_power;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_natural_gift_free(
    struct pkmn_natural_gift* p_natural_gift
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_NATURAL_GIFT_H */
