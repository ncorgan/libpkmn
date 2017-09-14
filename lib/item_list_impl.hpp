/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_LIST_IMPL_HPP
#define PKMN_ITEM_LIST_IMPL_HPP

#include "mem/scoped_lock.hpp"

#include <pkmn/item_list.hpp>

#include <boost/noncopyable.hpp>

namespace pkmn {

    class item_list_impl;
    typedef pkmn::mem::scoped_lock<item_list_impl> item_list_scoped_lock;

    class item_list_impl: public item_list, boost::noncopyable {
        public:
            item_list_impl() {};
            item_list_impl(
                int item_list_id,
                int game_id
            );

            virtual ~item_list_impl() {};

            std::string get_name();

            std::string get_game();

            int get_capacity();

            int get_num_items();

            virtual const pkmn::item_slot& at(
                int position
            );

            virtual void add(
                const std::string &item_name,
                int amount
            );

            virtual void remove(
                const std::string &item_name,
                int amount
            );

            virtual void move(
                int old_position,
                int new_position
            );

            virtual void set_item(
                int position,
                const std::string& item_name,
                int amount
            );

            const pkmn::item_slots_t& as_vector();

            const std::vector<std::string>& get_valid_items();

            void* get_native();

            friend item_list_scoped_lock;

        protected:
            int _item_list_id, _game_id, _version_group_id;
            int _capacity, _num_items;
            bool _pc;

            pkmn::item_slots_t _item_slots;
            std::vector<std::string> _valid_items;

            bool _our_mem;
            void* _native;
            boost::mutex _mem_mutex;

            virtual void _from_native(
                int index = -1
            ) = 0;
            virtual void _to_native(
                int index = -1
            ) = 0;
    };
}

#endif /* PKMN_ITEM_LIST_IMPL_HPP */
