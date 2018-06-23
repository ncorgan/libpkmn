/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEMON_PC_H
#define PKMN_C_POKEMON_PC_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/pokemon_box.h>

#include <pkmn-c/types/game.h>
#include <pkmn-c/types/pokemon_box_list.h>
#include <pkmn-c/types/string_types.h>

/*!
 * @brief A list of the Pokémon boxes in which a trainer stores Pokémon
 *
 * This struct provides an abstraction of the different in-game representations of
 * a Pokémon PC throughout the different games, and the corresponding functions
 * provide a common API for interacting with the PC.
 */
struct pkmn_pokemon_pc
{
    /*!
     * @brief The game this PC is associated with.
     */
    enum pkmn_game game;

    /*!
     * @brief How many boxes are in this PC.
     */
    size_t capacity;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Initializes a pkmn_pokemon_pc instance, based on the given game.
 *
 * If this call fails, the pokemon_pc_out struct will not be altered.
 *
 * \param game The game for which to instantiate the pc
 * \param pokemon_pc_out A pointer to the pc struct to populate
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any pointer parameter is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the game is invalid
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_pc_init(
    enum pkmn_game game,
    struct pkmn_pokemon_pc* p_pokemon_pc_out
);

/*!
 * @brief Frees a pkmn_pokemon_pc instance.
 *
 * All dynamically allocated memory will be freed, and all pointers will be
 * set to NULL.
 *
 * If this call fails, the pokemon_pc_ptr struct will be in an undefined state.
 *
 * \param pokemon_pc_ptr A pointer to the pc struct to free
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if pokemon_pc_ptr is NULL
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_pc_free(
    struct pkmn_pokemon_pc* p_pokemon_pc
);

/*!
 * @brief Returns the error message resulting from the last function that used
 *        the given pkmn_pokemon_pc.
 *
 * \returns <b>"None"</b> if the last function call using this pkmn_pokemon_pc instance
 *                        was successful
 * \returns <b>NULL</b> if pokemon_pc_ptr is NULL
 */
PKMN_C_API const char* pkmn_pokemon_pc_strerror(
    const struct pkmn_pokemon_pc* p_pokemon_pc
);

/*!
 * @brief Returns the box at the given index.
 *
 * If the struct pointed to by pokemon_box_out already contains a Pokémon box,
 * its memory will be leaked, as this function does not free the memory.
 *
 * If any error occurs, the struct pointed to by pokemon_box_out will not be
 * affected.
 *
 * \param pokemon_pc_ptr A pointer to the PC struct to use
 * \param index The position in the PC whose Pokémon box to return
 * \param pokemon_box_out Where to return the Pokémon box
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if either pointer parameter is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE If index is not in the range [0-capacity]
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_pc_get_box(
    const struct pkmn_pokemon_pc* p_pokemon_pc,
    size_t index,
    struct pkmn_pokemon_box* p_pokemon_box_out
);

/*!
 * @brief Returns a list of all box names.
 *
 * If the list pointed to by box_names_out is already in use, its memory will
 * be leaked, as this function does not free the memory.
 *
 * Generation I boxes have no name, so for a Generation I PC,
 * this call will fail.
 *
 * \param pokemon_pc_ptr A pointer to the PC struct to use
 * \param box_names_out Where to return the box names
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if either pointer parameter is NULL
 * \returns ::PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR if this is a Generation I PC
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_pc_get_box_names(
    const struct pkmn_pokemon_pc* p_pokemon_pc,
    struct pkmn_string_list* p_box_names_out
);

/*!
 * @brief Returns all Pokémon boxes in the PC represented as a list.
 *
 * If the list pointed to by pokemon_box_list_out is already in use, its memory
 * will be leaked, as this function does not free the memory.
 *
 * \param pokemon_pc_ptr A pointer to the PC struct to use
 * \param pokemon_box_list_out Where to return the Pokémon box list
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if either pointer parameter is NULL
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_pc_as_list(
    const struct pkmn_pokemon_pc* p_pokemon_pc,
    struct pkmn_pokemon_box_list* p_pokemon_box_list_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_PC_H */
