/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_BAG_GEN6IMPL_HPP
#define PKMN_ITEM_BAG_GEN6IMPL_HPP

#include "item_bag_impl.hpp"

#include <pksav/gen6/items.h>

namespace pkmn {

    class item_bag_gen6impl: public item_bag_impl {
        public:
            item_bag_gen6impl(
                int game_id,
                void* ptr
            );
            item_bag_gen6impl(
                int game_id,
                const pksav_gen6_bag_t& bag
            );

            ~item_bag_gen6impl();

        private:
            void _set_ptrs() override final;
    };

}

#endif /* PKMN_ITEM_BAG_GEN6IMPL_HPP */
