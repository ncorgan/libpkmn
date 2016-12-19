/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_STATS_H
#define PKMN_C_CALCULATIONS_STATS_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/types/int_pair.h>
#include <pkmn-c/types/stats.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_calculations_gb_stat(
    pkmn_stat_t stat,
    int level,
    int base_stat,
    int EV,
    int IV,
    int* stat_out
);

PKMN_API pkmn_error_t pkmn_calculations_gb_stat_range(
    pkmn_stat_t stat,
    int level,
    int base_stat,
    pkmn_int_pair_t* stat_range_out
);

PKMN_API pkmn_error_t pkmn_calculations_modern_stat(
    pkmn_stat_t stat,
    int level,
    float nature_modifier,
    int base_stat,
    int EV,
    int IV,
    int* stat_out
);

PKMN_API pkmn_error_t pkmn_calculations_modern_stat_range(
    pkmn_stat_t stat,
    int level,
    int base_stat,
    pkmn_int_pair_t* stat_range_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_STATS_H */
