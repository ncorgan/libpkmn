/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_LIST_GBIMPL_HPP
#define PKMN_ITEM_LIST_GBIMPL_HPP

#include "item_list_impl.hpp"

namespace pkmn {

    template <typename list_type, typename item_type>
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
            );
            void _to_native(
                int index = -1
            );
    };

}

#include "item_list_gbimpl.ipp"

#endif /* PKMN_ITEM_LIST_GBIMPL_HPP */
