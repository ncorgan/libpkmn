/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_ITEM_SLOT_HPP
#define CPP_WRAPPERS_ITEM_SLOT_HPP

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/item_list.hpp>

namespace pkmn { namespace swig {

    class item_slot
    {
        public:
            item_slot():
                _item_list(nullptr),
                _index(0)
            {}

            item_slot(
                const pkmn::item_list::sptr& item_list,
                int index
            ): _item_list(item_list),
               _index(index)
            {
            }

            inline const std::string& get_item()
            {
                if(!_item_list)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                return _item_list->as_vector().at(_index).item;
            }

            inline void set_item(
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

            inline int get_amount()
            {
                if(!_item_list)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                return _item_list->as_vector().at(_index).amount;
            }

            inline void set_amount(
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

            inline bool operator==(const item_slot& rhs) const
            {
                return (this->_item_list == rhs._item_list) &&
                       (this->_index == rhs._index);
            }

            inline bool operator!=(const item_slot& rhs) const
            {
                return !operator==(rhs);
            }

        private:
            pkmn::item_list::sptr _item_list;
            int _index;
    };
}}

#endif /* PKMN_SWIG_ITEM_SLOT_HPP */
