/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
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

#include <boost/assert.hpp>

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
            explicit item_list(
                const pkmn::item_list::sptr& cpp_item_list
            ): _item_list(cpp_item_list)
            {
                BOOST_ASSERT(_item_list.get() != nullptr);
            }

            explicit item_list(
                const std::string& name,
                pkmn::e_game game
            ): _item_list(pkmn::item_list::make(name, game))
            {
                BOOST_ASSERT(_item_list.get() != nullptr);
            }

            inline std::string get_name()
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                return _item_list->get_name();
            }

            inline pkmn::e_game get_game()
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                return _item_list->get_game();
            }

            inline int get_capacity()
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                return _item_list->get_capacity();
            }

            inline int get_num_items()
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                return _item_list->get_num_items();
            }

            inline item_slot at(
                int position
            )
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Position",
                    position,
                    1,
                    get_capacity()
                );

                return item_slot(_item_list, position-1);
#else
                return item_slot(_item_list, position);
#endif
            }

            inline void add(
                pkmn::e_item item,
                int amount
            )
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                _item_list->add(item, amount);
            }

            inline void remove(
                pkmn::e_item item,
                int amount
            )
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                _item_list->remove(item, amount);
            }

            inline void move(
                int old_position,
                int new_position
            )
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Old position",
                    old_position,
                    1,
                    get_num_items()
                );
                pkmn::enforce_bounds(
                    "New position",
                    new_position,
                    1,
                    get_num_items()
                );

                _item_list->move(old_position-1, new_position-1);
#else
                _item_list->move(old_position, new_position);
#endif
            }

            // Copy the vector, since the const in the reference
            // is casted away.
            inline std::vector<pkmn::e_item> get_valid_items()
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                return _item_list->get_valid_items();
            }

            // Copy the vector, since the const in the reference
            // is casted away.
            inline std::vector<std::string> get_valid_item_names()
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                return _item_list->get_valid_item_names();
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                return uintmax_t(_item_list.get());
            }
#else
            inline bool operator==(const item_list& rhs) const
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                return (_item_list == rhs._item_list);
            }

            inline bool operator!=(const item_list& rhs) const
            {
                BOOST_ASSERT(_item_list.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            pkmn::item_list::sptr _item_list;
    };

}}

#endif /* CPP_WRAPPERS_ITEM_LIST_HPP */
