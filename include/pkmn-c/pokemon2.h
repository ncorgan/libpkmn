/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEMON2_H
#define PKMN_C_POKEMON2_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/database/pokemon_entry.h>

#include <pkmn-c/types/attribute_names.h>
#include <pkmn-c/types/marking.h>
#include <pkmn-c/types/move_slot.h>
#include <pkmn-c/types/pokemon_info_structs.h>
#include <pkmn-c/types/stats.h>

typedef struct
{
    char* species;
    char* game;

    void* _internal;
} pkmn_pokemon2_t;

#ifdef __cplusplus
extern "C" {
#endif

pkmn_trainer_id2_t pkmn_pokemon2_default_trainer_id();

const char* pkmn_pokemon2_default_trainer_name();

PKMN_C_API pkmn_error_t pkmn_pokemon2_init(
    const char* species,
    const char* game,
    const char* form,
    int level,
    pkmn_pokemon2_t* pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_init_from_file(
    const char* filepath,
    pkmn_pokemon2_t* pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_free(
    pkmn_pokemon2_t* pokemon_ptr
);

PKMN_C_API const char* pkmn_pokemon2_strerror(
    pkmn_pokemon2_t* pokemon_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_to_game(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* game,
    pkmn_pokemon2_t* new_pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_export_to_file(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* filepath
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_database_entry(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_database_pokemon_entry_t* database_entry_ptr
);

// Common but non-static values

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_form(
    pkmn_pokemon2_t* pokemon_ptr,
    char* form_buffer,
    size_t form_buffer_len,
    size_t* form_length_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_form(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* form
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_level(
    pkmn_pokemon2_t* pokemon_ptr,
    int* level_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_level(
    pkmn_pokemon2_t* pokemon_ptr,
    int level
);

// Pokémon info

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_pokemon_info(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_pokemon_info_t* pokemon_info_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_is_egg(
    pkmn_pokemon2_t* pokemon_ptr,
    bool is_egg
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_pokerus_duration(
    pkmn_pokemon2_t* pokemon_ptr,
    int pokerus_duration
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_personality(
    pkmn_pokemon2_t* pokemon_ptr,
    uint32_t personality
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_gender(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_gender_t gender
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_ability(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* ability
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_nickname(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* nickname
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_held_item(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* held_item
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_experience(
    pkmn_pokemon2_t* pokemon_ptr,
    int experience
);

// Pokémon origin info

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_pokemon_origin_info(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_pokemon_origin_info_t* pokemon_origin_info_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_original_trainer_name(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* original_trainer_name
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_original_trainer_id(
    pkmn_pokemon2_t* pokemon_ptr,
    uint32_t original_trainer_id
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_original_trainer_public_id(
    pkmn_pokemon2_t* pokemon_ptr,
    uint16_t original_trainer_public_id
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_original_trainer_secret_id(
    pkmn_pokemon2_t* pokemon_ptr,
    uint16_t original_trainer_secret_id
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_original_game(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* game
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_ball(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* ball
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_location_met(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* location_met,
    bool as_egg
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_level_met(
    pkmn_pokemon2_t* pokemon_ptr,
    int level_met
);

// Moves

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_moves(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_move_slots_t* move_slots_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_move(
    pkmn_pokemon2_t* pokemon_ptr,
    size_t index,
    const char* move
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_move_pp(
    pkmn_pokemon2_t* pokemon_ptr,
    size_t index,
    int pp
);

// Stats

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_EVs(
    pkmn_pokemon2_t* pokemon_ptr,
    int* EVs_buffer_out,
    size_t stat_buffer_size,
    size_t* actual_num_EVs_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_EV(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_stat_t stat,
    int value
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_IVs(
    pkmn_pokemon2_t* pokemon_ptr,
    int* IVs_buffer_out,
    size_t stat_buffer_size,
    size_t* actual_num_IVs_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_IV(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_stat_t stat,
    int value
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_stats(
    pkmn_pokemon2_t* pokemon_ptr,
    int* stats_buffer_out,
    size_t stat_buffer_size,
    size_t* actual_num_stats_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_contest_stats(
    pkmn_pokemon2_t* pokemon_ptr,
    int* contest_stats_buffer_out,
    size_t contest_stat_buffer_size,
    size_t* actual_num_contest_stats_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_contest_stat(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_contest_stat_t contest_stat,
    int value
);

// Misc

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_markings(
    pkmn_pokemon2_t* pokemon_ptr,
    bool* has_markings_buffer_out,
    size_t has_marking_buffer_size,
    size_t* actual_num_markings_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_has_marking(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_marking_t marking,
    bool has_marking
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_has_ribbon(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* ribbon_name,
    bool* has_ribbon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_has_ribbon(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* ribbon_name,
    bool has_ribbon
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_ribbon_names(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_string_list_t* ribbon_names_out
);

// Battle info

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_battle_info(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_pokemon_battle_info_t* pokemon_battle_info_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_current_hp(
    pkmn_pokemon2_t* pokemon_ptr,
    int current_hp
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_condition(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_condition_t condition
);

// Attributes

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_numeric_attribute(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* attribute_name,
    int* value_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_numeric_attribute(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* attribute_name,
    int value
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_string_attribute(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* attribute_name,
    char* value_out,
    size_t value_buffer_len,
    size_t* actual_value_len_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_set_string_attribute(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* attribute_name,
    const char* value
);

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_attribute_names(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_attribute_names_t* attribute_names_out
);

// Filepaths

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_filepaths(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_pokemon_filepaths_t* filepaths_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON2_H */
