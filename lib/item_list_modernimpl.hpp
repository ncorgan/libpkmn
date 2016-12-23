/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_LIST_MODERNIMPL_HPP
#define PKMN_ITEM_LIST_MODERNIMPL_HPP

#include "item_list_impl.hpp"

#include <pksav/common/item.h>

namespace pkmn {

    class item_list_modernimpl: public item_list_impl {
        public:
            item_list_modernimpl(
                int item_list_id,
                int game_id,
                pksav_item_t* ptr,
                size_t capacity,
                bool copy
            );

            ~item_list_modernimpl();

        private:
            void _from_native(
                int index = -1
            );
            void _to_native(
                int index = -1
            );
    };
}

#endif /* PKMN_ITEM_LIST_MODERNIMPL_HPP */
