/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_LIST_GBIMPL_IPP
#define PKMN_ITEM_LIST_GBIMPL_IPP

#include <pkmn/database/item_entry.hpp>

#include <boost/thread/lock_guard.hpp>

#include <cstring>

#define GBLIST_RCAST reinterpret_cast<list_type*>(_native)

namespace pkmn {

    template<typename list_type>
    item_list_gbimpl<list_type>::item_list_gbimpl(
        int item_list_id,
        int game_id,
        void* ptr
    ): item_list_impl(item_list_id, game_id)
    {
        if(ptr) {
            _native = ptr;
            _our_mem = false;

            _from_native();
        } else {
            _native = reinterpret_cast<void*>(new list_type);
            std::memset(_native, 0, sizeof(list_type));
            GBLIST_RCAST->terminator = 0xFF;
            _our_mem = true;
        }
    }

    template<typename list_type>
    item_list_gbimpl<list_type>::item_list_gbimpl(
        int item_list_id,
        int game_id,
        const list_type &list
    ): item_list_impl(item_list_id, game_id)
    {
        _native = reinterpret_cast<void*>(new list_type);
        *GBLIST_RCAST = list;
        _our_mem = true;
        _from_native();
    }

    template<typename list_type>
    item_list_gbimpl<list_type>::~item_list_gbimpl()
    {
        boost::lock_guard<item_list_gbimpl<list_type>>(*this);

        if(_our_mem)
        {
            delete GBLIST_RCAST;
        }
    }

    template<typename list_type>
    void item_list_gbimpl<list_type>::_from_native(
        int index
    )
    {
        boost::lock_guard<item_list_gbimpl<list_type>>(*this);

        if(index == -1) {
            for(int i = 0; i < _capacity; ++i) {
                _item_slots[i].item = pkmn::database::item_entry(
                                          GBLIST_RCAST->items[i].index,
                                          _game_id
                                      ).get_name();
                _item_slots[i].amount = GBLIST_RCAST->items[i].count;
            }
        } else {
            _item_slots[index].item = pkmn::database::item_entry(
                                          GBLIST_RCAST->items[index].index,
                                          _game_id
                                      ).get_name();
            _item_slots[index].amount = GBLIST_RCAST->items[index].count;
        }

        _num_items = GBLIST_RCAST->count;
    }

    template<typename list_type>
    void item_list_gbimpl<list_type>::_to_native(
        int index
    )
    {
        boost::lock_guard<item_list_gbimpl<list_type>>(*this);

        bool count_set = false;
        if(index == -1)
        {
            for(int i = 0; i < _capacity; ++i)
            {
                GBLIST_RCAST->items[i].index = uint8_t(pkmn::database::item_entry(
                                                           _item_slots[i].item,
                                                           get_game()
                                                       ).get_item_index());
                GBLIST_RCAST->items[i].count = uint8_t(_item_slots[i].amount);
                if(not count_set and GBLIST_RCAST->items[i].index == 0)
                {
                    GBLIST_RCAST->count = uint8_t(i);
                    count_set = true;
                }
            }
        }
        else
        {
            GBLIST_RCAST->items[index].index = uint8_t(pkmn::database::item_entry(
                                                           _item_slots[index].item,
                                                           get_game()
                                                       ).get_item_index());
            GBLIST_RCAST->items[index].count = uint8_t(_item_slots[index].amount);
            for(int i = 0; i < _capacity and not count_set; ++i)
            {
                if(GBLIST_RCAST->items[i].index == 0)
                {
                    GBLIST_RCAST->count = uint8_t(i);
                    count_set = true;
                }
            }
        }
    }

}

#endif /* PKMN_ITEM_LIST_GBIMPL_IPP */
