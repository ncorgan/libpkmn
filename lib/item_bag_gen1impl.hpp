/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_BAG_GEN1IMPL_HPP
#define PKMN_ITEM_BAG_GEN1IMPL_HPP

#include "item_bag_impl.hpp"

#include <pksav/gen1/items.h>

namespace pkmn {

    class item_bag_gen1impl: public item_bag_impl
    {
        public:
            item_bag_gen1impl(
                int game_id,
                void* p_native
            );

            ~item_bag_gen1impl();

        private:
            void _set_ptrs() override final;
    };

}

#endif /* PKMN_ITEM_BAG_GEN1IMPL_HPP */
