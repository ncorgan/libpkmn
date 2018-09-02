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

#include <pkmn-c/enums/ability.h>
#include <pkmn-c/enums/game.h>
#include <pkmn-c/enums/item.h>
#include <pkmn-c/enums/move.h>
#include <pkmn-c/enums/nature.h>
#include <pkmn-c/enums/species.h>
#include <pkmn-c/enums/type.h>

#include <pkmn-c/types/string_types.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_database_ability_list(
    int generation,
    struct pkmn_ability_enum_list* p_ability_enum_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_ability_name_list(
    int generation,
    struct pkmn_string_list* p_ability_name_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_game_list(
    int generation,
    bool include_previous,
    struct pkmn_game_enum_list* p_game_enum_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_game_name_list(
    int generation,
    bool include_previous,
    struct pkmn_string_list* p_game_name_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_gamecube_shadow_pokemon_list(
    bool colosseum,
    struct pkmn_species_enum_list* p_gamecube_shadow_pokemon_enum_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_gamecube_shadow_pokemon_name_list(
    bool colosseum,
    struct pkmn_string_list* p_gamecube_shadow_pokemon_name_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_hm_move_list(
    enum pkmn_game game,
    struct pkmn_move_enum_list* p_hm_move_enum_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_hm_move_name_list(
    enum pkmn_game game,
    struct pkmn_string_list* p_hm_move_name_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_item_list(
    enum pkmn_game game,
    struct pkmn_item_enum_list* p_item_enum_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_item_name_list(
    enum pkmn_game game,
    struct pkmn_string_list* p_item_list_name_out
);

PKMN_C_API enum pkmn_error pkmn_database_location_name_list(
    enum pkmn_game game,
    bool whole_generation,
    struct pkmn_string_list* p_location_name_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_move_list(
    enum pkmn_game game,
    struct pkmn_move_enum_list* p_move_enum_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_move_name_list(
    enum pkmn_game game,
    struct pkmn_string_list* p_move_name_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_nature_list(
    struct pkmn_nature_enum_list* p_nature_enum_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_nature_name_list(
    struct pkmn_string_list* p_nature_name_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_pokemon_list(
    int generation,
    bool include_previous,
    struct pkmn_species_enum_list* p_pokemon_enum_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_pokemon_name_list(
    int generation,
    bool include_previous,
    struct pkmn_string_list* p_pokemon_name_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_region_name_list(
    struct pkmn_string_list* p_region_name_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_ribbon_name_list(
    int generation,
    struct pkmn_string_list* p_ribbon_name_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_super_training_medal_name_list(
    struct pkmn_string_list* p_super_training_medal_name_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_tm_move_list(
    enum pkmn_game game,
    struct pkmn_move_enum_list* p_tm_move_enum_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_tm_move_name_list(
    enum pkmn_game game,
    struct pkmn_string_list* p_tm_move_name_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_type_list(
    enum pkmn_game game,
    struct pkmn_type_enum_list* p_type_enum_list_out
);

PKMN_C_API enum pkmn_error pkmn_database_type_name_list(
    enum pkmn_game game,
    struct pkmn_string_list* p_type_name_list_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ITEMS_H */
