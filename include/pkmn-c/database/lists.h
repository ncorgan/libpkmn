/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_database_ability_list(
    int generation,
    struct pkmn_string_list* p_ability_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_game_list(
    int generation,
    bool include_previous,
    struct pkmn_string_list* p_p_game_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_gamecube_shadow_pokemon_list(
    bool colosseum,
    struct pkmn_string_list* p_p_gamecube_shadow_pokemon_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_item_list(
    const char* p_game,
    struct pkmn_string_list* p_item_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_location_list(
    const char* p_game,
    bool whole_generation,
    struct pkmn_string_list* p_location_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_move_list(
    const char* p_game,
    struct pkmn_string_list* p_move_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_nature_list(
    struct pkmn_string_list* p_nature_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_pokemon_list(
    int generation,
    bool include_previous,
    struct pkmn_string_list* p_pokemon_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_region_list(
    struct pkmn_string_list* p_region_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_ribbon_list(
    int generation,
    struct pkmn_string_list* p_ribbon_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_super_training_medal_list(
    struct pkmn_string_list* p_super_training_medal_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_type_list(
    const char* p_game,
    struct pkmn_string_list* p_type_list_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ITEMS_H */
