/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_list_gcnimpl.hpp"

#include <pksav/math/endian.h>

#include <algorithm>
#include <cstring>

#define NATIVE_RCAST reinterpret_cast<LibPkmGC::Item*>(_native)

namespace pkmn {

    item_list_gcnimpl::item_list_gcnimpl(
        int item_list_id,
        int game_id,
        LibPkmGC::Item* ptr,
        size_t capacity,
        bool copy
    ): item_list_impl(item_list_id, game_id)
    {
        if(ptr) {
            if(copy) {
                _native = reinterpret_cast<void*>(new LibPkmGC::Item[capacity]);
                std::memcpy(_native, ptr, sizeof(LibPkmGC::Item)*capacity);
                _our_mem = true;
            } else {
                _native = ptr;
                _our_mem = false;
            }

            _from_native();
        } else {
            _native = reinterpret_cast<void*>(new LibPkmGC::Item[capacity]);
            std::memset(_native, 0, sizeof(LibPkmGC::Item)*capacity);
            _our_mem = true;
        }
    }

    item_list_gcnimpl::~item_list_gcnimpl() {
        item_list_scoped_lock lock(this);

        if(_our_mem) {
            delete[] NATIVE_RCAST;
        }
    }

    // TODO: endianness?

    void item_list_gcnimpl::_from_native(
        int index
    ) {
        item_list_scoped_lock lock(this);

        if(index == -1) {
            for(int i = 0; i < _capacity; ++i) {
                _item_slots[i].item = pkmn::database::item_entry(
                                          int(NATIVE_RCAST[i].index),
                                          _game_id
                                      );
                _item_slots[i].amount = int(NATIVE_RCAST[i].quantity);
            }
        } else {
            _item_slots[index].item = pkmn::database::item_entry(
                                          int(NATIVE_RCAST[index].index),
                                          _game_id
                                      );
            _item_slots[index].amount = int(NATIVE_RCAST[index].quantity);
        }
    }

    void item_list_gcnimpl::_to_native(
        int index
    ) {
        item_list_scoped_lock lock(this);

        if(index == -1) {
            for(int i = 0; i < _capacity; ++i) {
                NATIVE_RCAST[i].index = LibPkmGC::ItemIndex(
                                            _item_slots[i].item.get_item_index()
                                        );
                NATIVE_RCAST[i].quantity = LibPkmGC::u16(
                                               _item_slots[i].amount
                                           );
            }
        } else {
            NATIVE_RCAST[index].index = LibPkmGC::ItemIndex(
                                            _item_slots[index].item.get_item_index()
                                        );
            NATIVE_RCAST[index].quantity = LibPkmGC::u16(
                                               _item_slots[index].amount
                                           );
        }
    }
}
