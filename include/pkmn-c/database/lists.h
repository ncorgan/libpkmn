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
#include <pkmn-c/types/string_types.h>

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_database_ability_list(
    int generation,
    pkmn_string_list_t* ability_list_out,
    size_t* list_length_out
);

PKMN_API pkmn_error_t pkmn_database_game_list(
    int generation,
    bool include_previous,
    pkmn_string_list_t* game_list_out,
    size_t* list_length_out
);

PKMN_API pkmn_error_t pkmn_database_item_list(
    const char* game,
    pkmn_string_list_t* item_list_out,
    size_t* list_length_out
);

PKMN_API pkmn_error_t pkmn_database_location_list(
    const char* game,
    bool whole_generation,
    pkmn_string_list_t* location_list_out,
    size_t* list_length_out
);

PKMN_API pkmn_error_t pkmn_database_move_list(
    const char* game,
    pkmn_string_list_t* move_list_out,
    size_t* list_length_out
);

PKMN_API pkmn_error_t pkmn_database_nature_list(
    pkmn_string_list_t* nature_list_out,
    size_t* list_length_out
);

PKMN_API pkmn_error_t pkmn_database_pokemon_list(
    int generation,
    bool include_previous,
    pkmn_string_list_t* pokemon_list_out,
    size_t* list_length_out
);

PKMN_API pkmn_error_t pkmn_database_region_list(
    pkmn_string_list_t* region_list_out,
    size_t* list_length_out
);

PKMN_API pkmn_error_t pkmn_database_ribbon_list(
    int generation,
    pkmn_string_list_t* ribbon_list_out,
    size_t* list_length_out
);

PKMN_API pkmn_error_t pkmn_database_super_training_medal_list(
    pkmn_string_list_t* super_training_medal_list_out,
    size_t* list_length_out
);

PKMN_API pkmn_error_t pkmn_database_type_list(
    const char* game,
    pkmn_string_list_t* type_list_out,
    size_t* list_length_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ITEMS_H */
