/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_ITEM_BAG_HPP
#define CPP_WRAPPERS_ITEM_BAG_HPP

#include "item_bag.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/item_bag.hpp>

#include <boost/assert.hpp>

namespace pkmn { namespace swig {

    /*
     * This class is a thin wrapper around pkmn::item_bag::sptr and
     * will be what some SWIG wrappers will use instead of the class
     * itself. It will allow syntax like the following to be used:
     *
     * itemBag["Items"][0].item = "Potion"
     */
    class item_bag
    {
        public:
            explicit item_bag(
                const pkmn::item_bag::sptr& cpp_item_bag
            ): _item_bag(cpp_item_bag)
            {
                BOOST_ASSERT(_item_bag.get() != nullptr);
            }

            explicit item_bag(
                const std::string& game
            ): _item_bag(pkmn::item_bag::make(game))
            {
                BOOST_ASSERT(_item_bag.get() != nullptr);
            }

            inline std::string get_game()
            {
                BOOST_ASSERT(_item_bag.get() != nullptr);

                return _item_bag->get_game();
            }

            inline pkmn::swig::item_list get_pocket(
                const std::string& name
            )
            {
                BOOST_ASSERT(_item_bag.get() != nullptr);

                return pkmn::swig::item_list(_item_bag->get_pocket(name));
            }

            // Copy the vector, since the const in the reference
            // is casted away.
            inline std::vector<std::string> get_pocket_names()
            {
                BOOST_ASSERT(_item_bag.get() != nullptr);

                return _item_bag->get_pocket_names();
            }

            inline void add(
                const std::string& item,
                int amount
            )
            {
                BOOST_ASSERT(_item_bag.get() != nullptr);

                _item_bag->add(item, amount);
            }

            inline void remove(
                const std::string& item,
                int amount
            )
            {
                BOOST_ASSERT(_item_bag.get() != nullptr);

                _item_bag->remove(item, amount);
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_item_bag.get() != nullptr);

                return uintmax_t(_item_bag.get());
            }
#else
            inline bool operator==(const item_bag& rhs) const
            {
                BOOST_ASSERT(_item_bag.get() != nullptr);

                return (_item_bag == rhs._item_bag);
            }

            inline bool operator!=(const item_bag& rhs) const
            {
                BOOST_ASSERT(_item_bag.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            pkmn::item_bag::sptr _item_bag;
    };

}}

#endif /* CPP_WRAPPERS_ITEM_BAG_HPP */
