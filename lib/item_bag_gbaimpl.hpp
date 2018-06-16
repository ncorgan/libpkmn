/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_BAG_GBAIMPL_HPP
#define PKMN_ITEM_BAG_GBAIMPL_HPP

#include "item_bag_impl.hpp"

#include <pksav/gba/items.h>

namespace pkmn {

    class item_bag_gbaimpl: public item_bag_impl
    {
        public:
            item_bag_gbaimpl(
                int game_id,
                const union pksav_gba_item_bag* p_pksav_bag
            );

            // TODO
            item_bag_gbaimpl(const item_bag_gbaimpl&) = delete;
            item_bag_gbaimpl(item_bag_gbaimpl&&) = delete;

            item_bag_gbaimpl& operator=(const item_bag_gbaimpl&) = delete;
            item_bag_gbaimpl& operator=(item_bag_gbaimpl&&) = delete;

            ~item_bag_gbaimpl() = default;

        private:
            union pksav_gba_item_bag _pksav_bag;
    };

}

#endif /* PKMN_ITEM_BAG_GBAIMPL_HPP */
