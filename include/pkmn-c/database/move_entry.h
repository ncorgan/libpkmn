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

#include <pkmn-c/enums/game.h>

#include <stdbool.h>

struct pkmn_database_move_entry
{
    char* p_name;
    enum pkmn_game game;
    char* p_description;
    char* p_target;
    char* p_damage_class;
    int base_power;
    int pp[4];
    float accuracy;
    char* p_effect;
    char* p_contest_type;
    char* p_contest_effect;
    char* p_super_contest_effect;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_database_get_move_entry(
    const char* p_move_name,
    enum pkmn_game game,
    struct pkmn_database_move_entry* p_move_entry_out
);

PKMN_C_API enum pkmn_error pkmn_database_move_entry_free(
    struct pkmn_database_move_entry* p_move_entry
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_DATABASE_MOVE_ENTRY_H */
