/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_list_modernimpl.hpp"

#include <pksav/math/endian.h>

#include <algorithm>
#include <cstring>

#define NATIVE_RCAST reinterpret_cast<pksav_item_t*>(_native)

namespace pkmn {

    item_list_modernimpl::item_list_modernimpl(
        int item_list_id,
        int game_id,
        pksav_item_t* ptr,
        size_t capacity,
        bool copy
    ): item_list_impl(item_list_id, game_id)
    {
        if(ptr) {
            if(copy) {
                _native = reinterpret_cast<void*>(new pksav_item_t[capacity]);
                std::memcpy(_native, ptr, sizeof(pksav_item_t)*capacity);
                _our_mem = true;
            } else {
                _native = ptr;
                _our_mem = false;
            }

            _from_native();
        } else {
            _native = reinterpret_cast<void*>(new pksav_item_t[capacity]);
            std::memset(_native, 0, sizeof(pksav_item_t)*capacity);
            _our_mem = true;

        }
    }

    item_list_modernimpl::~item_list_modernimpl() {
        item_list_scoped_lock(this);

        if(_our_mem) {
            delete[] NATIVE_RCAST;
        }
    }

    void item_list_modernimpl::_from_native(
        int index
    ) {
        item_list_scoped_lock(this);

        if(index == -1) {
            for(int i = 0; i < _capacity; ++i) {
                _item_slots[i].item = pkmn::database::item_entry(
                                          pksav_littleendian16(NATIVE_RCAST[i].index),
                                          _game_id
                                      );
                _item_slots[i].amount = pksav_littleendian16(NATIVE_RCAST[i].count);
            }
        } else {
            _item_slots[index].item = pkmn::database::item_entry(
                                          pksav_littleendian16(NATIVE_RCAST[index].index),
                                          _game_id
                                      );
            _item_slots[index].amount = pksav_littleendian16(NATIVE_RCAST[index].count);
        }
    }

    void item_list_modernimpl::_to_native(
        int index
    ) {
        item_list_scoped_lock(this);

        if(index == -1) {
            for(int i = 0; i < _capacity; ++i) {
                NATIVE_RCAST[i].index = pksav_littleendian16(uint16_t(
                                            _item_slots[i].item.get_item_index()
                                        ));
                NATIVE_RCAST[i].count = pksav_littleendian16(uint16_t(
                                            _item_slots[i].amount
                                        ));
            }
        } else {
            NATIVE_RCAST[index].index = pksav_littleendian16(uint16_t(
                                            _item_slots[index].item.get_item_index()
                                        ));
            NATIVE_RCAST[index].count = pksav_littleendian16(uint16_t(
                                            _item_slots[index].amount
                                        ));
        }
    }
}
