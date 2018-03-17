/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_ITEM_LIST_H
#define PKMN_C_ITEM_LIST_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/types/item_slot.h>
#include <pkmn-c/types/string_types.h>

/*!
 * @brief A list of items, corresponding to an in-game bag pocket or item PC.
 *
 * This struct provides an abstraction of the different in-game representations of an
 * item pocket or PC throughout the different games, and the corresponding functions
 * provide a common API for interacting with the items.
 *
 * The rules for interacting with different types of lists, as well as the set
 * of items that can be inserted into a given list, matches the behavior of the list
 * in the given game. As such, this class will never represent an invalid in-game
 * state unless the original item list was already in an invalid state.
 *
 * Any dynamically allocated memory in this struct is initialized by
 * ::pkmn_item_list_init and freed by ::pkmn_item_list_free.
 */
typedef struct
{
    /*!
     * @brief This list's name, as the pocket or PC is referred to in-game.
     */
    char* name;

    /*!
     * @brief The game this item list is associated with.
     */
    char* game;

    /*!
     * @brief How many items this list can hold.
     */
    size_t capacity;

    void* _internal;
} pkmn_item_list_t;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Initializes a pkmn_item_list_t instance, based on the given name and
 *        game.
 *
 * If this call fails, the item_list_out struct will not be altered.
 *
 * \param name The name of the pocket (or PC) whose item list to instantiate
 * \param game The game for which to instantiate the item list
 * \param item_list_out A pointer to the item bag struct to populate
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any pointer parameter is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the name or game is invalid
 */
PKMN_C_API pkmn_error_t pkmn_item_list_init(
    const char* name,
    const char* game,
    pkmn_item_list_t* item_list_out
);

/*!
 * @brief Frees a pkmn_item_list_t instance.
 *
 * All dynamically allocated memory will be freed, and all pointers will be
 * set to NULL.
 *
 * If this call fails, the item_list_ptr struct will be in an undefined state.
 *
 * \param item_list_ptr A pointer to the item list struct to free
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if item_list_ptr is NULL
 */
PKMN_C_API pkmn_error_t pkmn_item_list_free(
    pkmn_item_list_t* item_list_ptr
);

/*!
 * @brief Returns the error message resulting from the last function that used
 *        the given pkmn_item_list_t.
 *
 * \returns <b>"None"</b> if the last function call using this pkmn_item_list_t instance
 *                        was successful
 * \returns <b>NULL</b> if item_list_ptr is NULL
 */
PKMN_C_API const char* pkmn_item_list_strerror(
    pkmn_item_list_t* item_list_ptr
);

/*!
 * @brief Returns how many unique items are currently in this list.
 *
 * \param item_list_ptr A pointer to the item list struct to use
 * \param num_items_out Where to return the number of items in the list
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any pointer parameter is NULL
*/
PKMN_C_API pkmn_error_t pkmn_item_list_get_num_items(
    pkmn_item_list_t* item_list_ptr,
    size_t* num_items_out
);

/*!
 * @brief Returns the item slot at the given position in the list (read-only).
 *
 * The pkmn_item_slot_t returned in item_slot_out will need to be freed with
 * ::pkmn_item_slot_free.
 *
 * \param item_list_ptr A pointer to the item list struct to use
 * \param position 0-based position of the item to return
 * \param item_slot_out Where to return the item slot
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any pointer parameter is NULL
 */
PKMN_C_API pkmn_error_t pkmn_item_list_at(
    pkmn_item_list_t* item_list_ptr,
    size_t position,
    pkmn_item_slot_t* item_slot_out
);

/*!
 * @brief Adds some amount of the given item to the list.
 *
 * If some of this item is already in the pocket, the amount in that slot is
 * increased by the given amount. If not, the next empty slot will be used,
 * assuming one is available.
 *
 * \param item_list_ptr A pointer to the item list struct to use
 * \param item Name of the item to add
 * \param amount How many of the item to add
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any pointer parameter is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT If the item cannot be included in the given list
 * \returns ::PKMN_ERROR_OUT_OF_RANGE If amount is outside the range [1,99]
 */
PKMN_C_API pkmn_error_t pkmn_item_list_add(
    pkmn_item_list_t* item_list_ptr,
    const char* item,
    size_t amount
);

/*!
 * @brief Removes some amount of the given item from the list.
 *
 * If some of this item is already in the pocket, the amount in that slot is
 * decreased by the given amount. If all of this item is removed, the slot will
 * be cleared, and all later slots are shifted back.
 *
 * \param item_list_ptr A pointer to the item list struct to use
 * \param item Name of the item to remove
 * \param amount How many of the item to remove
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any pointer parameter is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT if the item cannot be included in the given list
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if amount is outside the range [1,99]
 */
PKMN_C_API pkmn_error_t pkmn_item_list_remove(
    pkmn_item_list_t* item_list_ptr,
    const char* item,
    size_t amount
);

/*!
 * @brief Moves the items in one given slot into another.
 *
 * \param item_list_ptr A pointer to the item list struct to use
 * \param old_position The position of the slot to move
 * \param new_position Where to move the given slot
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if item_list_ptr is NULL
 * \returns ::PKMN_ERROR_OUT_OF_RANGE if either parameter is outside the range [0,num_items-1]
 * \returns ::PKMN_ERROR_RUNTIME_ERROR if items in the given list cannot be moved
*/
PKMN_C_API pkmn_error_t pkmn_item_list_move(
    pkmn_item_list_t* item_list_ptr,
    size_t old_position,
    size_t new_position
);

/*!
 * @brief Sets the given item slot to the given item and amount.
 *
 * \param item_list_ptr A pointer to the item list struct to use
 * \param position The position for which to set the item
 * \param item The name of the item to set
 * \param amount How much of the item to put in the slot
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if item_list_ptr is NULL
 * \returns ::PKMN_ERROR_INVALID_ARGUMENT If the item cannot be included in the given list
 * \returns ::PKMN_ERROR_OUT_OF_RANGE If position is outside the range [0,num_items-1]
 * \returns ::PKMN_ERROR_RUNTIME_ERROR If the pocket's items are locked in one slot and the
 *                                     given item does not match that slot
*/
PKMN_C_API pkmn_error_t pkmn_item_list_set_item(
    pkmn_item_list_t* item_list_ptr,
    size_t position,
    const char* item,
    size_t amount
);

/*!
 * @brief Returns a list of the items that can be added to this list.
 *
 * The pkmn_string_list_t returned in valid_items_out will need to be freed by
 * ::pkmn_string_list_free.
 *
 * If this item list corresponds to an item PC, this list of names will include almost
 * every item in the given game, minus certain Key Items.
 *
 * \param item_list_ptr A pointer to the item list struct to use
 * \param valid_items_out Where to return the list of valid items
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if either parameter is NULL
*/
PKMN_C_API pkmn_error_t pkmn_item_list_get_valid_items(
    pkmn_item_list_t* item_list_ptr,
    pkmn_string_list_t* valid_items_out
);

/*!
 * @brief Returns the full list of items, represented as a list struct.
 *
 * The pkmn_item_slots_t returned in item_slots_out will need to be freed by
 * ::pkmn_item_slots_free.
 *
 * \param item_list_ptr A pointer to the item list struct to use
 * \param item_slots_out Where to return the item list struct
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if either parameter is NULL
 */
PKMN_C_API pkmn_error_t pkmn_item_list_as_list(
    pkmn_item_list_t* item_list_ptr,
    pkmn_item_slots_t* item_slots_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ITEM_LIST_H */
