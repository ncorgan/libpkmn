/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
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
            explicit item_slot(
                const pkmn::item_list::sptr& item_list,
                int index
            ): _item_list(item_list),
               _index(index)
            {
                BOOST_ASSERT(_item_list.get() != nullptr);
            }

            inline pkmn::e_item get_item()
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                return _item_list->as_vector().at(_index).item;
            }

            inline void set_item(
                pkmn::e_item item
            )
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                _item_list->set_item(
                    _index,
                    item,
                    (item == pkmn::e_item::NONE) ? 0
                                                 : (get_amount() == 0) ? 1
                                                                       : get_amount()
                );
            }

            inline int get_amount()
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                return _item_list->as_vector().at(_index).amount;
            }

            inline void set_amount(
                int amount
            )
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                _item_list->set_item(
                    _index,
                    (amount == 0) ? pkmn::e_item::NONE : get_item(),
                    amount
                );
            }

            int index()
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                return _index;
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                return uintmax_t(_item_list.get());
            }
#else
            inline bool operator==(const item_slot& rhs) const
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                return (_item_list == rhs._item_list) and
                       (_index == rhs._index);
            }

            inline bool operator!=(const item_slot& rhs) const
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            pkmn::item_list::sptr _item_list;
            int _index;
    };
}}

#endif /* PKMN_SWIG_ITEM_SLOT_HPP */
