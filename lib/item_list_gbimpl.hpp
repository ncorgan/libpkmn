/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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

    template <typename pksav_type>
    class item_list_gbimpl: public item_list_impl
    {
        public:
            item_list_gbimpl(
                int item_list_id,
                int game_id,
                const pksav_type* p_pksav_list = nullptr
            );

            // TODO
            item_list_gbimpl(const item_list_gbimpl&) = delete;
            item_list_gbimpl(item_list_gbimpl&&) = delete;

            item_list_gbimpl& operator=(const item_list_gbimpl&) = delete;
            item_list_gbimpl& operator=(item_list_gbimpl&&) = delete;

            ~item_list_gbimpl() = default;

        private:
            void _pksav_item_to_libpkmn_item_slot(
                const struct pksav_gb_item& pksav_item,
                pkmn::item_slot& r_item_slot
            );
            void _libpkmn_item_slot_to_pksav_item(
                const pkmn::item_slot& item_slot,
                struct pksav_gb_item& r_pksav_item
            );

            void _from_native(int index = -1) final;
            void _to_native(int index = -1) final;

            pksav_type _pksav_list;
    };

    // Generation I
    typedef item_list_gbimpl<struct pksav_gen1_item_bag> item_list_gen1_bagimpl;
    typedef item_list_gbimpl<struct pksav_gen1_item_pc>  item_list_gen1_pcimpl;

    // Generation II
    typedef item_list_gbimpl<struct pksav_gen2_item_pocket> item_list_gen2_item_pocketimpl;
    typedef item_list_gbimpl<struct pksav_gen2_ball_pocket> item_list_gen2_ball_pocketimpl;
    typedef item_list_gbimpl<struct pksav_gen2_item_pc>     item_list_gen2_pcimpl;
}

#include "item_list_gbimpl.ipp"

#endif /* PKMN_ITEM_LIST_GBIMPL_HPP */
