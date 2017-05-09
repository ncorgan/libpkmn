/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_GAME_SAVE_H
#define PKMN_C_GAME_SAVE_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/item_bag.h>
#include <pkmn-c/item_list.h>
#include <pkmn-c/pokemon_party.h>
#include <pkmn-c/pokemon_pc.h>

#include <pkmn-c/types/gender.h>

#include <stdint.h>

#if !defined(PKMN_GAME_SAVE_DECLARED) && !defined(__DOXYGEN__)
struct pkmn_game_save_t;
typedef struct pkmn_game_save_t pkmn_game_save_t;
#define PKMN_GAME_SAVE_DECLARED
#endif

typedef pkmn_game_save_t* pkmn_game_save_handle_t;

typedef enum {
    PKMN_GAME_SAVE_TYPE_NONE = 0,
    PKMN_GAME_SAVE_TYPE_RED_BLUE_YELLOW,
    PKMN_GAME_SAVE_TYPE_GOLD_SILVER,
    PKMN_GAME_SAVE_TYPE_CRYSTAL,
    PKMN_GAME_SAVE_TYPE_RUBY_SAPPHIRE,
    PKMN_GAME_SAVE_TYPE_EMERALD,
    PKMN_GAME_SAVE_TYPE_FIRERED_LEAFGREEN,
    PKMN_GAME_SAVE_TYPE_DIAMOND_PEARL,
    PKMN_GAME_SAVE_TYPE_PLATINUM,
    PKMN_GAME_SAVE_TYPE_HEARTGOLD_SOULSILVER
} pkmn_game_save_type_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_game_save_detect_type(
    const char* filepath,
    pkmn_game_save_type_t* game_save_type_out
);

PKMN_API pkmn_error_t pkmn_game_save_from_file(
    pkmn_game_save_handle_t* handle_ptr,
    const char* filepath
); 

PKMN_API pkmn_error_t pkmn_game_save_free(
    pkmn_game_save_handle_t* handle_ptr
);

PKMN_API const char* pkmn_game_save_strerror(
    pkmn_game_save_handle_t handle
);

PKMN_API pkmn_error_t pkmn_game_save_get_filepath(
    pkmn_game_save_handle_t handle,
    char* filepath_out,
    size_t buffer_len
);

PKMN_API pkmn_error_t pkmn_game_save_save(
    pkmn_game_save_handle_t handle
);

PKMN_API pkmn_error_t pkmn_game_save_save_as(
    pkmn_game_save_handle_t handle,
    const char* filepath
);

PKMN_API pkmn_error_t pkmn_game_save_get_game(
    pkmn_game_save_handle_t handle,
    char* game_out,
    size_t buffer_len
);

PKMN_API pkmn_error_t pkmn_game_save_get_trainer_name(
    pkmn_game_save_handle_t handle,
    char* trainer_name_out,
    size_t buffer_len
);

PKMN_API pkmn_error_t pkmn_game_save_set_trainer_name(
    pkmn_game_save_handle_t handle,
    const char* trainer_name
);

PKMN_API pkmn_error_t pkmn_game_save_get_trainer_id(
    pkmn_game_save_handle_t handle,
    uint32_t* trainer_id_out
);

PKMN_API pkmn_error_t pkmn_game_save_set_trainer_id(
    pkmn_game_save_handle_t handle,
    uint32_t trainer_id
);

PKMN_API pkmn_error_t pkmn_game_save_get_trainer_public_id(
    pkmn_game_save_handle_t handle,
    uint16_t* trainer_public_id_out
);

PKMN_API pkmn_error_t pkmn_game_save_set_trainer_public_id(
    pkmn_game_save_handle_t handle,
    uint16_t trainer_public_id
);

PKMN_API pkmn_error_t pkmn_game_save_get_trainer_secret_id(
    pkmn_game_save_handle_t handle,
    uint16_t* trainer_secret_id_out
);

PKMN_API pkmn_error_t pkmn_game_save_set_trainer_secret_id(
    pkmn_game_save_handle_t handle,
    uint16_t trainer_secret_id
);

PKMN_API pkmn_error_t pkmn_game_save_get_trainer_gender(
    pkmn_game_save_handle_t handle,
    pkmn_gender_t* gender_out
);

PKMN_API pkmn_error_t pkmn_game_save_set_trainer_gender(
    pkmn_game_save_handle_t handle,
    pkmn_gender_t gender
);

PKMN_API pkmn_error_t pkmn_game_save_get_rival_name(
    pkmn_game_save_handle_t handle,
    char* rival_name_out,
    size_t buffer_len
);

PKMN_API pkmn_error_t pkmn_game_save_set_rival_name(
    pkmn_game_save_handle_t handle,
    const char* rival_name
);

PKMN_API pkmn_error_t pkmn_game_save_get_money(
    pkmn_game_save_handle_t handle,
    int* money_out
);

PKMN_API pkmn_error_t pkmn_game_save_set_money(
    pkmn_game_save_handle_t handle,
    int money
);

PKMN_API pkmn_error_t pkmn_game_save_get_pokemon_party(
    pkmn_game_save_handle_t handle,
    pkmn_pokemon_party_handle_t* pokemon_party_handle_out
);

PKMN_API pkmn_error_t pkmn_game_save_get_pokemon_pc(
    pkmn_game_save_handle_t handle,
    pkmn_pokemon_pc_handle_t* pokemon_pc_handle_out
);

PKMN_API pkmn_error_t pkmn_game_save_get_item_bag(
    pkmn_game_save_handle_t handle,
    pkmn_item_bag_handle_t* item_bag_handle_out
);

PKMN_API pkmn_error_t pkmn_game_save_get_item_pc(
    pkmn_game_save_handle_t handle,
    pkmn_item_list_handle_t* item_pc_handle_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_GAME_SAVE_H */
