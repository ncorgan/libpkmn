/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_STAT_LIST_H
#define PKMN_C_TYPES_STAT_LIST_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

struct pkmn_stat_list
{
    int* p_stats;
    size_t num_stats;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_stat_list_free(
    struct pkmn_stat_list* p_stat_list
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_STAT_LIST_H */
