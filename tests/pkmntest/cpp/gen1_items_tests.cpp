/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen1_items_tests.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

namespace pkmntest {

    static void gen1_item_list_common(
        pkmn::item_list::sptr list
    ) {
        /*
         * Confirm exceptions are thrown when expected.
         */
        BOOST_CHECK_THROW(
            list->add("Potion", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            list->add("Potion", 100)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            list->remove("Potion", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            list->remove("Potion", 100)
        , std::out_of_range);

        /*
         * Start adding and removing stuff, and make sure
         * the numbers are accurate.
         */
        const pkmn::item_slots_t& item_slots = list->as_vector();
        list->add("Potion", 30);
        list->add("Great Ball", 99);
        list->add("Ether", 1);

        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Potion");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 99);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "Ether");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 1);
        BOOST_CHECK_EQUAL(list->get_num_items(), 3);

        list->remove("Great Ball", 20);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 79);
        BOOST_CHECK_EQUAL(list->get_num_items(), 3);

        list->move(0, 1);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Potion");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 30);
        BOOST_CHECK_EQUAL(list->get_num_items(), 3);

        list->remove("Potion", 30);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Ether");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 1);
        BOOST_CHECK_EQUAL(list->get_num_items(), 2);

        const std::vector<std::string>& valid_items = list->get_valid_items();
        BOOST_CHECK_GT(valid_items.size(), 0);
    }

    void gen1_item_list_test(
        pkmn::item_list::sptr items
    ) {
        /*
         * Check unchanging and initial values
         */
        BOOST_CHECK_EQUAL(items->get_name(), "Items");
        BOOST_CHECK_EQUAL(items->get_game(), "Red");
        BOOST_CHECK_EQUAL(items->get_capacity(), 20);
        BOOST_CHECK_EQUAL(items->get_num_items(), 0);

        gen1_item_list_common(items);
    }

    void gen1_pc_test(
        pkmn::item_list::sptr pc
    ) {
        /*
         * Check unchanging and initial values
         */
        BOOST_CHECK_EQUAL(pc->get_name(), "PC");
        BOOST_CHECK_EQUAL(pc->get_game(), "Red");
        BOOST_CHECK_EQUAL(pc->get_capacity(), 50);
        BOOST_CHECK_EQUAL(pc->get_num_items(), 0);

        gen1_item_list_common(pc);
    }

    void gen1_item_bag_test(
        pkmn::item_bag::sptr bag
    ) {
        const pkmn::item_pockets_t& pockets = bag->get_pockets();
        BOOST_CHECK_EQUAL(pockets.size(), 1);
        gen1_item_list_test(pockets.at("Items"));
    }

}