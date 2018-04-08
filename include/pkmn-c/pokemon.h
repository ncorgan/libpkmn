/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEMON_H
#define PKMN_C_POKEMON_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/database/pokemon_entry.h>

#include <pkmn-c/types/attribute_names.h>
#include <pkmn-c/types/condition.h>
#include <pkmn-c/types/marking.h>
#include <pkmn-c/types/move_slot.h>
#include <pkmn-c/types/stats.h>
#include <pkmn-c/types/trainer_info.h>

typedef struct
{
    char* species;
    char* game;

    void* _internal;
} pkmn_pokemon_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API union pkmn_trainer_id pkmn_pokemon_default_trainer_id();

PKMN_C_API const char* pkmn_pokemon_default_trainer_name();

PKMN_C_API pkmn_error_t pkmn_pokemon_init(
    const char* species,
    const char* game,
    const char* form,
    int level,
    pkmn_pokemon_t* pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_init_from_file(
    const char* filepath,
    pkmn_pokemon_t* pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_free(
    pkmn_pokemon_t* pokemon_ptr
);

PKMN_C_API const char* pkmn_pokemon_strerror(
    pkmn_pokemon_t* pokemon_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon_to_game(
    pkmn_pokemon_t* pokemon_ptr,
    const char* game,
    pkmn_pokemon_t* new_pokemon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_export_to_file(
    pkmn_pokemon_t* pokemon_ptr,
    const char* filepath
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_form(
    pkmn_pokemon_t* pokemon_ptr,
    char* form_buffer,
    size_t form_buffer_len,
    size_t* form_length_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_form(
    pkmn_pokemon_t* pokemon_ptr,
    const char* form
);

PKMN_C_API pkmn_error_t pkmn_pokemon_is_egg(
    pkmn_pokemon_t* pokemon_ptr,
    bool* is_egg_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_is_egg(
    pkmn_pokemon_t* pokemon_ptr,
    bool is_egg
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_database_entry(
    pkmn_pokemon_t* pokemon_ptr,
    pkmn_database_pokemon_entry_t* database_entry_ptr
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_condition(
    pkmn_pokemon_t* pokemon_ptr,
    enum pkmn_condition* condition_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_condition(
    pkmn_pokemon_t* pokemon_ptr,
    enum pkmn_condition condition
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_nickname(
    pkmn_pokemon_t* pokemon_ptr,
    char* nickname_out,
    size_t nickname_buffer_len,
    size_t* actual_nickname_len_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_nickname(
    pkmn_pokemon_t* pokemon_ptr,
    const char* nickname
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_gender(
    pkmn_pokemon_t* pokemon_ptr,
    enum pkmn_gender* gender_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_gender(
    pkmn_pokemon_t* pokemon_ptr,
    enum pkmn_gender gender
);

PKMN_C_API pkmn_error_t pkmn_pokemon_is_shiny(
    pkmn_pokemon_t* pokemon_ptr,
    bool* is_shiny_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_is_shiny(
    pkmn_pokemon_t* pokemon_ptr,
    bool is_shiny
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_held_item(
    pkmn_pokemon_t* pokemon_ptr,
    char* held_item_out,
    size_t held_item_buffer_len,
    size_t* actual_held_item_len_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_held_item(
    pkmn_pokemon_t* pokemon_ptr,
    const char* held_item
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_pokerus_duration(
    pkmn_pokemon_t* pokemon_ptr,
    int* pokerus_duration_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_pokerus_duration(
    pkmn_pokemon_t* pokemon_ptr,
    int pokerus_duration
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_original_trainer_info(
    pkmn_pokemon_t* pokemon_ptr,
    struct pkmn_trainer_info* original_trainer_info_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_original_trainer_name(
    pkmn_pokemon_t* pokemon_ptr,
    const char* original_trainer_name
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_original_trainer_public_id(
    pkmn_pokemon_t* pokemon_ptr,
    uint16_t original_trainer_public_id
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_original_trainer_secret_id(
    pkmn_pokemon_t* pokemon_ptr,
    uint16_t original_trainer_secret_id
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_original_trainer_id(
    pkmn_pokemon_t* pokemon_ptr,
    uint32_t original_trainer_id
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_original_trainer_gender(
    pkmn_pokemon_t* pokemon_ptr,
    enum pkmn_gender original_trainer_gender
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_current_trainer_friendship(
    pkmn_pokemon_t* pokemon_ptr,
    int* current_trainer_friendship_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_current_trainer_friendship(
    pkmn_pokemon_t* pokemon_ptr,
    int current_trainer_friendship
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_ability(
    pkmn_pokemon_t* pokemon_ptr,
    char* ability_out,
    size_t ability_buffer_len,
    size_t* actual_ability_len_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_ability(
    pkmn_pokemon_t* pokemon_ptr,
    const char* ability
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_ball(
    pkmn_pokemon_t* pokemon_ptr,
    char* ball_out,
    size_t ball_buffer_len,
    size_t* actual_ball_len_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_ball(
    pkmn_pokemon_t* pokemon_ptr,
    const char* ball
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_level_met(
    pkmn_pokemon_t* pokemon_ptr,
    int* level_met_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_level_met(
    pkmn_pokemon_t* pokemon_ptr,
    int level_met
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_location_met(
    pkmn_pokemon_t* pokemon_ptr,
    bool as_egg,
    char* location_met_out,
    size_t location_met_buffer_len,
    size_t* actual_location_met_len_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_location_met(
    pkmn_pokemon_t* pokemon_ptr,
    const char* location_met,
    bool as_egg
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_original_game(
    pkmn_pokemon_t* pokemon_ptr,
    char* original_game_out,
    size_t original_game_buffer_len,
    size_t* actual_original_game_len_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_original_game(
    pkmn_pokemon_t* pokemon_ptr,
    const char* game
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_personality(
    pkmn_pokemon_t* pokemon_ptr,
    uint32_t* personality_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_personality(
    pkmn_pokemon_t* pokemon_ptr,
    uint32_t personality
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_experience(
    pkmn_pokemon_t* pokemon_ptr,
    int* experience_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_experience(
    pkmn_pokemon_t* pokemon_ptr,
    int experience
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_level(
    pkmn_pokemon_t* pokemon_ptr,
    int* level_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_level(
    pkmn_pokemon_t* pokemon_ptr,
    int level
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_markings(
    pkmn_pokemon_t* pokemon_ptr,
    bool* has_markings_buffer_out,
    size_t has_marking_buffer_size,
    size_t* actual_num_markings_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_has_marking(
    pkmn_pokemon_t* pokemon_ptr,
    enum pkmn_marking marking,
    bool has_marking
);

PKMN_C_API pkmn_error_t pkmn_pokemon_has_ribbon(
    pkmn_pokemon_t* pokemon_ptr,
    const char* ribbon_name,
    bool* has_ribbon_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_has_ribbon(
    pkmn_pokemon_t* pokemon_ptr,
    const char* ribbon_name,
    bool has_ribbon
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_ribbon_names(
    pkmn_pokemon_t* pokemon_ptr,
    struct pkmn_string_list* ribbon_names_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_contest_stats(
    pkmn_pokemon_t* pokemon_ptr,
    int* contest_stats_buffer_out,
    size_t contest_stat_buffer_size,
    size_t* actual_num_contest_stats_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_contest_stat(
    pkmn_pokemon_t* pokemon_ptr,
    enum pkmn_contest_stat contest_stat,
    int value
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_moves(
    pkmn_pokemon_t* pokemon_ptr,
    struct pkmn_move_slots* move_slots_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_move(
    pkmn_pokemon_t* pokemon_ptr,
    size_t index,
    const char* move
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_move_pp(
    pkmn_pokemon_t* pokemon_ptr,
    size_t index,
    int pp
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_EVs(
    pkmn_pokemon_t* pokemon_ptr,
    int* EVs_buffer_out,
    size_t stat_buffer_size,
    size_t* actual_num_EVs_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_EV(
    pkmn_pokemon_t* pokemon_ptr,
    enum pkmn_stat stat,
    int value
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_IVs(
    pkmn_pokemon_t* pokemon_ptr,
    int* IVs_buffer_out,
    size_t stat_buffer_size,
    size_t* actual_num_IVs_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_IV(
    pkmn_pokemon_t* pokemon_ptr,
    enum pkmn_stat stat,
    int value
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_stats(
    pkmn_pokemon_t* pokemon_ptr,
    int* stats_buffer_out,
    size_t stat_buffer_size,
    size_t* actual_num_stats_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_current_hp(
    pkmn_pokemon_t* pokemon_ptr,
    int* current_hp_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_current_hp(
    pkmn_pokemon_t* pokemon_ptr,
    int current_hp
);

// Filepaths

PKMN_C_API pkmn_error_t pkmn_pokemon_get_icon_filepath(
    pkmn_pokemon_t* pokemon_ptr,
    char* icon_filepath_out,
    size_t icon_filepath_buffer_len,
    size_t* actual_icon_filepath_len_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_sprite_filepath(
    pkmn_pokemon_t* pokemon_ptr,
    char* sprite_filepath_out,
    size_t sprite_filepath_buffer_len,
    size_t* actual_sprite_filepath_len_out
);

// Attributes

PKMN_C_API pkmn_error_t pkmn_pokemon_get_numeric_attribute(
    pkmn_pokemon_t* pokemon_ptr,
    const char* attribute_name,
    int* value_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_numeric_attribute(
    pkmn_pokemon_t* pokemon_ptr,
    const char* attribute_name,
    int value
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_string_attribute(
    pkmn_pokemon_t* pokemon_ptr,
    const char* attribute_name,
    char* value_out,
    size_t value_buffer_len,
    size_t* actual_value_len_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_string_attribute(
    pkmn_pokemon_t* pokemon_ptr,
    const char* attribute_name,
    const char* value
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_boolean_attribute(
    pkmn_pokemon_t* pokemon_ptr,
    const char* attribute_name,
    bool* value_out
);

PKMN_C_API pkmn_error_t pkmn_pokemon_set_boolean_attribute(
    pkmn_pokemon_t* pokemon_ptr,
    const char* attribute_name,
    bool value
);

PKMN_C_API pkmn_error_t pkmn_pokemon_get_attribute_names(
    pkmn_pokemon_t* pokemon_ptr,
    struct pkmn_attribute_names* attribute_names_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_H */
