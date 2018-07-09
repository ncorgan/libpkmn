/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_BAG_IMPL_HPP
#define PKMN_ITEM_BAG_IMPL_HPP

#include <pkmn/item_bag.hpp>

#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace pkmn {

    class item_bag_impl: public item_bag,
                         public boost::basic_lockable_adapter<boost::recursive_mutex>
    {
        public:
            item_bag_impl() {};
            explicit item_bag_impl(
                int game_id
            );

            virtual ~item_bag_impl() {};

            std::string get_game() final;

            const pkmn::item_list::sptr& get_pocket(
                const std::string& name
            ) final;

            const pkmn::item_pockets_t& get_pockets() final;

            const std::vector<std::string>& get_pocket_names() final;

            virtual void add(
                const std::string& item_name,
                int amount
            ) final;

            virtual void remove(
                const std::string& item_name,
                int amount
            ) final;

            void* get_native() final;

        protected:
            int _game_id;

            pkmn::item_pockets_t _item_pockets;
            std::vector<std::string> _item_pocket_names;

            virtual void _to_native() = 0;

            void* _p_native;
    };

}

#endif /* PKMN_ITEM_BAG_IMPL_HPP */
