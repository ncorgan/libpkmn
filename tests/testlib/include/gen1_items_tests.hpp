/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GEN1_ITEMS_TESTS_HPP
#define GEN1_ITEMS_TESTS_HPP

#include <pkmn/config.hpp>
#include <pkmn/item_bag.hpp>
#include <pkmn/item_list.hpp>

PKMN_API void gen1_item_list_test(
    pkmn::item_list::sptr items
);

PKMN_API void gen1_pc_test(
    pkmn::item_list::sptr pc
);

PKMN_API void gen1_item_bag_test(
    pkmn::item_bag::sptr bag
);

#endif /* GEN1_ITEMS_TESTS_HPP */
