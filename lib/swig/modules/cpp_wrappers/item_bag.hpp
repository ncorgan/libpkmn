/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
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
            item_bag():
                _item_bag(nullptr)
            {}

            item_bag(
                const pkmn::item_bag::sptr& cpp_item_bag
            ): _item_bag(cpp_item_bag)
            {
            }

            item_bag(
                const std::string& game
            ): _item_bag(pkmn::item_bag::make(game))
            {
            }

            bool operator==(
                const item_bag& rhs
            ) const
            {
                return (_item_bag == rhs._item_bag);
            }

            std::string get_game()
            {
                return _item_bag->get_game();
            }

            pkmn::swig::item_list get_pocket(
                const std::string& name
            )
            {
                return pkmn::swig::item_list(_item_bag->get_pocket(name));
            }

            // Copy the vector, since the const in the reference
            // is casted away.
            std::vector<std::string> get_pocket_names()
            {
                return _item_bag->get_pocket_names();
            }

            void add(
                const std::string& item,
                int amount
            )
            {
                _item_bag->add(item, amount);
            }

            void remove(
                const std::string& item,
                int amount
            )
            {
                _item_bag->remove(item, amount);
            }

            // TODO: ifdef for specific wrappers

            // For hash code functions
            uintmax_t cptr()
            {
                return uintmax_t(_item_bag.get());
            }

        private:
            pkmn::item_bag::sptr _item_bag;
    };

}}

#endif /* CPP_WRAPPERS_ITEM_BAG_HPP */
