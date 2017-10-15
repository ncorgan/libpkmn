/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_LIST_GBIMPL_HPP
#define PKMN_ITEM_LIST_GBIMPL_HPP

#include "item_list_impl.hpp"

#include <pksav/gen1/items.h>
#include <pksav/gen2/items.h>

namespace pkmn {

    template <typename list_type>
    class item_list_gbimpl: public item_list_impl {
        public:
            item_list_gbimpl(
                int item_list_id,
                int game_id,
                void* ptr
            );
            item_list_gbimpl(
                int item_list_id,
                int game_id,
                const list_type &list
            );

            ~item_list_gbimpl();

        private:
            void _from_native(
                int index = -1
            ) override final;
            void _to_native(
                int index = -1
            ) override final;
    };

    // Generation I
    typedef item_list_gbimpl<pksav_gen1_item_bag_t> item_list_gen1_bagimpl;
    typedef item_list_gbimpl<pksav_gen1_item_pc_t>  item_list_gen1_pcimpl;

    // Generation II
    typedef item_list_gbimpl<pksav_gen2_item_pocket_t> item_list_gen2_item_pocketimpl;
    typedef item_list_gbimpl<pksav_gen2_key_item_pocket_t> item_list_gen2_key_item_pocketimpl;
    typedef item_list_gbimpl<pksav_gen2_ball_pocket_t> item_list_gen2_ball_pocketimpl;
    typedef item_list_gbimpl<pksav_gen2_item_pc_t>  item_list_gen2_pcimpl;
}

#include "item_list_gbimpl.ipp"

#endif /* PKMN_ITEM_LIST_GBIMPL_HPP */
