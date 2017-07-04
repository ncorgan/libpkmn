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
#include <pkmn-c/types/trainer_info.h>

#include <stdint.h>

#if !defined(PKMN_GAME_SAVE_DECLARED) && !defined(__DOXYGEN__)
struct pkmn_game_save_t;
typedef struct pkmn_game_save_t pkmn_game_save_t;
#define PKMN_GAME_SAVE_DECLARED
#endif

/*!
 * @brief The game save handle used throughout LibPKMN.
 *
 * Passing a non-NULL handle into any function before calling pkmn_game_save_from_file
 * will result in a crash. Passing a NULL handle into any function will
 * cause it to return ::PKMN_ERROR_NULL_POINTER.
 */
typedef pkmn_game_save_t* pkmn_game_save_handle_t;

typedef enum {
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
} pkmn_game_save_type_t;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Returns what type of game save is at the given filepath.
 *
 * Returns a pkmn_game_save_type_t.
 *
 * \param filepath The filepath to check
 * \param game_save_type_out Where to return the result
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if either parameter is NULL
 */
PKMN_API pkmn_error_t pkmn_game_save_detect_type(
    const char* filepath,
    pkmn_game_save_type_t* game_save_type_out
);

/*!
 * @brief Imports the save from the game save at the given file.
 *
 * \param handle_ptr A pointer to the handle to initialize
 * \param filepath The filepath from which to import the game save
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the given filepath doesn't exist
 * \returns ::PKMN_ERROR_NULL_POINTER if either parameter is NULL
 */
PKMN_API pkmn_error_t pkmn_game_save_from_file(
    pkmn_game_save_handle_t* handle_ptr,
    const char* filepath
); 

/*!
 * @brief Frees the given game save handle.
 *
 * The game save handle at the given handle will be set to NULL upon completion.
 *
 * Passing in a pointer to a handle before passing it into pkmn_game_save_from_file
 * will result in a crash.
 *
 * \param handle_ptr A pointer to the handle to free
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if handle_ptr is NULL
 */
PKMN_API pkmn_error_t pkmn_game_save_free(
    pkmn_game_save_handle_t* handle_ptr
);

/*!
 * @brief Returns a string elaboration of the last error returned by this handle.
 *
 * \param handle The handle to the game save to check
 * \returns The last error message from the handle
 * \returns NULL If the handle is NULL
 */
PKMN_API const char* pkmn_game_save_strerror(
    pkmn_game_save_handle_t handle
);

/*!
 * @brief Returns the path from which the given save was imported.
 *
 * \param handle The handle to the game save to check
 * \param filepath_out The buffer in which to return the filepath
 * \param buffer_len The size of the buffer passed into filepath_out
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if handle or filepath_out is NULL
 * \returns ::PKMN_ERROR_BUFFER_TOO_SMALL if filepath_out is too small for the return string
 */
PKMN_API pkmn_error_t pkmn_game_save_get_filepath(
    pkmn_game_save_handle_t handle,
    char* filepath_out,
    size_t buffer_len
);

/*!
 * @brief Saves the game save at its current filepath.
 *
 * If the save operation fails, the save file will be in an unknown
 * state.
 *
 * \param handle The handle to the game save to save
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if handle is NULL
 * \returns ::PKMN_ERROR_RUNTIME_ERROR if the save operation fails
 */
PKMN_API pkmn_error_t pkmn_game_save_save(
    pkmn_game_save_handle_t handle
);

/*!
 * @brief Saves the game save at the given filepath.
 *
 * No changes will be made to the file at the original path, and the
 * new filepath will be considered the current one.
 *
 * If the save operation fails, the save file will be in an unknown
 * state.
 *
 * \param handle The handle to the game save to save
 * \param filepath The new filepath for the game save
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if handle or filepath is null
 * \returns ::PKMN_ERROR_RUNTIME_ERROR if the save operation fails
 */
PKMN_API pkmn_error_t pkmn_game_save_save_as(
    pkmn_game_save_handle_t handle,
    const char* filepath
);

/*!
 * @brief Returns which game this save corresponds to.
 *
 * This is guaranteed to be valid within a given version group, but as
 * there is often no way to distinguish between saves within the same
 * version group, LibPKMN will attempt to use the filename to determine
 * the specific game.
 *
 * \param handle The handle to the game save to check
 * \param game_out The buffer in which to return the game
 * \param buffer_len The size of the buffer passed into game_out
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if handle or game_out is NULL
 * \returns ::PKMN_ERROR_BUFFER_TOO_SMALL if game_out is too small for the return string
 */
PKMN_API pkmn_error_t pkmn_game_save_get_game(
    pkmn_game_save_handle_t handle,
    char* game_out,
    size_t buffer_len
);

/*!
 * @brief Returns the trainer's name, ID, and gender.
 *
 * \param handle The handle to the game save to check
 * \param trainer_info_out Where to return the trainer info
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if handle or trainer_info_out is NULL
 */
