/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_BAG_GEN2IMPL_HPP
#define PKMN_ITEM_BAG_GEN2IMPL_HPP

#include "item_bag_impl.hpp"

#include <pksav/gen2/items.h>

namespace pkmn {

    class item_bag_gen2impl: public item_bag_impl {
        public:
            item_bag_gen2impl(
                int game_id,
                void* ptr
            );
            item_bag_gen2impl(
                int game_id,
                const pksav_gen2_item_bag_t &item_bag
            );

            ~item_bag_gen2impl();

        private:
            void _set_ptrs();
    };

}

#endif /* PKMN_ITEM_BAG_GEN2IMPL_HPP */
