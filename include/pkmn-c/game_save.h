/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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
#include <pkmn-c/pokedex.h>
#include <pkmn-c/pokemon_party.h>
#include <pkmn-c/pokemon_pc.h>

#include <pkmn-c/types/attribute_names.h>
#include <pkmn-c/types/gender.h>
#include <pkmn-c/types/trainer_info.h>

#include <stdint.h>

enum pkmn_game_save_type
{
    //! Not a valid save.
    PKMN_GAME_SAVE_TYPE_NONE = 0,
    //! Red/Blue/Yellow
    PKMN_GAME_SAVE_TYPE_RED_BLUE_YELLOW,
    //! Gold/Silver
    PKMN_GAME_SAVE_TYPE_GOLD_SILVER,
    //! Crystal
    PKMN_GAME_SAVE_TYPE_CRYSTAL,
    //! Ruby/Sapphire
    PKMN_GAME_SAVE_TYPE_RUBY_SAPPHIRE,
    //! Emerald
    PKMN_GAME_SAVE_TYPE_EMERALD,
    //! FireRed/LeafGreen
    PKMN_GAME_SAVE_TYPE_FIRERED_LEAFGREEN,
    //! Colosseum/XD
    PKMN_GAME_SAVE_TYPE_COLOSSEUM_XD
};

struct pkmn_game_save
{
    char* game;

    void* _internal;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_game_save_detect_type(
    const char* filepath,
    enum pkmn_game_save_type* game_save_type_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_init_from_file(
    const char* filepath,
    struct pkmn_game_save* game_save_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_free(
    struct pkmn_game_save* game_save_ptr
);

PKMN_C_API const char* pkmn_game_save_strerror(
    struct pkmn_game_save* game_save_ptr
);

// Save file actions

PKMN_C_API enum pkmn_error pkmn_game_save_get_filepath(
    struct pkmn_game_save* game_save_ptr,
    char* filepath_buffer,
    size_t filepath_buffer_length,
    size_t* actual_filepath_length_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_save(
    struct pkmn_game_save* game_save_ptr
);

PKMN_C_API enum pkmn_error pkmn_game_save_save_as(
    struct pkmn_game_save* game_save_ptr,
    const char* filepath
);

// Trainer info

PKMN_C_API enum pkmn_error pkmn_game_save_get_trainer_info(
    struct pkmn_game_save* game_save_ptr,
    struct pkmn_trainer_info* trainer_info_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_trainer_name(
    struct pkmn_game_save* game_save_ptr,
    const char* trainer_name
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_trainer_id(
    struct pkmn_game_save* game_save_ptr,
    uint32_t trainer_id
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_trainer_public_id(
    struct pkmn_game_save* game_save_ptr,
    uint16_t trainer_public_id
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_trainer_secret_id(
    struct pkmn_game_save* game_save_ptr,
    uint16_t trainer_secret_id
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_trainer_gender(
    struct pkmn_game_save* game_save_ptr,
    enum pkmn_gender gender
);

// Other fields

PKMN_C_API enum pkmn_error pkmn_game_save_get_rival_name(
    struct pkmn_game_save* game_save_ptr,
    char* rival_name_buffer,
    size_t rival_name_buffer_length,
    size_t* actual_rival_name_length_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_rival_name(
    struct pkmn_game_save* game_save_ptr,
    const char* rival_name
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_money(
    struct pkmn_game_save* game_save_ptr,
    int* money_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_money(
    struct pkmn_game_save* game_save_ptr,
    int money
);

// Pokémon and items

PKMN_C_API enum pkmn_error pkmn_game_save_get_pokedex(
    struct pkmn_game_save* game_save_ptr,
    struct pkmn_pokedex* pokedex_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_pokemon_party(
    struct pkmn_game_save* game_save_ptr,
    struct pkmn_pokemon_party* pokemon_party_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_pokemon_pc(
    struct pkmn_game_save* game_save_ptr,
    struct pkmn_pokemon_pc* pokemon_pc_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_item_bag(
    struct pkmn_game_save* game_save_ptr,
    struct pkmn_item_bag* item_bag_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_item_pc(
    struct pkmn_game_save* game_save_ptr,
    struct pkmn_item_list* item_pc_out
);

// Attributes

PKMN_C_API enum pkmn_error pkmn_game_save_get_numeric_attribute(
    struct pkmn_game_save* game_save_ptr,
    const char* attribute_name,
    int* value_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_numeric_attribute(
    struct pkmn_game_save* game_save_ptr,
    const char* attribute_name,
    int value
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_string_attribute(
    struct pkmn_game_save* game_save_ptr,
    const char* attribute_name,
    char* value_out,
    size_t value_buffer_len,
    size_t* actual_value_len_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_string_attribute(
    struct pkmn_game_save* game_save_ptr,
    const char* attribute_name,
    const char* value
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_boolean_attribute(
    struct pkmn_game_save* game_save_ptr,
    const char* attribute_name,
    bool* value_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_boolean_attribute(
    struct pkmn_game_save* game_save_ptr,
    const char* attribute_name,
    bool value
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_attribute_names(
    struct pkmn_game_save* game_save_ptr,
    struct pkmn_attribute_names* attribute_names_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_GAME_SAVE_H */
