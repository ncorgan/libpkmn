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
        const struct pksav_gen2_item_bag* p_pksav_bag
    ): item_bag_impl(game_id)
    {
        if(p_pksav_bag != nullptr)
        {
            _pksav_bag = *p_pksav_bag;
        }
        else
        {
            std::memset(&_pksav_bag, 0, sizeof(_pksav_bag));
            _pksav_bag.item_pocket.terminator = 0xFF;
            _pksav_bag.key_item_pocket.terminator = 0xFF;
            _pksav_bag.ball_pocket.terminator = 0xFF;
        }

        bool is_crystal = (_game_id == 6);

        int item_pocket_id     = is_crystal ? 10 : 5;
        int ball_pocket_id     = is_crystal ? 11 : 6;
        int key_item_pocket_id = is_crystal ? 12 : 7;
        int tmhm_pocket_id     = is_crystal ? 13 : 8;

        _item_pockets["Items"]    = std::make_shared<item_list_gen2_item_pocketimpl>(
                                        item_pocket_id,
                                        _game_id,
                                        &_pksav_bag.item_pocket
                                    );
        _item_pockets["KeyItems"] = std::make_shared<item_list_gen2_keyitemimpl>(
                                        key_item_pocket_id,
                                        _game_id,
                                        &_pksav_bag.key_item_pocket
                                    );
        _item_pockets["Balls"]    = std::make_shared<item_list_gen2_ball_pocketimpl>(
                                        ball_pocket_id,
                                        _game_id,
                                        &_pksav_bag.ball_pocket
                                    );
        _item_pockets["TM/HM"]    = std::make_shared<item_list_gen2_tmhmimpl>(
                                        tmhm_pocket_id,
                                        _game_id,
                                        &_pksav_bag.tmhm_pocket
                                    );
    }
}