PKMN_API pkmn_error_t pkmn_game_save_get_trainer_info(
    pkmn_game_save_handle_t handle,
    pkmn_trainer_info_t* trainer_info_out
);

/*!
 * @brief Sets the trainer's name, ID, and gender.
 *
 * Trainer IDs are how the game determines if a given Pokémon was originally caught by
 * a given trainer. In Generation I-II games, trainer IDs are 16-bit. In Generation III
 * and beyond, another 16-bit "secret ID" was added, making the full ID 32 bits. In
 * Generation I-II saves, if the secret ID field is set, it will result in an error.
 *
 * \param handle The handle to the game save to check
 * \param trainer_info The trainer info to set
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if handle or trainer_info is NULL
 * \returns ::PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR if the secret ID is not 0 for a Generation I-II save
 */
PKMN_API pkmn_error_t pkmn_game_save_set_trainer_info(
    pkmn_game_save_handle_t handle,
    pkmn_trainer_info_t* trainer_info
);

/*!
 * @brief Returns the name of the rival character.
 *
 * Gamecube games have no rival character, so calling this function with those saves will result
 * in an error.
 *
 * \param handle The handle to the game save to use
 * \param rival_name_out The buffer in which to return the rival name
 * \param buffer_len The size of the buffer passed into rival_name_out
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if handle or rival_name_out is NULL
 * \returns ::PKMN_ERROR_BUFFER_TOO_SMALL if rival_name_out is too small for the return string
 * \returns ::PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR if the save is for a Gamecube game
 */
PKMN_API pkmn_error_t pkmn_game_save_get_rival_name(
    pkmn_game_save_handle_t handle,
    char* rival_name_out,
    size_t buffer_len
);

/*!
 * @brief Sets the name of the rival character.
 *
 * Valid player names are 1-7 characters.
 *
 * Gamecube games have no rival character, so calling this function with those saves will result
 * in an error. The following games have set rival names that cannot be changed:
 *  * Ruby/Sapphire/Emerald
 *  * Black/White
 *  * X/Y
 *  * Omega Ruby/Alpha Sapphire
 *  * Sun/Moon
 *
 * \param handle The handle to the game save to use
 * \param rival_name The rival name to set
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if handle or rival_name is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the given rival name is empty or longer than 7 characters
 * \returns ::PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR if the game has no rival or the rival cannot be renamed
 */
PKMN_API pkmn_error_t pkmn_game_save_set_rival_name(
    pkmn_game_save_handle_t handle,
    const char* rival_name
);

/*!
 * @brief Returns the amount of money the player is holding.
 *
 * The return value will be in the range [0-999999].
 *
 * \param handle The handle to the game save to use
 * \param money_out Where to return the amount of value
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if handle or money_out is NULL
 */
PKMN_API pkmn_error_t pkmn_game_save_get_money(
    pkmn_game_save_handle_t handle,
    int* money_out
);

/*!
 * @brief Sets the amount of money the player is holding.
 *
 * Valid values are in the range [0-999999].
 *
 * \param handle The handle to the game save to use
 * \param money Where to return the amount of value
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if handle is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if the input is not in the range [0-999999].
 */
PKMN_API pkmn_error_t pkmn_game_save_set_money(
    pkmn_game_save_handle_t handle,
    int money
);

/*!
 * @brief Returns a handle to the trainer's Pokémon party.
 *
 * \param handle The handle to the game save to use
 * \param pokemon_party_handle_out A pointer to where to return the pokemon_party handle
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if handle or pokemon_party_handle_out is NULL
 */
PKMN_API pkmn_error_t pkmn_game_save_get_pokemon_party(
    pkmn_game_save_handle_t handle,
    pkmn_pokemon_party_handle_t* pokemon_party_handle_out
);


/*!
 * @brief Returns a handle to the trainer's Pokémon PC.
 *
 * \param handle The handle to the game save to use
 * \param pokemon_pc_handle_out A pointer to where to return the pokemon_pc handle
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if handle or pokemon_pc_handle_out is NULL
 */
PKMN_API pkmn_error_t pkmn_game_save_get_pokemon_pc(
    pkmn_game_save_handle_t handle,
    pkmn_pokemon_pc_handle_t* pokemon_pc_handle_out
);

/*!
 * @brief Returns a handle to the trainer's item bag.
 *
 * \param handle The handle to the game save to use
 * \param item_bag_handle_out A pointer to where to return the item_bag handle
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if handle or item_bag_handle_out is NULL
 */
PKMN_API pkmn_error_t pkmn_game_save_get_item_bag(
    pkmn_game_save_handle_t handle,
    pkmn_item_bag_handle_t* item_bag_handle_out
);

/*!
 * @brief Returns a handle to the trainer's item pc.
 *
 * \param handle The handle to the game save to use
 * \param item_list_handle_out A pointer to where to return the item_list handle
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if handle or item_list_handle_out is NULL
 */
PKMN_API pkmn_error_t pkmn_game_save_get_item_pc(
    pkmn_game_save_handle_t handle,
    pkmn_item_list_handle_t* item_list_handle_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_GAME_SAVE_H */
