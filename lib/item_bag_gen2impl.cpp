/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#define RCAST reinterpret_cast<pksav_gen2_item_bag_t*>(_native)

#include "item_bag_gen2impl.hpp"
#include "item_list_gbimpl.hpp"
#include "item_list_gen2_tmhmimpl.hpp"

#include <pkmn/types/shared_ptr.hpp>

#include <cstring>

namespace pkmn {

    item_bag_gen2impl::item_bag_gen2impl(
        int game_id,
        void* ptr
    ): item_bag_impl(game_id)
    {
        if(ptr) {
            _native = ptr;
            _our_mem = false;
        } else {
            _native = reinterpret_cast<void*>(new pksav_gen2_item_bag_t);
            std::memset(_native, 0, sizeof(pksav_gen2_item_bag_t));
            _our_mem = true;
        }

        _set_ptrs();
    }

    item_bag_gen2impl::item_bag_gen2impl(
        int game_id,
        const pksav_gen2_item_bag_t &item_bag
    ): item_bag_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pksav_gen2_item_bag_t);
        *RCAST = item_bag;
        _our_mem = true;

        _set_ptrs();
    }

    item_bag_gen2impl::~item_bag_gen2impl() {
        if(_our_mem) {
            delete RCAST;
        }
    }

    void item_bag_gen2impl::_set_ptrs() {
        bool crystal = (_game_id == 6);

        int item_pocket_id     = crystal ? 10 : 5;
        int ball_pocket_id     = crystal ? 11 : 6;
        int key_item_pocket_id = crystal ? 12 : 7;
        int tmhm_pocket_id     = crystal ? 13 : 8;

        _item_pockets["Items"]    = pkmn::make_shared<item_list_gen2_item_pocketimpl>(
                                        item_pocket_id, _game_id, &RCAST->item_pocket
                                    );
        _item_pockets["Balls"]    = pkmn::make_shared<item_list_gen2_ball_pocketimpl>(
                                        ball_pocket_id, _game_id, &RCAST->ball_pocket
                                    );
        _item_pockets["KeyItems"] = pkmn::make_shared<item_list_gen2_key_item_pocketimpl>(
                                        key_item_pocket_id, _game_id, &RCAST->key_item_pocket
                                    );
        _item_pockets["TM/HM"]    = pkmn::make_shared<item_list_gen2_tmhmimpl>(
                                        tmhm_pocket_id, _game_id, &RCAST->tmhm_pocket
                                    );
    }
}
