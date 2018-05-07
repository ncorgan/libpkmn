/*
 * p_Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
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

struct pkmn_pokemon
{
    char* p_species;
    char* p_game;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API union pkmn_trainer_id pkmn_pokemon_default_trainer_id();

PKMN_C_API const char* pkmn_pokemon_default_trainer_name();

PKMN_C_API enum pkmn_error pkmn_pokemon_init(
    const char* p_species,
    const char* p_game,
    const char* p_form,
    int level,
    struct pkmn_pokemon* p_pokemon_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_init_from_file(
    const char* p_filepath,
    struct pkmn_pokemon* p_pokemon_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_free(
    struct pkmn_pokemon* p_pokemon
);

PKMN_C_API const char* pkmn_pokemon_strerror(
    struct pkmn_pokemon* p_pokemon
);

PKMN_C_API enum pkmn_error pkmn_pokemon_to_game(
    struct pkmn_pokemon* p_pokemon,
    const char* p_game,
    struct pkmn_pokemon* p_new_pokemon_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_export_to_file(
    struct pkmn_pokemon* p_pokemon,
    const char* p_filepath
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_form(
    struct pkmn_pokemon* p_pokemon,
    char* p_form_buffer,
    size_t form_buffer_len,
    size_t* p_form_length_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_form(
    struct pkmn_pokemon* p_pokemon,
    const char* p_form
);

PKMN_C_API enum pkmn_error pkmn_pokemon_is_egg(
    struct pkmn_pokemon* p_pokemon,
    bool* p_is_egg_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_is_egg(
    struct pkmn_pokemon* p_pokemon,
    bool is_egg
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_database_entry(
    struct pkmn_pokemon* p_pokemon,
    struct pkmn_database_pokemon_entry* p_database_entry
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_condition(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_condition* p_condition_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_condition(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_condition condition
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_nickname(
    struct pkmn_pokemon* p_pokemon,
    char* p_nickname_out,
    size_t nickname_buffer_len,
    size_t* p_actual_nickname_len_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_nickname(
    struct pkmn_pokemon* p_pokemon,
    const char* p_nickname
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_gender(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_gender* p_gender_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_gender(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_gender gender
);

PKMN_C_API enum pkmn_error pkmn_pokemon_is_shiny(
    struct pkmn_pokemon* p_pokemon,
    bool* p_is_shiny_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_is_shiny(
    struct pkmn_pokemon* p_pokemon,
    bool is_shiny
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_held_item(
    struct pkmn_pokemon* p_pokemon,
    char* p_held_item_out,
    size_t held_item_buffer_len,
    size_t* p_actual_held_item_len_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_held_item(
    struct pkmn_pokemon* p_pokemon,
    const char* p_held_item
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_pokerus_duration(
    struct pkmn_pokemon* p_pokemon,
    int* p_pokerus_duration_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_pokerus_duration(
    struct pkmn_pokemon* p_pokemon,
    int pokerus_duration
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_original_trainer_info(
    struct pkmn_pokemon* p_pokemon,
    struct pkmn_trainer_info* p_original_trainer_info_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_original_trainer_name(
    struct pkmn_pokemon* p_pokemon,
    const char* p_original_trainer_name
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_original_trainer_public_id(
    struct pkmn_pokemon* p_pokemon,
    uint16_t original_trainer_public_id
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_original_trainer_secret_id(
    struct pkmn_pokemon* p_pokemon,
    uint16_t original_trainer_secret_id
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_original_trainer_id(
    struct pkmn_pokemon* p_pokemon,
    uint32_t original_trainer_id
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_original_trainer_gender(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_gender original_trainer_gender
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_current_trainer_friendship(
    struct pkmn_pokemon* p_pokemon,
    int* p_current_trainer_friendship_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_current_trainer_friendship(
    struct pkmn_pokemon* p_pokemon,
    int current_trainer_friendship
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_ability(
    struct pkmn_pokemon* p_pokemon,
    char* p_ability_out,
    size_t ability_buffer_len,
    size_t* p_actual_ability_len_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_ability(
    struct pkmn_pokemon* p_pokemon,
    const char* p_ability
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_ball(
    struct pkmn_pokemon* p_pokemon,
    char* p_ball_out,
    size_t ball_buffer_len,
    size_t* p_actual_ball_len_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_ball(
    struct pkmn_pokemon* p_pokemon,
    const char* p_ball
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_level_met(
    struct pkmn_pokemon* p_pokemon,
    int* p_level_met_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_level_met(
    struct pkmn_pokemon* p_pokemon,
    int level_met
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_location_met(
    struct pkmn_pokemon* p_pokemon,
    bool as_egg,
    char* p_location_met_out,
    size_t location_met_buffer_len,
    size_t* p_actual_location_met_len_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_location_met(
    struct pkmn_pokemon* p_pokemon,
    const char* p_location_met,
    bool as_egg
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_original_game(
    struct pkmn_pokemon* p_pokemon,
    char* p_original_game_out,
    size_t original_game_buffer_len,
    size_t* p_actual_original_game_len_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_original_game(
    struct pkmn_pokemon* p_pokemon,
    const char* p_game
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_personality(
    struct pkmn_pokemon* p_pokemon,
    uint32_t* p_personality_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_personality(
    struct pkmn_pokemon* p_pokemon,
    uint32_t personality
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_experience(
    struct pkmn_pokemon* p_pokemon,
    int* p_experience_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_experience(
    struct pkmn_pokemon* p_pokemon,
    int experience
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_level(
    struct pkmn_pokemon* p_pokemon,
    int* p_level_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_level(
    struct pkmn_pokemon* p_pokemon,
    int level
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_markings(
    struct pkmn_pokemon* p_pokemon,
    bool* p_has_markings_buffer_out,
    size_t has_marking_buffer_size,
    size_t* p_actual_num_markings_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_has_marking(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_marking marking,
    bool has_marking
);

PKMN_C_API enum pkmn_error pkmn_pokemon_has_ribbon(
    struct pkmn_pokemon* p_pokemon,
    const char* p_ribbon_name,
    bool* p_has_ribbon_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_has_ribbon(
    struct pkmn_pokemon* p_pokemon,
    const char* p_ribbon_name,
    bool has_ribbon
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_ribbon_names(
    struct pkmn_pokemon* p_pokemon,
    struct pkmn_string_list* p_ribbon_names_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_contest_stats(
    struct pkmn_pokemon* p_pokemon,
    int* p_contest_stats_buffer_out,
    size_t contest_stat_buffer_size,
    size_t* p_actual_num_contest_stats_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_contest_stat(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_contest_stat contest_stat,
    int value
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_moves(
    struct pkmn_pokemon* p_pokemon,
    struct pkmn_move_slots* p_move_slots_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_move(
    struct pkmn_pokemon* p_pokemon,
    size_t index,
    const char* p_move
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_move_pp(
    struct pkmn_pokemon* p_pokemon,
    size_t index,
    int pp
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_EVs(
    struct pkmn_pokemon* p_pokemon,
    int* p_EVs_buffer_out,
    size_t stat_buffer_size,
    size_t* p_actual_num_EVs_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_EV(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_stat stat,
    int value
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_IVs(
    struct pkmn_pokemon* p_pokemon,
    int* p_IVs_buffer_out,
    size_t stat_buffer_size,
    size_t* p_actual_num_IVs_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_IV(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_stat stat,
    int value
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_stats(
    struct pkmn_pokemon* p_pokemon,
    int* p_stats_buffer_out,
    size_t stat_buffer_size,
    size_t* p_actual_num_stats_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_current_hp(
    struct pkmn_pokemon* p_pokemon,
    int* p_current_hp_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_current_hp(
    struct pkmn_pokemon* p_pokemon,
    int current_hp
);

// Filepaths

PKMN_C_API enum pkmn_error pkmn_pokemon_get_icon_filepath(
    struct pkmn_pokemon* p_pokemon,
    char* p_icon_filepath_out,
    size_t icon_filepath_buffer_len,
    size_t* p_actual_icon_filepath_len_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_sprite_filepath(
    struct pkmn_pokemon* p_pokemon,
    char* p_sprite_filepath_out,
    size_t sprite_filepath_buffer_len,
    size_t* p_actual_sprite_filepath_len_out
);

// Attributes

PKMN_C_API enum pkmn_error pkmn_pokemon_get_numeric_attribute(
    struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    int* p_value_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_numeric_attribute(
    struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    int value
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_string_attribute(
    struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    char* p_value_out,
    size_t value_buffer_len,
    size_t* p_actual_value_len_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_string_attribute(
    struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    const char* p_value
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_boolean_attribute(
    struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    bool* p_value_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_set_boolean_attribute(
    struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    bool value
);

PKMN_C_API enum pkmn_error pkmn_pokemon_get_attribute_names(
    struct pkmn_pokemon* p_pokemon,
    struct pkmn_attribute_names* p_attribute_names_out
);

#ifdef __cplusplus
}
#endif

#endif /* p_PKMN_C_POKEMON_H */
