/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/item_list.hpp>
#include <pkmn/item_bag.hpp>

// Only create one main
#undef BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

static void _gen1_items_test(
    pkmn::item_list::sptr items
) {
    (void)items;
}

BOOST_AUTO_TEST_CASE(gen1_items_test) {
    pkmn::item_list::sptr items = pkmn::item_list::make("Items", "Red");
    _gen1_items_test(items);
}

BOOST_AUTO_TEST_CASE(gen1_pc_test) {
    pkmn::item_list::sptr pc = pkmn::item_list::make("PC", "Red");
}

BOOST_AUTO_TEST_CASE(gen1_item_bag_test) {
    pkmn::item_bag::sptr bag = pkmn::item_bag::make("Red");
    const pkmn::item_pockets_t& pockets = bag->get_pockets();
    _gen1_items_test(pockets.at("Items"));
}
