/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_bag_gen1impl.hpp"
#include "item_list_gbimpl.hpp"

#include "utils/misc.hpp"

#include <cstring>

namespace pkmn {

    item_bag_gen1impl::item_bag_gen1impl(
        int game_id,
        const struct pksav_gen1_item_bag* p_pksav_bag
    ): item_bag_impl(game_id)
    {
        if(p_pksav_bag != nullptr)
        {
            _pksav_bag = *p_pksav_bag;
        }
        else
        {
            std::memset(&_pksav_bag, 0, sizeof(_pksav_bag));
            _pksav_bag.terminator = 0xFF;
        }

        _p_native = &_pksav_bag;

        BOOST_STATIC_CONSTEXPR int YELLOW_GAME_ID = 3;
        BOOST_STATIC_CONSTEXPR int RB_ITEM_POCKET_ID = 1;
        BOOST_STATIC_CONSTEXPR int YELLOW_ITEM_POCKET_ID = 3;

        _item_pockets["Items"] = std::make_shared<item_list_gen1_bagimpl>(
                                     (_game_id == YELLOW_GAME_ID)
                                         ? YELLOW_ITEM_POCKET_ID
                                         : RB_ITEM_POCKET_ID,
                                     _game_id, &_pksav_bag
                                 );
    }

    void item_bag_gen1impl::_to_native()
    {
        pkmn::rcast_equal<struct pksav_gen1_item_bag>(
            _item_pockets["Items"]->get_native(),
            &_pksav_bag
        );
    }
}
