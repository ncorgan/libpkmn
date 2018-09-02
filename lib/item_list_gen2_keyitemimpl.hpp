/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_ITEM_LIST_GEN2_KEYITEMIMPL_HPP
#define INCLUDED_PKMN_ITEM_LIST_GEN2_KEYITEMIMPL_HPP

#include "item_list_impl.hpp"

#include <pksav/gen2/items.h>

namespace pkmn {

    class item_list_gen2_keyitemimpl: public item_list_impl
    {
        public:
            item_list_gen2_keyitemimpl(
                int item_list_id,
                int game_id,
                const struct pksav_gen2_key_item_pocket* p_pksav_list = nullptr
            );

            // TODO
            item_list_gen2_keyitemimpl(const item_list_gen2_keyitemimpl&) = delete;
            item_list_gen2_keyitemimpl(item_list_gen2_keyitemimpl&&) = delete;

            item_list_gen2_keyitemimpl& operator=(const item_list_gen2_keyitemimpl&) = delete;
            item_list_gen2_keyitemimpl& operator=(item_list_gen2_keyitemimpl&&) = delete;

            ~item_list_gen2_keyitemimpl() = default;

            void add(
                pkmn::e_item item,
                int amount
            ) final;

            void remove(
                pkmn::e_item item,
                int amount
            ) final;

            void set_item(
                int position,
                pkmn::e_item item,
                int amount
            ) final;

        private:
            struct pksav_gen2_key_item_pocket _pksav_list;

            void _from_native(
                int index = -1
            ) final;
            void _to_native(
                int index = -1
            ) final;
    };
}

#endif /* INCLUDED_PKMN_ITEM_LIST_GEN2_KEYITEMIMPL_HPP */
