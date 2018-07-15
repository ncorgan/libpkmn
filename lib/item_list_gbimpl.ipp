/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_LIST_GBIMPL_IPP
#define PKMN_ITEM_LIST_GBIMPL_IPP

#include <pkmn/database/item_entry.hpp>

#include <cstring>

namespace pkmn {

    template<typename pksav_type>
    item_list_gbimpl<pksav_type>::item_list_gbimpl(
        int item_list_id,
        int game_id,
        const pksav_type* p_pksav_list
    ): item_list_impl(item_list_id, game_id)
    {
        if(p_pksav_list != nullptr)
        {
            _pksav_list = *p_pksav_list;
        }
        else
        {
            std::memset(&_pksav_list, 0, sizeof(_pksav_list));
            _pksav_list.terminator = 0xFF;
        }

        _from_native();

        _p_native = &_pksav_list;
    }


    template<typename pksav_type>
    void item_list_gbimpl<pksav_type>::_pksav_item_to_libpkmn_item_slot(
        const struct pksav_gb_item& pksav_item,
        pkmn::item_slot& r_item_slot
    )
    {
        r_item_slot.item = pkmn::database::item_entry(
                               pksav_item.index,
                               _game_id
                           ).get_name();
        r_item_slot.amount = pksav_item.count;
    }

    template<typename pksav_type>
    void item_list_gbimpl<pksav_type>::_libpkmn_item_slot_to_pksav_item(
        const pkmn::item_slot& item_slot,
        struct pksav_gb_item& r_pksav_item
    )
    {
        r_pksav_item.index = uint8_t(pkmn::database::item_entry(
                                         item_slot.item,
                                         get_game()
                                     ).get_item_index());
        r_pksav_item.count = uint8_t(item_slot.amount);
    }

    template<typename pksav_type>
    void item_list_gbimpl<pksav_type>::_from_native(int index)
    {
        if(index == -1)
        {
            for(int item_index = 0; item_index < _capacity; ++item_index)
            {
                _pksav_item_to_libpkmn_item_slot(
                    _pksav_list.items[item_index],
                    _item_slots[item_index]
                );
            }
        }
        else
        {
            _pksav_item_to_libpkmn_item_slot(
                _pksav_list.items[index],
                _item_slots[index]
            );
        }

        _num_items = _pksav_list.count;
    }

    template<typename pksav_type>
    void item_list_gbimpl<pksav_type>::_to_native(int index)
    {
        if(index == -1)
        {
            for(int item_index = 0; item_index < _capacity; ++item_index)
            {
                _libpkmn_item_slot_to_pksav_item(
                    _item_slots[item_index],
                    _pksav_list.items[item_index]
                );
            }
        }
        else
        {
            _libpkmn_item_slot_to_pksav_item(
                _item_slots[index],
                _pksav_list.items[index]
            );
        }

        _pksav_list.count = uint8_t(_num_items);
    }

}

#endif /* PKMN_ITEM_LIST_GBIMPL_IPP */
