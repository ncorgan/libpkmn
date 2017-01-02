/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_BAG_IMPL_HPP
#define PKMN_ITEM_BAG_IMPL_HPP

#include "mem/scoped_lock.hpp"

#include <pkmn/item_bag.hpp>

#include <boost/noncopyable.hpp>

namespace pkmn {

    class item_bag_impl;
    typedef pkmn::mem::scoped_lock<item_bag_impl> item_bag_scoped_lock;

    class item_bag_impl: public item_bag, boost::noncopyable {
        public:
            item_bag_impl() {};
            item_bag_impl(
                int game_id
            );

            virtual ~item_bag_impl() {};

            std::string get_game();

            pkmn::item_list::sptr get_pocket(
                const std::string &name
            );

            const pkmn::item_pockets_t& get_pockets();

            const std::vector<std::string>& get_pocket_names();

            virtual void add(
                const std::string &item_name,
                int amount
            );

            virtual void remove(
                const std::string &item_name,
                int amount
            );

            void* get_native();

            friend item_bag_scoped_lock;

        protected:
            int _game_id;

            pkmn::item_pockets_t _item_pockets;
            std::vector<std::string> _item_pocket_names;

            bool _our_mem;
            void* _native;
            boost::mutex _mem_mutex;

            virtual void _set_ptrs() = 0;
    };

}

#endif /* PKMN_ITEM_BAG_IMPL_HPP */
