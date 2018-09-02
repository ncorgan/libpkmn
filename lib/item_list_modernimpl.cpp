/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_list_modernimpl.hpp"

#include <pkmn/database/item_entry.hpp>

#include <pksav/math/endian.h>

#include <cstring>

namespace pkmn {

    item_list_modernimpl::item_list_modernimpl(
        int item_list_id,
        int game_id,
        const struct pksav_item* p_pksav_list
    ): item_list_impl(item_list_id, game_id)
    {
       _pksav_list.resize(_capacity, {0,0});

       if(p_pksav_list != nullptr)
       {
           std::memcpy(
               _pksav_list.data(),
               p_pksav_list,
               sizeof(struct pksav_item)*_capacity
           );
       }

       _from_native();

       _p_native = _pksav_list.data();
    }

    void item_list_modernimpl::_pksav_item_to_libpkmn_item_slot(
        const struct pksav_item& pksav_item,
        pkmn::item_slot& r_item_slot
    )
    {
        r_item_slot.item = pkmn::database::item_entry(
                               pksav_littleendian16(pksav_item.index),
                               _game_id
                           ).get_item();
        r_item_slot.amount = pksav_littleendian16(pksav_item.count);
    }

    void item_list_modernimpl::_libpkmn_item_slot_to_pksav_item(
        const pkmn::item_slot& item_slot,
        struct pksav_item& r_pksav_item
    )
    {
        r_pksav_item.index = pksav_littleendian16(uint16_t(
                                 pkmn::database::item_entry(
                                     item_slot.item,
                                     get_game()
                                 ).get_item_index()));
        r_pksav_item.count = pksav_littleendian16(uint16_t(item_slot.amount));
    }

    void item_list_modernimpl::_from_native(int index)
    {
        if(index == -1)
        {
            for(int item_index = 0; item_index < _capacity; ++item_index)
            {
                _pksav_item_to_libpkmn_item_slot(
                    _pksav_list[item_index],
                    _item_slots[item_index]
                );
            }
        }
        else
        {
            _pksav_item_to_libpkmn_item_slot(
                _pksav_list[index],
                _item_slots[index]
            );
        }
    }

    void item_list_modernimpl::_to_native(
        int index
    )
    {
        if(index == -1)
        {
            for(int item_index = 0; item_index < _capacity; ++item_index)
            {
                _libpkmn_item_slot_to_pksav_item(
                    _item_slots[item_index],
                    _pksav_list[item_index]
                );
            }
        }
        else
        {
            _libpkmn_item_slot_to_pksav_item(
                _item_slots[index],
                _pksav_list[index]
            );
        }
    }
}
