/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_BAG_GEN2IMPL_HPP
#define PKMN_ITEM_BAG_GEN2IMPL_HPP

#include "item_bag_impl.hpp"

#include <pksav/gen2/items.h>

namespace pkmn {

    class item_bag_gen2impl: public item_bag_impl
    {
        public:
            item_bag_gen2impl(
                int game_id,
                const struct pksav_gen2_item_bag* p_pksav_bag = nullptr
            );

            // TODO
            item_bag_gen2impl(const item_bag_gen2impl&) = delete;
            item_bag_gen2impl(item_bag_gen2impl&&) = delete;

            item_bag_gen2impl& operator=(const item_bag_gen2impl&) = delete;
            item_bag_gen2impl& operator=(item_bag_gen2impl&&) = delete;

            ~item_bag_gen2impl() = default;

        private:
            void _to_native() override;

            struct pksav_gen2_item_bag _pksav_bag;
    };

}

#endif /* PKMN_ITEM_BAG_GEN2IMPL_HPP */
