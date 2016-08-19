/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <gen1_items_tests.hpp>

// Only create one main
#undef BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

void gen1_item_list_test(
    pkmn::item_list::sptr items
) {
    (void)items;
}

void gen1_pc_test(
    pkmn::item_list::sptr pc
) {
    (void)pc;
}

void gen1_item_bag_test(
    pkmn::item_bag::sptr bag
) {
    (void)bag;
}
