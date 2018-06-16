/*
 * Copyright (c) 2015-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_ITEM_LIST_GEN2_TMHMIMPL_HPP
#define INCLUDED_PKMN_ITEM_LIST_GEN2_TMHMIMPL_HPP

#include "item_list_impl.hpp"

#include <pksav/gen2/items.h>

namespace pkmn {
    class item_list_gen2_tmhmimpl: public item_list_impl {
        public:
            item_list_gen2_tmhmimpl(
                int item_list_id,
                int game_id,
                const struct pksav_gen2_tmhm_pocket* p_pksav_list
            );

            // TODO
            item_list_gen2_tmhmimpl(const item_list_gen2_tmhmimpl&) = delete;
            item_list_gen2_tmhmimpl(item_list_gen2_tmhmimpl&&) = delete;

            item_list_gen2_tmhmimpl& operator=(const item_list_gen2_tmhmimpl&) = delete;
            item_list_gen2_tmhmimpl& operator=(item_list_gen2_tmhmimpl&&) = delete;

            ~item_list_gen2_tmhmimpl() = default;

            int get_num_items() override;

            void add(
                const std::string& name,
                int amount
            ) final;

            void remove(
                const std::string& name,
                int amount
            ) final;

            void move(
                int old_position,
                int new_position
            ) final;

            void set_item(
                int position,
                const std::string& item_name,
                int amount
            ) final;

        private:
            struct pksav_gen2_tmhm_pocket _pksav_list;

            void _from_native(
                int index = -1
            ) final;
            void _to_native(
                int index = -1
            ) final;
    };
}

#endif /* INCLUDED_PKMN_ITEM_LIST_GEN2_TMHMIMPL_HPP */
