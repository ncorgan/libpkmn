/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_SLOT_HPP
#define PKMN_ITEM_SLOT_HPP

#include <pkmn/config.hpp>
#include <pkmn/database/item_entry.hpp>

#include <string>
#include <vector>

namespace pkmn {

    /*!
     * @brief A slot in an item list.
     *
     * An item list is made up of a list of item names, each of which contains
     * a unique item type and the amount of that item in the given pocket. This
     * struct represents a single one of those slots.
     */
    struct item_slot {
        /*!
         * @brief Default constructor.
         *
         * This represents an empty item.
         */
        PKMN_INLINE item_slot():
            item("None"),
            amount(0) {}

        /*!
         * @brief Constructor that takes in copies of each member.
         *
         * This calls the copy constructor for the string.
         *
         * \param item_name item name
         * \param item_amount how many of the item
         */
        PKMN_INLINE item_slot(
            const std::string& item_name,
            int item_amount
        ): item(item_name),
           amount(item_amount) {}

#ifndef SWIG
        /*!
         * @brief Constructor that takes in references to each member.
         *
         * This calls the move constructor for the string.
         *
         * \param item_name item entry
         * \param item_amount how many of the item
         */
        PKMN_INLINE item_slot(
            std::string&& item_name,
            int item_amount
        ): item(std::move(item_name)),
           amount(item_amount) {}
#endif

        /*!
         * @brief The item in the given slot.
         */
        std::string item;
        /*!
         * @brief How many of the item is in the given slot.
         */
        int amount;
    };

    #ifndef SWIG
    /*!
     * @brief Equality check between two item slots.
     */
    PKMN_INLINE bool operator==(
        const item_slot& lhs,
        const item_slot& rhs
    ) {
        return (lhs.item == rhs.item) and \
               (lhs.amount == rhs.amount);
    }

    /*!
     * @brief Inequality check between two item slots.
     */
    PKMN_INLINE bool operator!=(
        const item_slot& lhs,
        const item_slot& rhs
    ) {
        return (lhs.item != rhs.item) or \
               (lhs.amount != rhs.amount);
    }
    #endif /* SWIG */

    /*!
     * @brief List of item slots.
     *
     * This is effectively a vector representation of an item list.
     */
    typedef std::vector<item_slot> item_slots_t;
}

#endif /* PKMN_ITEM_SLOT_HPP */
