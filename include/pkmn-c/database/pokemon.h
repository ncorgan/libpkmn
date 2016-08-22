/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_DATABASE_POKEMON_H
#define PKMN_C_DATABASE_POKEMON_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>
#include <pkmn-c/types/levelup_move.h>
#include <pkmn-c/types/string_types.h>

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_database_pokemon_species(
    const char* pokemon_name,
    const char* game_name,
    char* pokemon_species_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_pokedex_entry(
    const char* pokemon_name,
    const char* game_name,
    char* pokemon_pokedex_entry_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_height(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    float* height_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_weight(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    float* weight_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_chance_male(
    const char* pokemon_name,
    const char* game_name,
    float* chance_male_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_chance_female(
    const char* pokemon_name,
    const char* game_name,
    float* chance_female_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_has_gender_differences(
    const char* pokemon_name,
    const char* game_name,
    bool* has_gender_differences_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_base_happiness(
    const char* pokemon_name,
    const char* game_name,
    int* base_happiness_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_get_types(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    pkmn_string_pair_t* types_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_get_abilities(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    pkmn_string_pair_t* abilities_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_hidden_ability(
    const char* pokemon_name,
    const char* game_name,
    char* pokemon_hidden_ability_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_get_egg_groups(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    pkmn_string_pair_t* egg_groups_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_base_stat(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    const char* stat_name,
    int* base_stat_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_EV_yield(
    const char* pokemon_name,
    const char* game_name,
    const char* stat_name,
    int* EV_yield_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_experience_yield(
    const char* pokemon_name,
    const char* game_name,
    int* experience_yield_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_experience_at_level(
    const char* pokemon_name,
    const char* game_name,
    int level,
    int* experience_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_level_at_experience(
    const char* pokemon_name,
    const char* game_name,
    int experience,
    int* level_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_levelup_moves(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    pkmn_levelup_moves_t* levelup_moves_out,
    size_t* list_length_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_tm_hm_moves(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    pkmn_string_list_t* tm_hm_moves_out,
    size_t* list_length_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_egg_moves(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    pkmn_string_list_t* egg_moves_out,
    size_t* list_length_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_tutor_moves(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    pkmn_string_list_t* tutor_moves_out,
    size_t* list_length_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_forms(
    const char* pokemon_name,
    const char* game_name,
    pkmn_string_list_t* forms_out,
    size_t* list_length_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_evolutions(
    const char* pokemon_name,
    const char* game_name,
    pkmn_string_list_t* evolutions_out,
    size_t* list_length_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_DATABASE_POKEMON_H */
