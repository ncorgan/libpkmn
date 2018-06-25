/*
 * p_Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_ITEM_BAG_H
#define PKMN_C_ITEM_BAG_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/item_list.h>

#include <pkmn-c/enums/game.h>

#include <pkmn-c/types/string_types.h>

/*!
 * @brief A bag of items, sorted into different pockets.
 *
 * This struct provides an abstraction of the different in-game
 * representations of a trainer's item bag throughout the different games,
 * and the corresponding functions provide a common API for interacting
 * with the bag.
 *
 * Any dynamically allocated memory in this struct is initialized by
 * ::pkmn_item_bag_init and freed by ::pkmn_item_bag_free.
 */
struct pkmn_item_bag
{
    /*!
     * @brief The game this bag is associated with.
     */
    enum pkmn_game game;

    /*!
     * @brief A list of the names of this bag's pockets.
     *
     * All string entries are guaranteed to be valid inputs for
     * ::pkmn_item_bag_get_pocket.
     */
    struct pkmn_string_list pocket_names;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Initializes a pkmn_item_bag_t instance, based on the given game.
 *
 * If this call fails, the item_bag_out struct will not be altered.
 *
 * \param game The game for which to instantiate the item bag
 * \param item_bag_out A pointer to the item bag struct to populate
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the game is invalid
 * \returns ::PKMN_ERROR_NULL_POINTER if either parameter is NULL
 */
PKMN_C_API enum pkmn_error pkmn_item_bag_init(
    enum pkmn_game game,
    struct pkmn_item_bag* p_item_bag_out
);

/*!
 * @brief Frees a pkmn_item_bag_t instance.
 *
 * All dynamically allocated memory will be freed, and all pointers will be
 * set to NULL.
 *
 * If this call fails, the item_bag_ptr struct will be in an undefined state.
 *
 * \param item_bag_ptr A pointer to the item bag struct to free
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if item_bag_ptr is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if position is outside the range [0,capacity-1]
 */
PKMN_C_API enum pkmn_error pkmn_item_bag_free(
    struct pkmn_item_bag* p_item_bag
);

/*!
 * @brief Returns the error message resulting from the last function that used
 *        the given pkmn_item_bag_t.
 *
 * \returns <b>"None"</b> if the last function call using this pkmn_item_bag_t instance
 *                        was successful
 * \returns <b>NULL</b> if item_bag_ptr is NULL
 */
PKMN_C_API const char* pkmn_item_bag_strerror(
    const struct pkmn_item_bag* p_item_bag
);

/*!
 * @brief Returns an interface to the bag's pocket with the given name.
 *
 * The pkmn_item_list_t returned from this function shares the same underlying
 * memory as the pkmn_item_bag_t that created it, so calling ::pkmn_item_bag_add
 * or ::pkmn_item_bag_remove will affect the pkmn_item_list_t corresponding to
 * the appropriate pocket.
 *
 * \param item_bag_ptr A pointer to the item bag struct to use
 * \param pocket_name Name of the pocket to return
 * \param item_list_out Output pointer in which to return the pocket
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any pointer parameter is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the given name does not correspond to a
 *                                        valid pocket
 */
PKMN_C_API enum pkmn_error pkmn_item_bag_get_pocket(
    const struct pkmn_item_bag* p_item_bag,
    const char* pocket_name,
    struct pkmn_item_list* p_item_list_out
);

/*!
 * @brief Adds the given amount of the given item to the appropriate pocket.
 *
 * This function figures out the appropriate pocket based on the given item and
 * adds the item to it.
 *
 * \param item_bag_ptr A pointer to the item bag struct to use
 * \param item Name of the item to add
 * \param amount How many of the item to add
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any pointer parameter is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the item name is invalid
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the item cannot be added to any
 *                                        of this bag's pockets
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if amount is not in the range [0,99]
 */
PKMN_C_API enum pkmn_error pkmn_item_bag_add(
    const struct pkmn_item_bag* p_item_bag,
    const char* p_item,
    size_t amount
);

/*!
 * @brief Removes the given amount of the given item from the appropriate pocket.
 *
 * This function figures out the appropriate pocket based on the given item and
 * removes the item from it.
 *
 * \param item_bag_ptr A pointer to the item bag struct to use
 * \param item Name of the item to add
 * \param amount How many of the item to add
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any pointer parameter is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the item name is invalid
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the item cannot be removed from any
 *                                        of this bag's pockets
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if amount is not in the range [0,99]
 */
PKMN_C_API enum pkmn_error pkmn_item_bag_remove(
    const struct pkmn_item_bag* p_item_bag,
    const char* p_item,
    size_t amount
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ITEM_BAG_H */
