/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_BAG_GBAIMPL_HPP
#define PKMN_ITEM_BAG_GBAIMPL_HPP

#include "item_bag_impl.hpp"

#include <pksav/gba/items.h>

namespace pkmn {

    class item_bag_gbaimpl: public item_bag_impl {
        public:
            item_bag_gbaimpl(
                int game_id,
                void* ptr
            );
            item_bag_gbaimpl(
                int game_id,
                const pksav_gba_item_storage_t &item_storage
            );

            ~item_bag_gbaimpl();

        private:
            void _set_ptrs();
    };

}

#endif /* PKMN_ITEM_BAG_GBAIMPL_HPP */
