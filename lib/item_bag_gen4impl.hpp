/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_BAG_GEN4IMPL_HPP
#define PKMN_ITEM_BAG_GEN4IMPL_HPP

#include "item_bag_impl.hpp"

#include <pksav/gen4/items.h>

namespace pkmn {

    class item_bag_gen4impl: public item_bag_impl {
        public:
            item_bag_gen4impl(
                int game_id,
                void* ptr
            );

            ~item_bag_gen4impl();

        private:
            void _set_ptrs() override final;
    };

}

#endif /* PKMN_ITEM_BAG_GEN4IMPL_HPP */
