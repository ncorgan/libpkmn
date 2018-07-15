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
                const struct pksav_gen1_item_bag* p_pksav_bag = nullptr
            );

            // TODO
            item_bag_gen1impl(const item_bag_gen1impl&) = delete;
            item_bag_gen1impl(item_bag_gen1impl&&) = delete;

            item_bag_gen1impl& operator=(const item_bag_gen1impl&) = delete;
            item_bag_gen1impl& operator=(item_bag_gen1impl&&) = delete;

            ~item_bag_gen1impl() = default;

        private:
            void _to_native() override;

            struct pksav_gen1_item_bag _pksav_bag;
    };

}

#endif /* PKMN_ITEM_BAG_GEN1IMPL_HPP */
