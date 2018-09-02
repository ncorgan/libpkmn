/*
 * Copyright (c) 2017,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_list_gcnimpl.hpp"

#include <pkmn/database/item_entry.hpp>

#include <cstring>

namespace pkmn {

    item_list_gcnimpl::item_list_gcnimpl(
        int item_list_id,
        int game_id,
        const LibPkmGC::Item* p_libpkmgc_list
    ): item_list_impl(item_list_id, game_id)
    {
        _libpkmgc_list.resize(_capacity, {LibPkmGC::NoItem,0});

        if(p_libpkmgc_list != nullptr)
        {
            std::memcpy(
                _libpkmgc_list.data(),
                p_libpkmgc_list,
                sizeof(LibPkmGC::Item)*_capacity
            );
        }

        _from_native();

        _p_native = _libpkmgc_list.data();
    }

    void item_list_gcnimpl::_libpkmgc_item_to_libpkmn_item_slot(
        const LibPkmGC::Item& libpkmgc_item,
        pkmn::item_slot& r_item_slot
    )
    {
        r_item_slot.item = pkmn::database::item_entry(
                               libpkmgc_item.index,
                               _game_id
                           ).get_item();
        r_item_slot.amount = libpkmgc_item.quantity;
    }

    void item_list_gcnimpl::_libpkmn_item_slot_to_libpkmgc_item(
        const pkmn::item_slot& item_slot,
        LibPkmGC::Item& r_libpkmgc_item
    )
    {
        r_libpkmgc_item.index = LibPkmGC::ItemIndex(
                                    pkmn::database::item_entry(
                                        item_slot.item,
                                        get_game()
                                    ).get_item_index()
                                );
        r_libpkmgc_item.quantity = LibPkmGC::u16(item_slot.amount);
    }

    void item_list_gcnimpl::_from_native(int index)
    {
        if(index == -1)
        {
            for(int item_index = 0; item_index < _capacity; ++item_index)
            {
                _libpkmgc_item_to_libpkmn_item_slot(
                    _libpkmgc_list[item_index],
                    _item_slots[item_index]
                );
            }
        }
        else
        {
            _libpkmgc_item_to_libpkmn_item_slot(
                _libpkmgc_list[index],
                _item_slots[index]
            );
        }
    }

    void item_list_gcnimpl::_to_native(int index)
    {
        if(index == -1)
        {
            for(int item_index = 0; item_index < _capacity; ++item_index)
            {
                _libpkmn_item_slot_to_libpkmgc_item(
                    _item_slots[item_index],
                    _libpkmgc_list[item_index]
                );
            }
        }
        else
        {
            _libpkmn_item_slot_to_libpkmgc_item(
                _item_slots[index],
                _libpkmgc_list[index]
            );
        }
    }
}
