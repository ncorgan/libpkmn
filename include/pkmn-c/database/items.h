/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_DATABASE_ITEMS_H
#define PKMN_C_DATABASE_ITEMS_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_database_item_category(
    const char* item_name,
    const char* game_name,
    char* item_category_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_database_item_pocket(
    const char* item_name,
    const char* game_name,
    char* item_pocket_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_database_item_description(
    const char* item_name,
    const char* game_name,
    char* item_description_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_database_item_cost(
    const char* item_name,
    const char* game_name,
    int* item_cost_out
);

PKMN_API pkmn_error_t pkmn_database_item_holdable(
    const char* item_name,
    const char* game_name,
    bool* item_holdable_out
);

PKMN_API pkmn_error_t pkmn_database_item_fling_power(
    const char* item_name,
    const char* game_name,
    int* item_fling_power_out
);

PKMN_API pkmn_error_t pkmn_database_item_fling_effect(
    const char* item_name,
    const char* game_name,
    char* item_fling_effect_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_DATABASE_ITEMS_H */
