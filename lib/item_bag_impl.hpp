/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_BAG_IMPL_HPP
#define PKMN_ITEM_BAG_IMPL_HPP

#include <pkmn/item_bag.hpp>

#include <boost/noncopyable.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace pkmn {

    class item_bag_impl: public item_bag,
                         private boost::noncopyable,
                         public boost::basic_lockable_adapter<boost::recursive_mutex>
    {
        public:
            item_bag_impl() {};
            explicit item_bag_impl(
                int game_id
            );

            virtual ~item_bag_impl() {};

            std::string get_game() override final;

            const pkmn::item_list::sptr& get_pocket(
                const std::string& name
            ) override final;

            const pkmn::item_pockets_t& get_pockets() override final;

            const std::vector<std::string>& get_pocket_names() override final;

            virtual void add(
                const std::string& item_name,
                int amount
            ) override final;

            virtual void remove(
                const std::string& item_name,
                int amount
            ) override final;

            void* get_native() override final;

        protected:
            int _game_id;

            pkmn::item_pockets_t _item_pockets;
            std::vector<std::string> _item_pocket_names;

            bool _our_mem;
            void* _native;

            virtual void _set_ptrs() = 0;
    };

}

#endif /* PKMN_ITEM_BAG_IMPL_HPP */
