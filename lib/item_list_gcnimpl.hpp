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

namespace pkmn {

    class item_list_gcnimpl: public item_list_impl {
        public:
            item_list_gcnimpl(
                int item_list_id,
                int game_id,
                LibPkmGC::Item* ptr,
                size_t capacity,
                bool copy
            );

            ~item_list_gcnimpl();

        private:
            void _from_native(
                int index = -1
            ) override final;
            void _to_native(
                int index = -1
            ) override final;
    };
}

#endif /* PKMN_ITEM_LIST_GCNIMPL_HPP */
