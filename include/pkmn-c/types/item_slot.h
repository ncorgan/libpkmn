/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_ITEM_SLOT_H
#define PKMN_C_TYPES_ITEM_SLOT_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

/*!
 * @brief A struct representing a slot in an item list.
 *
 * This struct is populated by ::pkmn_item_list_at and must be freed with
 * ::pkmn_item_slot_free.
 */
typedef struct
{
    //! The item name.
    char* item;
    //! The item amount, in the range [0-99].
    size_t amount;
} pkmn_item_slot_t;

/*!
 * @brief A more C-like representation of an item list.
 *
 * This struct is populated by ::pkmn_item_list_as_list and must be freed
 * with ::pkmn_item_slots_free.
 */
typedef struct
{
    //! The list of items, dynamically allocated.
    pkmn_item_slot_t* item_slots;
    //! The number of items in the list.
    size_t length;
} pkmn_item_slots_t;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Frees the dynamically allocated memory in a pkmn_item_slot_t.
 *
 * \param item_slot_ptr The item slot to free
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if item_slot_ptr is NULL
 */
PKMN_C_API pkmn_error_t pkmn_item_slot_free(
    pkmn_item_slot_t* item_slot_ptr
);

/*!
 * @brief Frees the dynamically allocated memory in a pkmn_item_slots_t.
 *
 * \param item_slots_ptr The item slots to free
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if item_slots_ptr is NULL
 */
PKMN_C_API pkmn_error_t pkmn_item_slots_free(
    pkmn_item_slots_t* item_slots_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_ITEM_SLOT_H */
