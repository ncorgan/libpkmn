/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_DATABASE_MOVE_ENTRY_H
#define PKMN_C_DATABASE_MOVE_ENTRY_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdbool.h>

struct pkmn_database_move_entry
{
    char* name;
    char* game;
    char* description;
    char* target;
    char* damage_class;
    int base_power;
    int pp[4];
    float accuracy;
    char* effect;
    char* contest_type;
    char* contest_effect;
    char* super_contest_effect;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_database_get_move_entry(
    const char* move_name,
    const char* move_game,
    struct pkmn_database_move_entry* move_entry_out
);

PKMN_C_API enum pkmn_error pkmn_database_move_entry_free(
    struct pkmn_database_move_entry* move_entry
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_DATABASE_MOVE_ENTRY_H */
