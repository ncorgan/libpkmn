/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEMON_BOX_H
#define PKMN_C_POKEMON_BOX_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/types/pokemon_list.h>

/*!
 * @brief A list of the Pokémon a trainer stores in a PC box.
 *
 * This struct provides an abstraction of the different in-game representations of
 * a Pokémon box throughout the different games, and the corresponding functions
 * provide a common API for interacting with the box.
 */
struct pkmn_pokemon_box
{
    /*!
     * @brief The game this box is associated with.
     */
    char* p_game;

    /*!
     * @brief How many Pokémon this party can hold.
     */
    size_t capacity;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Initializes a pkmn_pokemon_box instance, based on the given game.
 *
 * If this call fails, the pokemon_box_out struct will not be altered.
 *
 * \param game The game for which to instantiate the box
 * \param pokemon_box_out A pointer to the box struct to populate
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any pointer parameter is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the game is invalid
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_box_init(
    const char* p_game,
    struct pkmn_pokemon_box* p_pokemon_box_out
);

/*!
 * @brief Frees a pkmn_pokemon_box instance.
 *
 * All dynamically allocated memory will be freed, and all pointers will be
 * set to NULL.
 *
 * If this call fails, the pokemon_box_ptr struct will be in an undefined state.
 *
 * \param pokemon_box_ptr A pointer to the box struct to free
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if pokemon_box_ptr is NULL
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_box_free(
    struct pkmn_pokemon_box* p_pokemon_box
);

/*!
 * @brief Returns the error message resulting from the last function that used
 *        the given pkmn_pokemon_box.
 *
 * \returns <b>"None"</b> if the last function call using this pkmn_pokemon_box instance
 *                        was successful
 * \returns <b>NULL</b> if pokemon_box_ptr is NULL
 */
PKMN_C_API const char* pkmn_pokemon_box_strerror(
    const struct pkmn_pokemon_box* p_pokemon_box
);

/*!
 * @brief Returns this box's name.
 *
 * Box names were only supported in Generation II, so this call
 * will fail for Generation I boxes.
 *
 * \param pokemon_box_ptr A pointer to the box struct to use
 * \param name_buffer_out Where to return the name
 * \param name_buffer_length The length of the buffer passed into name_buffer_out
 * \param actual_name_length_out Where to return the actual name length (optional)
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if pokemon_box_ptr or name_buffer_out is NULL
 * \returns ::PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR if the box is from a Generation I game
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_box_get_name(
    const struct pkmn_pokemon_box* p_pokemon_box,
    char* p_name_buffer_out,
    size_t name_buffer_length,
    size_t* p_actual_name_length_out
);

/*!
 * @brief Sets this box's name.
 *
 * Valid box names are of length [1-8].
 *
 * Box names were only supported in Generation II, so this call
 * will fail for Generation I boxes.
 *
 * \param pokemon_box_ptr A pointer to the box struct to use
 * \param name The new name for the box
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if either parameter is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the box name is not of length [1-8]
 * \returns ::PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR if the box is from a Generation I game
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_box_set_name(
    const struct pkmn_pokemon_box* p_pokemon_box,
    const char* p_name
);

/*!
 * @brief Returns the number of Pokémon currently in the box.
 *
 * This value will always be in the range [0-6].
 *
 * \param pokemon_box_ptr A pointer to the box struct to use
 * \param num_pokemon_out Where to return the number of Pokémon
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if either parameter is NULL
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_box_get_num_pokemon(
    const struct pkmn_pokemon_box* p_pokemon_box,
    size_t* p_num_pokemon_out
);

/*!
 * @brief Returns the Pokémon at the given position (0-based).
 *
 * If the struct pointed to by pokemon_out already contains a Pokémon,
 * its memory will be leaked, as this function does not free the memory.
 *
 * If any error occurs, the struct pointed to by pokemon_out will not be
 * affected.
 *
 * \param pokemon_box_ptr A pointer to the box struct to use
 * \param position The position on the box whose Pokémon to return
 * \param pokemon_out Where to return the Pokémon
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if either pointer parameter is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE If index is not in the range [0-capacity]
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_box_get_pokemon(
    const struct pkmn_pokemon_box* p_pokemon_box,
    size_t position,
    struct pkmn_pokemon* p_pokemon_out
);

/*!
 * @brief Copies the given Pokémon to the given position in the box (0-based).
 *
 * If the given Pokémon is not from the same game as the box, LibPKMN
 * will attempt to convert the Pokémon to the given game, and this
 * converted Pokémon will be added.
 *
 * In Generation I-II, Pokémon in boxes are stored contiguously, so attempting
 * to place a Pokémon in a position past this point will result in an error.
 *
 * \param pokemon_box_ptr A pointer to the box struct to use
 * \param position The position on the team where to copy the Pokémon
 * \param pokemon_ptr A pointer to the Pokémon to copy
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if either pointer parameter is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE If the index is outside the range [0-num_pokemon)
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT If the new Pokémon is incompatible with the box's game
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_box_set_pokemon(
    const struct pkmn_pokemon_box* p_pokemon_box,
    size_t position,
    struct pkmn_pokemon* p_pokemon
);

/*!
 * @brief Returns all Pokémon in the box, represented as a list.
 *
 * If the list pointed to by pokemon_list_out is already in use, its memory
 * will be leaked, as this function does not free the memory.
 *
 * \param pokemon_box_ptr A pointer to the box struct to use
 * \param pokemon_list_out Where to return the Pokémon list
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if either pointer parameter is NULL
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_box_as_list(
    const struct pkmn_pokemon_box* p_pokemon_box,
    struct pkmn_pokemon_list* p_pokemon_list_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_BOX_H */
