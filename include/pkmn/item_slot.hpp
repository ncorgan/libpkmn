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

    struct item_slot {
        PKMN_INLINE item_slot() {}

        PKMN_INLINE item_slot(
            const pkmn::database::item_entry &slot_item,
            int slot_amount
        ): item(slot_item),
           amount(slot_amount) {}

#ifndef SWIG
        PKMN_INLINE item_slot(
            pkmn::database::item_entry&& slot_item,
            int slot_amount
        ): item(slot_item),
           amount(slot_amount) {}
#endif

        pkmn::database::item_entry item;
        int amount;
    };

    #ifndef SWIG
    PKMN_INLINE bool operator==(
        const item_slot &lhs,
        const item_slot &rhs
    ) {
        return (lhs.item == rhs.item) and \
               (lhs.amount == rhs.amount);
    }

    PKMN_INLINE bool operator!=(
        const item_slot &lhs,
        const item_slot &rhs
    ) {
        return (lhs.item != rhs.item) or \
               (lhs.amount != rhs.amount);
    }
    #endif /* SWIG */

    typedef std::vector<item_slot> item_slots_t;
}

#endif /* PKMN_ITEM_SLOT_HPP */
