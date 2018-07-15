/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_LIST_GCNIMPL_HPP
#define PKMN_ITEM_LIST_GCNIMPL_HPP

#include "item_list_impl.hpp"

#include <LibPkmGC/Core/ItemInfo.h>

#include <vector>

namespace pkmn {

    class item_list_gcnimpl: public item_list_impl
    {
        public:
            item_list_gcnimpl(
                int item_list_id,
                int game_id,
                const LibPkmGC::Item* p_libpkmgc_list = nullptr
            );

            // TODO
            item_list_gcnimpl(const item_list_gcnimpl&) = delete;
            item_list_gcnimpl(item_list_gcnimpl&&) = delete;

            item_list_gcnimpl& operator=(const item_list_gcnimpl&) = delete;
            item_list_gcnimpl& operator=(item_list_gcnimpl&&) = delete;

            ~item_list_gcnimpl() = default;

        private:
            void _libpkmgc_item_to_libpkmn_item_slot(
                const LibPkmGC::Item& pksav_item,
                pkmn::item_slot& r_item_slot
            );
            void _libpkmn_item_slot_to_libpkmgc_item(
                const pkmn::item_slot& item_slot,
                LibPkmGC::Item& r_libpkmgc_item
            );

            void _from_native(int index = -1) final;
            void _to_native(int index = -1) final;

            std::vector<LibPkmGC::Item> _libpkmgc_list;
    };
}

#endif /* PKMN_ITEM_LIST_GCNIMPL_HPP */
