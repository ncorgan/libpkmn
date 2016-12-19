/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_DATABASE_ITEM_ENTRY_H
#define PKMN_C_DATABASE_ITEM_ENTRY_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdbool.h>

typedef struct {
    char* name;
    char* game;
    char* category;
    char* pocket;
    char* description;
    int cost;
    bool holdable;
    int fling_power;
    char* fling_effect;
} pkmn_database_item_entry_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_database_get_item_entry(
    const char* item_name,
    const char* item_game,
    pkmn_database_item_entry_t* item_entry_out
);

PKMN_API pkmn_error_t pkmn_database_item_entry_free(
    pkmn_database_item_entry_t* item_entry
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_DATABASE_ITEM_ENTRY_H */
