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

struct pkmn_hidden_power
{
    char* type;
    int base_power;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_hidden_power_free(
    struct pkmn_hidden_power* hidden_power_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_HIDDEN_POWER_H */
