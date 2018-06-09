/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_ITEM_LIST_GEN2_KEYITEMIMPL_HPP
#define INCLUDED_PKMN_ITEM_LIST_GEN2_KEYITEMIMPL_HPP

#include "item_list_impl.hpp"

namespace pkmn {
    class item_list_gen2_keyitemimpl: public item_list_impl
    {
        public:
            item_list_gen2_keyitemimpl(
                int item_list_id,
                int game_id,
                void* ptr
            );
            ~item_list_gen2_keyitemimpl();

            void add(
                const std::string& item_name,
                int amount
            ) override final;

            void remove(
                const std::string& item_name,
                int amount
            ) override final;

            void set_item(
                int position,
                const std::string& item_name,
                int amount
            ) override final;

        private:
            void _from_p_native(
                int index = -1
            ) override final;
            void _to_native(
                int index = -1
            ) override final;
    };
}

#endif /* INCLUDED_PKMN_ITEM_LIST_GEN2_KEYITEMIMPL_HPP */
