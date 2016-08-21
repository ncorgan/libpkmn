/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_ITEMS_H
#define PKMN_C_ITEMS_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO: move target when database branch done
// NOTE: functions to be overhauled not wrapped

PKMN_API pkmn_error_t pkmn_database_move_type(
    const char* item_name,
    const char* game_name,
    char* move_type_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_database_move_description(
    const char* item_name,
    const char* game_name,
    char* move_description_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_database_move_damage_class(
    const char* item_name,
    const char* game_name,
    char* move_damage_class_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_database_move_base_power(
    const char* item_name,
    const char* game_name,
    int* base_power_out
);

PKMN_API pkmn_error_t pkmn_database_move_pp(
    const char* item_name,
    const char* game_name,
    int num_pp_ups,
    int* pp_out
);

PKMN_API pkmn_error_t pkmn_database_move_base_power(
    const char* item_name,
    const char* game_name,
    int* base_power_out
);

PKMN_API pkmn_error_t pkmn_database_move_accuracy(
    const char* item_name,
    const char* game_name,
    float* accuracy_out
);

PKMN_API pkmn_error_t pkmn_database_move_priority(
    const char* item_name,
    const char* game_name,
    int* priority_out
);

PKMN_API pkmn_error_t pkmn_database_move_effect(
    const char* item_name,
    const char* game_name,
    char* move_effect_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ITEMS_H */
