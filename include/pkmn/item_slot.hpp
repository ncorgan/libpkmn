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

    typedef struct {
        pkmn::database::item_entry item;
        int amount;
    } item_slot_t;

    #ifndef SWIG
    PKMN_INLINE bool operator==(
        const item_slot_t &lhs,
        const item_slot_t &rhs
    ) {
        return (lhs.item == rhs.item) and \
               (lhs.amount == rhs.amount);
    }

    PKMN_INLINE bool operator!=(
        const item_slot_t &lhs,
        const item_slot_t &rhs
    ) {
        return (lhs.item != rhs.item) or \
               (lhs.amount != rhs.amount);
    }
    #endif /* SWIG */

    typedef std::vector<item_slot_t> item_slots_t;
}

#endif /* PKMN_ITEM_SLOT_HPP */
