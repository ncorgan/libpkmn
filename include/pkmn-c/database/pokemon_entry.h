/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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

#define PKMN_C_NUM_STATS 7

struct pkmn_database_pokemon_entry
{
    char* p_name;
    char* p_game;
    char* p_species;
    char* p_form;
    char* p_pokedex_entry;
    float height;
    float weight;
    float chance_male;
    float chance_female;
    bool has_gender_differences;
    int base_friendship;
    struct pkmn_string_pair types;
    struct pkmn_string_pair abilities;
    char* p_hidden_ability;
    struct pkmn_string_pair egg_groups;
    int base_stats[PKMN_C_NUM_STATS];
    int EV_yields[PKMN_C_NUM_STATS];
    int experience_yield;
    struct pkmn_levelup_moves levelup_moves;
    struct pkmn_string_list tm_hm_moves;
    struct pkmn_string_list egg_moves;
    struct pkmn_string_list tutor_moves;
    struct pkmn_string_list forms;
    struct pkmn_string_list evolutions;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_database_get_pokemon_entry(
    const char* p_species,
    const char* p_game,
    const char* p_form,
    struct pkmn_database_pokemon_entry* p_pokemon_entry_out
);

PKMN_C_API enum pkmn_error pkmn_database_pokemon_entry_set_form(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    const char* p_form
);

PKMN_C_API enum pkmn_error pkmn_database_pokemon_entry_experience_at_level(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    int level,
    int* p_experience_out
);

PKMN_C_API enum pkmn_error pkmn_database_pokemon_entry_level_at_experience(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    int experience,
    int* p_level_out
);

PKMN_C_API enum pkmn_error pkmn_database_pokemon_entry_icon_filepath(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    bool shiny,
    char* p_icon_filepath_out,
    size_t buffer_len,
    size_t* p_icon_filepath_length_out
);

PKMN_C_API enum pkmn_error pkmn_database_pokemon_entry_sprite_filepath(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    bool female,
    bool shiny,
    char* p_sprite_filepath_out,
    size_t buffer_len,
    size_t* p_sprite_filepath_length_out
);

PKMN_C_API enum pkmn_error pkmn_database_pokemon_entry_free(
    struct pkmn_database_pokemon_entry* p_pokemon_entry
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_DATABASE_POKEMON_ENTRY_H */
