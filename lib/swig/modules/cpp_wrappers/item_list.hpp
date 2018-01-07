/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_ITEM_LIST_HPP
#define CPP_WRAPPERS_ITEM_LIST_HPP

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/item_list.hpp>

#include "item_slot.hpp"

namespace pkmn { namespace swig {

    /*
     * This class is a thin wrapper around pkmn::item_list::sptr and
     * will be what some SWIG wrappers will use instead of the class
     * itself. It will allow syntax like the following to be used:
     *
     * itemPocket[0].item = "Potion"
     */
    class item_list
    {
        public:
            item_list():
                _item_list(nullptr)
            {}

            item_list(
                const pkmn::item_list::sptr& cpp_item_list
            ): _item_list(cpp_item_list)
            {
            }

            item_list(
                const std::string& name,
                const std::string& game
            ): _item_list(pkmn::item_list::make(name, game))
            {
            }

            bool operator==(
                const item_list& rhs
            ) const
            {
                return (_item_list == rhs._item_list);
            }

            std::string get_name()
            {
                return _item_list->get_name();
            }

            std::string get_game()
            {
                return _item_list->get_game();
            }

            int get_capacity()
            {
                return _item_list->get_capacity();
            }

            int get_num_items()
            {
                return _item_list->get_num_items();
            }

            item_slot at(
                int index
            )
            {
                return item_slot(_item_list, index);
            }

            void add(
                const std::string& item,
                int amount
            )
            {
                _item_list->add(item, amount);
            }

            void remove(
                const std::string& item,
                int amount
            )
            {
                _item_list->remove(item, amount);
            }

            void move(
                int old_position,
                int new_position
            )
            {
                _item_list->move(old_position, new_position);
            }

            // Copy the vector, since the const in the reference
            // is casted away.
            std::vector<std::string> get_valid_items()
            {
                return _item_list->get_valid_items();
            }

            // TODO: ifdef for specific wrappers

            // For hash code functions
            uintmax_t cptr()
            {
                return uintmax_t(_item_list.get());
            }

        private:
            pkmn::item_list::sptr _item_list;
    };

}}

#endif /* CPP_WRAPPERS_ITEM_LIST_HPP */
