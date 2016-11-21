/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_list_gen2_tmhmimpl.hpp"

#include <pksav/gen2/items.h>

#include <algorithm>
#include <cstring>

BOOST_STATIC_CONSTEXPR int TM01 = 305;
BOOST_STATIC_CONSTEXPR int TM50 = 354;
BOOST_STATIC_CONSTEXPR int HM01 = 397;
BOOST_STATIC_CONSTEXPR int HM07 = 403;

static PKMN_CONSTEXPR_OR_INLINE bool ITEM_ID_IS_TM(int num) {
    return (num >= TM01 and num <= TM50);
}

static PKMN_CONSTEXPR_OR_INLINE bool ITEM_ID_IS_HM(int num) {
    return (num >= HM01 and num <= HM07);
}

static PKMN_CONSTEXPR_OR_INLINE bool ITEM_ID_IN_RANGE(int num) {
    return (ITEM_ID_IS_TM(num) or ITEM_ID_IS_HM(num));
}

#define NATIVE_RCAST reinterpret_cast<pksav_gen2_tmhm_pocket_t*>(_native)

namespace pkmn {

    item_list_gen2_tmhmimpl::item_list_gen2_tmhmimpl(
        int item_list_id,
        int game_id,
        void* ptr
    ): item_list_impl(item_list_id, game_id)
    {
        if(ptr) {
            _native = ptr;
            _our_mem = false;

            for(int i = 0; i < 50; ++i) {
                _item_slots[i].item = pkmn::database::item_entry((i+TM01), game_id);
            }
            for(int i = 0; i < 7; ++i) {
                _item_slots[i+50].item = pkmn::database::item_entry((i+HM01), game_id);
            }

            _from_native();
        } else {
            _native = reinterpret_cast<void*>(new pksav_gen2_tmhm_pocket_t);
            std::memset(_native, 0, sizeof(pksav_gen2_tmhm_pocket_t));
            _our_mem = true;

            for(int i = 0; i < 50; ++i) {
                _item_slots[i].item = pkmn::database::item_entry((i+TM01), game_id);
                _item_slots[i].amount = 0;
            }
            for(int i = 0; i < 7; ++i) {
                _item_slots[i+50].item = pkmn::database::item_entry((i+HM01), game_id);
                _item_slots[i+50].amount = 0;
            }
        }
    }

    item_list_gen2_tmhmimpl::~item_list_gen2_tmhmimpl() {
        if(_our_mem) {
            delete NATIVE_RCAST;
        }
    }

    int item_list_gen2_tmhmimpl::get_num_items() {
        int ret = 0;
        for(int i = 0; i < 50; i++) {
            if(NATIVE_RCAST->tm_count[i] > 0) {
                ++ret;
            }
        }
        for(int i = 0; i < 7; i++) {
            if(NATIVE_RCAST->hm_count[i] > 0) {
                ++ret;
            }
        }

        return ret;
    }

    void item_list_gen2_tmhmimpl::add(
        const std::string &name,
        const int amount
    ) {
        if(amount < 1 or amount > 99) {
            throw std::out_of_range("Valid amount: 1-99");
        }

        pkmn::database::item_entry item(name, get_game());
        if(item.get_pocket() != get_name()) {
            throw std::runtime_error("This item is not valid for this list.");
        }

        int item_id = item.get_item_id();
        int position = -1;
        if(ITEM_ID_IS_TM(item_id)) {
            position = item_id - TM01;
        } else if(ITEM_ID_IS_HM(item_id)) {
            position = item_id - 347;
        } else {
            throw std::runtime_error("Invalid item.");
        }

        int new_amount = _item_slots[position].amount + amount;
        _item_slots[position].amount = std::min<int>(99, new_amount);
        _to_native(position);
    }

    void item_list_gen2_tmhmimpl::remove(
        const std::string &name,
        const int amount
    ) {
        if(amount < 1 or amount > 99) {
            throw std::runtime_error("Valid amount: 1-99");
        }

        pkmn::database::item_entry item(name, get_game());
        if(item.get_pocket() != get_name()) {
            throw std::runtime_error("This item is not valid for this list.");
        }

        int item_id = item.get_item_id();
        int position = -1;
        if(ITEM_ID_IS_TM(item_id)) {
            position = item_id - TM01;
        } else if(ITEM_ID_IS_HM(item_id)) {
            position = item_id - 347;
        } else {
            throw std::runtime_error("Invalid item.");
        }

        int new_amount = _item_slots[position].amount - amount;
        _item_slots[position].amount = std::max<int>(0, new_amount);
        _to_native(position);
    }

    void item_list_gen2_tmhmimpl::move(
        PKMN_UNUSED(const int position1),
        PKMN_UNUSED(const int position2)
    ) {
        throw std::runtime_error("Cannot move items in this pocket.");
    }

    void item_list_gen2_tmhmimpl::_from_native(
        PKMN_UNUSED(int index)
    ) {
        for(size_t i = 0; i < 50; ++i) {
            _item_slots[i].amount = NATIVE_RCAST->tm_count[i];
        }
        for(size_t i = 0; i < 7; ++i) {
            _item_slots[50+i].amount = NATIVE_RCAST->hm_count[i];
        }
    }

    void item_list_gen2_tmhmimpl::_to_native(
        PKMN_UNUSED(int index)
    ) {
        for(size_t i = 0; i < 50; ++i) {
            NATIVE_RCAST->tm_count[i] = uint8_t(_item_slots[i].amount);
        }
        for(size_t i = 0; i < 7; ++i) {
            NATIVE_RCAST->hm_count[i] = uint8_t(_item_slots[50+i].amount);
        }
    }
}
