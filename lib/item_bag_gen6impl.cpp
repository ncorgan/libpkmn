/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#define NATIVE_RCAST (reinterpret_cast<pksav_gen6_bag_t*>(_native))

#include "item_bag_gen6impl.hpp"
#include "item_list_modernimpl.hpp"

#include "database/database_common.hpp"

#include <pkmn/types/shared_ptr.hpp>

#include <cstring>
#include <stdexcept>

namespace pkmn {

    item_bag_gen6impl::item_bag_gen6impl(
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
            _native = reinterpret_cast<void*>(new pksav_gen6_bag_t);
            std::memset(_native, 0, sizeof(pksav_gen6_bag_t));
            _our_mem = true;
        }

        _set_ptrs();
    }

    item_bag_gen6impl::~item_bag_gen6impl()
    {
        if(_our_mem)
        {
            delete NATIVE_RCAST;
        }
    }

    void item_bag_gen6impl::_set_ptrs()
    {
        bool is_xy = (_game_id == 23 or _game_id == 24);

        int item_pocket_id     = is_xy ? 81 : 86;
        int medicine_pocket_id = is_xy ? 82 : 87;
        int tms_hms_pocket_id  = is_xy ? 83 : 88;
        int berries_pocket_id  = is_xy ? 84 : 89;
        int key_item_pocket_id = is_xy ? 85 : 90;

        _item_pockets["Items"] = pkmn::make_shared<item_list_modernimpl>(
                                     item_pocket_id, _game_id, NATIVE_RCAST->items,
                                     sizeof(NATIVE_RCAST->items)/sizeof(pksav_item_t),
                                     false
                                 );
        _item_pockets["Medicine"] = pkmn::make_shared<item_list_modernimpl>(
                                        medicine_pocket_id, _game_id, NATIVE_RCAST->medicine,
                                        sizeof(NATIVE_RCAST->medicine)/sizeof(pksav_item_t),
                                        false
                                    );
        _item_pockets["TMs & HMs"] = pkmn::make_shared<item_list_modernimpl>(
                                         tms_hms_pocket_id, _game_id, NATIVE_RCAST->tms_hms,
                                         sizeof(NATIVE_RCAST->tms_hms)/sizeof(pksav_item_t),
                                         false
                                     );
        _item_pockets["Berries"] = pkmn::make_shared<item_list_modernimpl>(
                                        berries_pocket_id, _game_id, NATIVE_RCAST->berries,
                                        sizeof(NATIVE_RCAST->berries)/sizeof(pksav_item_t),
                                        false
                                    );
        _item_pockets["Key Items"] = pkmn::make_shared<item_list_modernimpl>(
                                         key_item_pocket_id, _game_id, NATIVE_RCAST->key_items,
                                         sizeof(NATIVE_RCAST->key_items)/sizeof(pksav_item_t),
                                         false
                                     );
    }
}
