/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_LIST_MODERNIMPL_HPP
#define PKMN_ITEM_LIST_MODERNIMPL_HPP

#include "item_list_impl.hpp"

#include <pksav/common/item.h>

#include <vector>

namespace pkmn {

    class item_list_modernimpl: public item_list_impl
    {
        public:
            item_list_modernimpl(
                int item_list_id,
                int game_id,
                const struct pksav_item* p_pksav_list = nullptr
            );

            // TODO
            item_list_modernimpl(const item_list_modernimpl&) = delete;
            item_list_modernimpl(item_list_modernimpl&&) = delete;

            item_list_modernimpl& operator=(const item_list_modernimpl&) = delete;
            item_list_modernimpl& operator=(item_list_modernimpl&&) = delete;

            ~item_list_modernimpl() = default;

        private:
            void _pksav_item_to_libpkmn_item_slot(
                const struct pksav_item& pksav_item,
                pkmn::item_slot& r_item_slot
            );
            void _libpkmn_item_slot_to_pksav_item(
                const pkmn::item_slot& item_slot,
                struct pksav_item& r_pksav_item
            );

            void _from_native(int index = -1) final;
            void _to_native(int index = -1) final;

            std::vector<struct pksav_item> _pksav_list;
    };
}

#endif /* PKMN_ITEM_LIST_MODERNIMPL_HPP */
