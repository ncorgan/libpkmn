/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#define GEN1_CAST(ptr) (static_cast<struct pksav_gen1_item_bag*>(ptr))

#include "item_bag_gen1impl.hpp"
#include "item_list_gbimpl.hpp"

#include <boost/thread/lock_guard.hpp>

#include <cstring>

namespace pkmn {

    item_bag_gen1impl::item_bag_gen1impl(
        int game_id,
        void* p_native
    ): item_bag_impl(game_id)
    {
        if(p_native)
        {
            _p_native = p_native;
            _is_our_mem = false;
        }
        else
        {
            _p_native = new struct pksav_gen1_item_bag;
            std::memset(_p_native, 0, sizeof(struct pksav_gen1_item_bag));
            GEN1_CAST(_p_native)->terminator = 0xFF;
            _is_our_mem = true;
        }

        _set_ptrs();
    }

    item_bag_gen1impl::~item_bag_gen1impl()
    {
        if(_is_our_mem)
        {
            delete GEN1_CAST(_p_native);
        }
    }

    void item_bag_gen1impl::_set_ptrs()
    {
        BOOST_STATIC_CONSTEXPR int YELLOW_GAME_ID = 3;
        BOOST_STATIC_CONSTEXPR int RB_ITEM_POCKET_ID = 1;
        BOOST_STATIC_CONSTEXPR int YELLOW_ITEM_POCKET_ID = 3;

        _item_pockets["Items"] = std::make_shared<item_list_gen1_bagimpl>(
                                     (_game_id == YELLOW_GAME_ID)
                                         ? YELLOW_ITEM_POCKET_ID
                                         : RB_ITEM_POCKET_ID,
                                     _game_id, _p_native
                                 );
    }
}
