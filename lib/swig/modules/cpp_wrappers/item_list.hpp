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

namespace pkmn { namespace swig {

    class item_slot_wrapper
    {
        public:
            item_slot_wrapper():
                _item_list(nullptr),
                _index(0)
            {}

            item_slot_wrapper(
                const pkmn::item_list::sptr& item_list,
                int index
            ): _item_list(item_list),
               _index(index)
            {
            }

            const std::string& get_item()
            {
                if(!_item_list)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                return _item_list->as_vector().at(_index).item;
            }

            void set_item(
                const std::string& item
            )
            {
                if(!_item_list)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                _item_list->set_item(
                    _index,
                    item,
                    (item == "None") ? 0
                                     : (get_amount() == 0) ? 1
                                                           : get_amount()
                );
            }

            int get_amount()
            {
                if(!_item_list)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                return _item_list->as_vector().at(_index).amount;
            }

            void set_amount(
                int amount
            )
            {
                if(!_item_list)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                _item_list->set_item(
                    _index,
                    (amount == 0) ? "None" : get_item(),
                    amount
                );
            }

        private:
            pkmn::item_list::sptr _item_list;
            int _index;
    };

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

            item_slot_wrapper at(
                int index
            )
            {
                return item_slot_wrapper(_item_list, index);
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

            const std::vector<std::string>& get_valid_items()
            {
                return _item_list->get_valid_items();
            }

        private:
            pkmn::item_list::sptr _item_list;
    };

}}

#endif /* CPP_WRAPPERS_ITEM_LIST_HPP */
