/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUMS_ENUM_TO_STRING_H
#define PKMN_C_ENUMS_ENUM_TO_STRING_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/enums/ability.h>
#include <pkmn-c/enums/ball.h>
#include <pkmn-c/enums/condition.h>
#include <pkmn-c/enums/egg_group.h>
#include <pkmn-c/enums/game.h>
#include <pkmn-c/enums/gender.h>
#include <pkmn-c/enums/item.h>
#include <pkmn-c/enums/language.h>
#include <pkmn-c/enums/move_damage_class.h>
#include <pkmn-c/enums/move_target.h>
#include <pkmn-c/enums/move.h>
#include <pkmn-c/enums/nature.h>
#include <pkmn-c/enums/species.h>
#include <pkmn-c/enums/stat.h>
#include <pkmn-c/enums/type.h>

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_ability_to_string(
    enum pkmn_ability ability,
    char* p_ability_name_out,
    size_t ability_name_buffer_len,
    size_t* p_ability_name_len_out
);

PKMN_C_API enum pkmn_error pkmn_string_to_ability(
    const char* p_ability_name,
    enum pkmn_ability* ability_out
);

PKMN_C_API enum pkmn_error pkmn_ball_to_string(
    enum pkmn_ball ball,
    char* p_ball_name_out,
    size_t ball_name_buffer_len,
    size_t* p_ball_name_len_out
);

PKMN_C_API enum pkmn_error pkmn_string_to_ball(
    const char* p_ball_name,
    enum pkmn_ball* ball_out
);

PKMN_C_API enum pkmn_error pkmn_condition_to_string(
    enum pkmn_condition condition,
    char* p_condition_name_out,
    size_t condition_name_buffer_len,
    size_t* p_condition_name_len_out
);

PKMN_C_API enum pkmn_error pkmn_string_to_condition(
    const char* p_condition_name,
    enum pkmn_condition* condition_out
);

PKMN_C_API enum pkmn_error pkmn_egg_group_to_string(
    enum pkmn_egg_group egg_group,
    char* p_egg_group_name_out,
    size_t egg_group_name_buffer_len,
    size_t* p_egg_group_name_len_out
);

PKMN_C_API enum pkmn_error pkmn_string_to_egg_group(
    const char* p_egg_group_name,
    enum pkmn_egg_group* egg_group_out
);

PKMN_C_API enum pkmn_error pkmn_game_to_string(
    enum pkmn_game game,
    char* p_game_name_out,
    size_t game_name_buffer_len,
    size_t* p_game_name_len_out
);

PKMN_C_API enum pkmn_error pkmn_string_to_game(
    const char* p_game_name,
    enum pkmn_game* game_out
);

PKMN_C_API enum pkmn_error pkmn_gender_to_string(
    enum pkmn_gender gender,
    char* p_gender_name_out,
    size_t gender_name_buffer_len,
    size_t* p_gender_name_len_out
);

PKMN_C_API enum pkmn_error pkmn_string_to_gender(
    const char* p_gender_name,
    enum pkmn_gender* gender_out
);

PKMN_C_API enum pkmn_error pkmn_item_to_string(
    enum pkmn_item item,
    char* p_item_name_out,
    size_t item_name_buffer_len,
    size_t* p_item_name_len_out
);

PKMN_C_API enum pkmn_error pkmn_string_to_item(
    const char* p_item_name,
    enum pkmn_item* item_out
);

PKMN_C_API enum pkmn_error pkmn_language_to_string(
    enum pkmn_language language,
    char* p_language_name_out,
    size_t language_name_buffer_len,
    size_t* p_language_name_len_out
);

PKMN_C_API enum pkmn_error pkmn_string_to_language(
    const char* p_language_name,
    enum pkmn_language* language_out
);

PKMN_C_API enum pkmn_error pkmn_move_damage_class_to_string(
    enum pkmn_move_damage_class move_damage_class,
    char* p_move_damage_class_name_out,
    size_t move_damage_class_name_buffer_len,
    size_t* p_move_damage_class_name_len_out
);

PKMN_C_API enum pkmn_error pkmn_string_to_move_damage_class(
    const char* p_move_damage_class_name,
    enum pkmn_move_damage_class* move_damage_class_out
);

PKMN_C_API enum pkmn_error pkmn_move_target_to_string(
    enum pkmn_move_target move_target,
    char* p_move_target_name_out,
    size_t move_target_name_buffer_len,
    size_t* p_move_target_name_len_out
);

PKMN_C_API enum pkmn_error pkmn_string_to_move_target(
    const char* p_move_target_name,
    enum pkmn_move_target* move_target_out
);

PKMN_C_API enum pkmn_error pkmn_move_to_string(
    enum pkmn_move move,
    char* p_move_name_out,
    size_t move_name_buffer_len,
    size_t* p_move_name_len_out
);

PKMN_C_API enum pkmn_error pkmn_string_to_move(
    const char* p_move_name,
    enum pkmn_move* move_out
);

PKMN_C_API enum pkmn_error pkmn_nature_to_string(
    enum pkmn_nature nature,
    char* p_nature_name_out,
    size_t nature_name_buffer_len,
    size_t* p_nature_name_len_out
);

PKMN_C_API enum pkmn_error pkmn_string_to_nature(
    const char* p_nature_name,
    enum pkmn_nature* nature_out
);

PKMN_C_API enum pkmn_error pkmn_species_to_string(
    enum pkmn_species species,
    char* p_species_name_out,
    size_t species_name_buffer_len,
    size_t* p_species_name_len_out
);

PKMN_C_API enum pkmn_error pkmn_string_to_species(
    const char* p_species_name,
    enum pkmn_species* species_out
);

PKMN_C_API enum pkmn_error pkmn_stat_to_string(
    enum pkmn_stat stat,
    char* p_stat_name_out,
    size_t stat_name_buffer_len,
    size_t* p_stat_name_len_out
);

PKMN_C_API enum pkmn_error pkmn_string_to_stat(
    const char* p_stat_name,
    enum pkmn_stat* stat_out
);

PKMN_C_API enum pkmn_error pkmn_type_to_string(
    enum pkmn_type type,
    char* p_type_name_out,
    size_t type_name_buffer_len,
    size_t* p_type_name_len_out
);

PKMN_C_API enum pkmn_error pkmn_string_to_type(
    const char* p_type_name,
    enum pkmn_type* type_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ENUMS_ENUM_TO_STRING_H */
