/*
 * p_Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEMON_PARTY_H
#define PKMN_C_POKEMON_PARTY_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/types/pokemon_list.h>

/*!
 * @brief A list of the Pokémon a trainer carries with them.
 *
 * This struct provides an abstraction of the different in-game representations of
 * a Pokémon party throughout the different games, and the corresponding functions
 * provide a common API for interacting with the party.
 */
struct pkmn_pokemon_party
{
    /*!
     * @brief The game this party is associated with.
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
 * @brief Initializes a pkmn_pokemon_party instance, based on the given game.
 *
 * If this call fails, the pokemon_party_out struct will not be altered.
 *
 * \param game The game for which to instantiate the party
 * \param pokemon_party_out A pointer to the party struct to populate
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any pointer parameter is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the game is invalid
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_party_init(
    const char* p_game,
    struct pkmn_pokemon_party* p_pokemon_party_out
);

/*!
 * @brief Frees a pkmn_pokemon_party instance.
 *
 * All dynamically allocated memory will be freed, and all pointers will be
 * set to NULL.
 *
 * If this call fails, the pokemon_party_ptr struct will be in an undefined state.
 *
 * \param pokemon_party_ptr A pointer to the party struct to free
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if pokemon_party_ptr is NULL
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_party_free(
    struct pkmn_pokemon_party* p_pokemon_party
);

/*!
 * @brief Returns the error message resulting from the last function that used
 *        the given pkmn_pokemon_party.
 *
 * \returns <b>"None"</b> if the last function call using this pkmn_pokemon_party instance
 *                        was successful
 * \returns <b>NULL</b> if pokemon_party_ptr is NULL
 */
PKMN_C_API const char* pkmn_pokemon_party_strerror(
    struct pkmn_pokemon_party* p_pokemon_party
);

/*!
 * @brief Returns the number of Pokémon currently in the party.
 *
 * This value will always be in the range [0-6].
 *
 * \param pokemon_party_ptr A pointer to the party struct to use
 * \param num_pokemon_out Where to return the number of Pokémon
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if either parameter is NULL
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_party_get_num_pokemon(
    struct pkmn_pokemon_party* p_pokemon_party,
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
 * \param pokemon_party_ptr A pointer to the party struct to use
 * \param position The position on the party whose Pokémon to return
 * \param pokemon_out Where to return the Pokémon
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if either pointer parameter is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE If index is not in the range [0-5]
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_party_get_pokemon(
    struct pkmn_pokemon_party* p_pokemon_party,
    size_t position,
    struct pkmn_pokemon* p_pokemon_out
);

/*!
 * @brief Copies the given Pokémon to the given position in the party (0-based).
 *
 * If the given Pokémon is not from the same game as the party, LibPKMN
 * will attempt to convert the Pokémon to the given game, and this
 * converted Pokémon will be added.
 *
 * Pokémon in parties are stored contiguously, so attempting to place
 * a Pokémon in a position past this point will result in an error.
 *
 * \param pokemon_party_ptr A pointer to the party struct to use
 * \param position The position on the team where to copy the Pokémon
 * \param pokemon_ptr A pointer to the Pokémon to copy
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if either pointer parameter is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE If the index is outside the range [0-num_pokemon)
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT If the new Pokémon is incompatible with the party's game
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_party_set_pokemon(
    struct pkmn_pokemon_party* p_pokemon_party,
    size_t position,
    struct pkmn_pokemon* p_pokemon
);

/*!
 * @brief Returns all Pokémon in the party, represented as a list.
 *
 * If the list pointed to by pokemon_list_out is already in use, its memory
 * will be leaked, as this function does not free the memory.
 *
 * \param pokemon_party_ptr A pointer to the party struct to use
 * \param pokemon_list_out Where to return the Pokémon list
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if either pointer parameter is NULL
 */
PKMN_C_API enum pkmn_error pkmn_pokemon_party_as_list(
    struct pkmn_pokemon_party* p_pokemon_party,
    struct pkmn_pokemon_list* p_pokemon_list_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_PARTY_H */
