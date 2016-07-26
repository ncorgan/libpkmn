/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_LIST_IMPL_HPP
#define PKMN_ITEM_LIST_IMPL_HPP

#include <pkmn/item_list.hpp>

#include <boost/noncopyable.hpp>

namespace pkmn {

    class item_list_impl: public item_list, boost::noncopyable {
        public:
            item_list_impl() {};
            item_list_impl(
                int item_list_id,
                int game_id
            );

            ~item_list_impl() {};

            std::string get_name();

            std::string get_game();

            int get_capacity();

            int get_num_items();

            virtual const pkmn::item_slot_t& at(
                int position
            );

            const pkmn::item_slots_t& as_vector();

            void* get_native();

        protected:
            int _item_list_id, _game_id, _version_group_id;
            int _capacity, _num_items;

            pkmn::item_slots_t _item_slots;

            bool _our_mem;
            void* _native;

            virtual void _from_native(
                int index
            ) = 0;
            virtual void _to_native(
                int index
            ) = 0;
    };

}

#endif /* PKMN_ITEM_LIST_IMPL_HPP */
