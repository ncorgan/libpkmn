/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#define GEN2_CAST(ptr) (static_cast<struct pksav_gen2_item_bag*>(ptr))

#include "item_bag_gen2impl.hpp"
#include "item_list_gbimpl.hpp"
#include "item_list_gen2_keyitemimpl.hpp"
#include "item_list_gen2_tmhmimpl.hpp"

#include <boost/thread/lock_guard.hpp>

#include <cstring>

namespace pkmn {

    item_bag_gen2impl::item_bag_gen2impl(
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
            _p_native = new struct pksav_gen2_item_bag;
            std::memset(_p_native, 0, sizeof(struct pksav_gen2_item_bag));
            GEN2_CAST(_p_native)->item_pocket.terminator = 0xFF;
            GEN2_CAST(_p_native)->key_item_pocket.terminator = 0xFF;
            GEN2_CAST(_p_native)->ball_pocket.terminator = 0xFF;
            _is_our_mem = true;
        }

        _set_ptrs();
    }

    item_bag_gen2impl::~item_bag_gen2impl()
    {
        if(_is_our_mem)
        {
            delete GEN2_CAST(_p_native);
        }
    }

    void item_bag_gen2impl::_set_ptrs()
    {
        bool crystal = (_game_id == 6);

        int item_pocket_id     = crystal ? 10 : 5;
        int ball_pocket_id     = crystal ? 11 : 6;
        int key_item_pocket_id = crystal ? 12 : 7;
        int tmhm_pocket_id     = crystal ? 13 : 8;

        _item_pockets["Items"]    = std::make_shared<item_list_gen2_item_pocketimpl>(
                                        item_pocket_id,
                                        _game_id,
                                        &GEN2_CAST(_p_native)->item_pocket
                                    );
        _item_pockets["KeyItems"] = std::make_shared<item_list_gen2_keyitemimpl>(
                                        key_item_pocket_id,
                                        _game_id,
                                        &GEN2_CAST(_p_native)->key_item_pocket
                                    );
        _item_pockets["Balls"]    = std::make_shared<item_list_gen2_ball_pocketimpl>(
                                        ball_pocket_id,
                                        _game_id,
                                        &GEN2_CAST(_p_native)->ball_pocket
                                    );
        _item_pockets["TM/HM"]    = std::make_shared<item_list_gen2_tmhmimpl>(
                                        tmhm_pocket_id,
                                        _game_id,
                                        &GEN2_CAST(_p_native)->tmhm_pocket
                                    );
    }
}
