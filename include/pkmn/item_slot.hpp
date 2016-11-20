/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_SLOT_HPP
#define PKMN_ITEM_SLOT_HPP

#include <pkmn/config.hpp>
#include <pkmn/database/item_entry.hpp>

#include <vector>

namespace pkmn {

    /*!
     * @brief A slot in an item list.
     *
     * An item list is made up of a list of item slots, each of which contains
     * a unique item type and the amount of that item in the given pocket. This
     * struct represents a single one of those slots.
     */
    struct item_slot {
        /*!
         * @brief Default constructor.
         *
         * The item entry represents an invalid item.
         */
        PKMN_CONSTEXPR_OR_INLINE item_slot():
            item(pkmn::database::item_entry()),
            amount(0) {}

        /*!
         * @brief Constructor that takes in copies of each member.
         *
         * This calls the copy constructor for the item entry.
         *
         * \param slot_item item entry
         * \param slot_amount how many of the item
         */
        PKMN_CONSTEXPR_OR_INLINE item_slot(
            const pkmn::database::item_entry &slot_item,
            int slot_amount
        ): item(slot_item),
           amount(slot_amount) {}

#ifndef SWIG
        /*!
         * @brief Constructor that takes in references to each member.
         *
         * This calls the move constructor for the item entry.
         *
         * \param slot_item item entry
         * \param slot_amount how many of the item
         */
        PKMN_CONSTEXPR_OR_INLINE item_slot(
            pkmn::database::item_entry&& slot_item,
            int slot_amount
        ): item(slot_item),
           amount(slot_amount) {}
#endif

        /*!
         * @brief The item in the given slot.
         */
        pkmn::database::item_entry item;
        /*!
         * @brief How many of the item is in the given slot.
         */
        int amount;
    };

    #ifndef SWIG
    /*!
     * @brief Equality check between two item slots.
     */
    PKMN_CONSTEXPR_OR_INLINE bool operator==(
        const item_slot &lhs,
        const item_slot &rhs
    ) {
        return (lhs.item == rhs.item) and \
               (lhs.amount == rhs.amount);
    }

    /*!
     * @brief Inequality check between two item slots.
     */
    PKMN_CONSTEXPR_OR_INLINE bool operator!=(
        const item_slot &lhs,
        const item_slot &rhs
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
