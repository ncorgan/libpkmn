/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_DATABASE_ITEM_ENTRY_H
#define PKMN_C_DATABASE_ITEM_ENTRY_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/enums/game.h>
#include <pkmn-c/enums/item.h>

#include <stdbool.h>

/*!
 * @brief A struct representing a database entry for an in-game item.
 *
 * Do not attempt to use this struct before passing it into
 * ::pkmn_database_get_item_entry, or it will most likely crash.
 */
struct pkmn_database_item_entry
{
    /*!
     * @brief The item's name in the given game.
     *
     * If this entry corresponds to a "None" item or an empty item slot,
     * this field will be "None".
     *
     * If this entry corresponds to an invalid item, this field will be
     * "Invalid (0xXX)", where XX corresponds to the hex value of the
     * item's in-game index.
     */
    char* p_name;
    /*!
     * @brief The game associated with this entry.
     */
    enum pkmn_game game;
    /*!
     * @brief This item's category.
     *
     * If this entry corresponds to a "None" item or an empty item slot,
     * this field will be "None".
     *
     * If this entry corresponds to an invalid item, this field will be
     * "Unknown".
     */
    char* p_category;
    /*!
     * @brief Which pocket would store this item in the given game.
     */
    char* p_pocket;
    /*!
     * @brief A description of this item.
     *
     * If this entry corresponds to a TM/HM, this string will be "Teaches the
     * move $name.", where $name corresponds to the move name.
     *
     * If this entry corresponds to a "None" item or an empty item slot,
     * this string will be "None".
     *
     * If this entry corresponds to an invalid item, this string will be
     * "Unknown".
     */
    char* p_description;
    /*!
     * @brief The price of this item if bought at a Poké Mart.
     *
     * If this item is a key item (and thus cannot be bought or sold), this
     * field will return 0.
     *
     * If this item corresponds to a "None" item, an empty item slot, or
     * an invalid item, this field will be -1.
     */
    int cost;
    /*!
     * @brief Whether or not a Pokémon can hold this item.
     *
     * Pokémon could not hold items in Generation I, so if this entry
     * corresponds to an item in Red, Blue, or Yellow, this value will
     * be false.
     *
     * If this item corresponds to a "None" item, an empty item slot, or
     * an invalid item, this value will be false.
     */
    bool holdable;
    /*!
     * @brief The base power of the move Fling when this item is held.
     *
     * The move Fling was introduced in Generation IV, so if this entry
     * corresponds to a game from a previous generation, this value will be
     * -1.
     *
     * If this item cannot be used with the move Fling, this value will be -1.
     *
     * If this item corresponds to a "None" item, an empty item slot, or
     * an invalid item, this value will be -1.
     */
    int fling_power;
    /*!
     * @brief The effect of the move Fling when this item is held.
     */
    char* p_fling_effect;
};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Returns an item entry based on its name and game.
 *
 * \param item_name Item's name (does not need to match name in given game)
 * \param item_game Which game this item is from
 * \param item_entry_out Where to return the item entry
 * \return ::PKMN_ERROR_NONE upon success
 * \return ::PKMN_ERROR_NULL_POINTER if any parameter is NULL
 * \return ::PKMN_ERROR_INVALID_ARGUMENT if the item or game name is invalid
 * \return ::PKMN_ERROR_INVALID_ARGUMENT if the given item was not in the given game
 */
PKMN_C_API enum pkmn_error pkmn_database_get_item_entry(
    const char* p_item_name,
    enum pkmn_game game,
    struct pkmn_database_item_entry* p_item_entry_out
);

/*!
 * @brief Frees all memory allocated by ::pkmn_database_get_item_entry.
 *
 * Do not attempt to use this function on a struct pkmn_database_item_entry after
 * already freeing it, or it will most likely crash.
 *
 * \param item_entry A pointer to the item entry struct to free
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if item_entry is NULL
 */
PKMN_C_API enum pkmn_error pkmn_database_item_entry_free(
    struct pkmn_database_item_entry* p_item_entry
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_DATABASE_ITEM_ENTRY_H */
