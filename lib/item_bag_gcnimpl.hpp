/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_BAG_GCNIMPL_HPP
#define PKMN_ITEM_BAG_GCNIMPL_HPP

#include "item_bag_impl.hpp"

#include "libpkmgc_includes.hpp"

#include <memory>

namespace pkmn {

    class item_bag_gcnimpl: public item_bag_impl
    {
        public:
            item_bag_gcnimpl(
                int game_id,
                const LibPkmGC::GC::BagData* p_libpkmgc_bag
            );

            // TODO
            item_bag_gcnimpl(const item_bag_gcnimpl&) = delete;
            item_bag_gcnimpl(item_bag_gcnimpl&&) = delete;

            item_bag_gcnimpl& operator=(const item_bag_gcnimpl&) = delete;
            item_bag_gcnimpl& operator=(item_bag_gcnimpl&&) = delete;

            ~item_bag_gcnimpl() = default;

        private:
            void _to_native();

            std::unique_ptr<LibPkmGC::GC::BagData> _libpkmgc_bag_uptr;
    };

}

#endif /* PKMN_ITEM_BAG_GCNIMPL_HPP */
