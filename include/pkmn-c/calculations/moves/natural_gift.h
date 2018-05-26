/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_MODIFIERS_MOVES_H
#define PKMN_C_CALCULATIONS_MODIFIERS_MOVES_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/types/natural_gift.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_calculations_natural_gift_stats(
    const char* p_item_name,
    int generation,
    struct pkmn_natural_gift* p_natural_gift_stats_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_MODIFIERS_MOVES_H */
