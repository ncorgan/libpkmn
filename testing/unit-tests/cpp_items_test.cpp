/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen1_items_tests.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(gen1_item_list_test) {
    pkmn::item_list::sptr items = pkmn::item_list::make("Items", "Red");
    pkmntest::gen1_item_list_test(items);
}

BOOST_AUTO_TEST_CASE(gen1_pc_test) {
    pkmn::item_list::sptr pc = pkmn::item_list::make("PC", "Red");
    pkmntest::gen1_pc_test(pc);
}

BOOST_AUTO_TEST_CASE(gen1_item_bag_test) {
    pkmn::item_bag::sptr bag = pkmn::item_bag::make("Red");
    pkmntest::gen1_item_bag_test(bag);
}
