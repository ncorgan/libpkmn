/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_DATABASE_POKEMON_ENTRY_H
#define PKMN_C_DATABASE_POKEMON_ENTRY_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>
#include <pkmn-c/types/levelup_move.h>
#include <pkmn-c/types/stats.h>
#include <pkmn-c/types/string_types.h>

#include <stdbool.h>

typedef struct {
    char* name;
    char* game;
    char* species;
    char* form;
    char* pokedex_entry;
    float height;
    float weight;
    float chance_male;
    float chance_female;
    bool has_gender_differences;
    int base_friendship;
    pkmn_string_pair_t types;
    pkmn_string_pair_t abilities;
    char* hidden_ability;
    pkmn_string_pair_t egg_groups;
    int base_stats[7];
    int EV_yields[7];
    int experience_yield;
    pkmn_levelup_moves_t levelup_moves;
    pkmn_string_list_t tm_hm_moves;
    pkmn_string_list_t egg_moves;
    pkmn_string_list_t tutor_moves;
    pkmn_string_list_t forms;
    pkmn_string_list_t evolutions;
} pkmn_database_pokemon_entry_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_database_get_pokemon_entry(
    const char* species,
    const char* game,
    const char* form,
    pkmn_database_pokemon_entry_t* pokemon_entry_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_entry_set_form(
    pkmn_database_pokemon_entry_t* pokemon_entry,
    const char* form
);

PKMN_API pkmn_error_t pkmn_database_pokemon_entry_experience_at_level(
    pkmn_database_pokemon_entry_t* pokemon_entry,
    int level,
    int* experience_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_entry_level_at_experience(
    pkmn_database_pokemon_entry_t* pokemon_entry,
    int experience,
    int* level_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_entry_icon_filepath(
    pkmn_database_pokemon_entry_t* pokemon_entry,
    bool shiny,
    char* icon_filepath_out,
    size_t buffer_len
);

PKMN_API pkmn_error_t pkmn_database_pokemon_entry_sprite_filepath(
    pkmn_database_pokemon_entry_t* pokemon_entry,
    bool female,
    bool shiny,
    char* sprite_filepath_out,
    size_t buffer_len
);

PKMN_API pkmn_error_t pkmn_database_pokemon_entry_free(
    pkmn_database_pokemon_entry_t* pokemon_entry
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_DATABASE_POKEMON_ENTRY_H */
