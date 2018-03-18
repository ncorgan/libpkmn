/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#define NATIVE_RCAST (reinterpret_cast<pksav_gen1_item_bag_t*>(_native))

#include "item_bag_gen1impl.hpp"
#include "item_list_gbimpl.hpp"

#include <boost/thread/lock_guard.hpp>

#include <cstring>

namespace pkmn {

    item_bag_gen1impl::item_bag_gen1impl(
        int game_id,
        void* ptr
    ): item_bag_impl(game_id)
    {
        if(ptr)
        {
            _native = ptr;
            _our_mem = false;
        }
        else
        {
            _native = reinterpret_cast<void*>(new pksav_gen1_item_bag_t);
            std::memset(_native, 0, sizeof(pksav_gen1_item_bag_t));
            NATIVE_RCAST->terminator = 0xFF;
            _our_mem = true;
        }

        _set_ptrs();
    }

    item_bag_gen1impl::item_bag_gen1impl(
        int game_id,
        const pksav_gen1_item_bag_t &item_bag
    ): item_bag_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pksav_gen1_item_bag_t);
        *NATIVE_RCAST = item_bag;
        _our_mem = true;

        _set_ptrs();
    }

    item_bag_gen1impl::~item_bag_gen1impl()
    {
        boost::lock_guard<item_bag_gen1impl> lock(*this);

        if(_our_mem)
        {
            delete NATIVE_RCAST;
        }
    }

    void item_bag_gen1impl::_set_ptrs() {
        _item_pockets["Items"] = std::make_shared<item_list_gen1_bagimpl>(
                                     (_game_id == 3) ? 3 : 1, _game_id, _native
                                 );
    }
}
