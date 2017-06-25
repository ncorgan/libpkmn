/*
 * Copyright (c) 2015-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_ITEM_LIST_GEN2_TMHMIMPL_HPP
#define INCLUDED_PKMN_ITEM_LIST_GEN2_TMHMIMPL_HPP

#include "item_list_impl.hpp"

namespace pkmn {
    class item_list_gen2_tmhmimpl: public item_list_impl {
        public:
            item_list_gen2_tmhmimpl(
                int item_list_id,
                int game_id,
                void* ptr
            );
            ~item_list_gen2_tmhmimpl();

            int get_num_items();

            void add(
                const std::string &name,
                int amount
            );

            void remove(
                const std::string &name,
                int amount
            );

            void move(
                int old_position,
                int new_position
            );

            void set_item(
                int position,
                const pkmn::item_slot& slot
            );

        private:
            void _from_native(
                int index = -1
            );
            void _to_native(
                int index = -1
            );
    };
}

#endif /* INCLUDED_PKMN_ITEM_LIST_GEN2_TMHMIMPL_HPP */
