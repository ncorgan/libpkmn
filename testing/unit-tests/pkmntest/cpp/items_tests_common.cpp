/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "items_tests_common.hpp"

// Don't create the main in a library
#undef BOOST_TEST_MAIN
#include "pkmn_boost_unit_test.hpp"

namespace pkmntest {

    void test_item_list_empty_slots(
        pkmn::item_list::sptr list,
        const pkmn::database::item_entry &none_entry
    ) {
        const pkmn::item_slots_t& item_slots = list->as_vector();
        BOOST_CHECK_EQUAL(item_slots.size(), list->get_capacity());
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }
    }

    void test_item_list_out_of_range_error(
        pkmn::item_list::sptr list,
        const std::string &item_name
    ) {
        int num_items = list->get_num_items();

        BOOST_CHECK_THROW(
            list->add(item_name, 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            list->add(item_name, 100)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            list->remove(item_name, 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            list->remove(item_name, 100)
        , std::out_of_range);

        // This shouldn't have added any items
        BOOST_CHECK_EQUAL(list->get_num_items(), num_items);
    }

    void test_item_list_items_from_wrong_pocket(
        pkmn::item_list::sptr list,
        const std::vector<std::string> &item_names
    ) {
        int num_items = list->get_num_items();

        for(auto iter = item_names.begin(); iter != item_names.end(); ++iter) {
            BOOST_CHECK_THROW(
                list->add(*iter, 1)
            , std::invalid_argument);
        }

        // This shouldn't have added any items
        BOOST_CHECK_EQUAL(list->get_num_items(), num_items);
    }

    void test_item_list_add_remove(
        pkmn::item_list::sptr list,
        const pkmn::database::item_entry &none_entry,
        const std::vector<std::string> &item_names
    ) {
        BOOST_REQUIRE_EQUAL(item_names.size(), 8);
        const pkmn::item_slots_t& item_slots = list->as_vector();

        list->add(item_names[0], 30);
        list->add(item_names[1], 99);
        list->add(item_names[2], 1);

        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), item_names[0]);
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), item_names[1]);
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 99);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), item_names[2]);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 1);
        BOOST_CHECK_EQUAL(list->get_num_items(), 3);

        list->add(item_names[2], 15);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), item_names[0]);
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), item_names[1]);
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 99);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), item_names[2]);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 16);
        BOOST_CHECK_EQUAL(list->get_num_items(), 3);

        list->remove(item_names[1], 20);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), item_names[0]);
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), item_names[1]);
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), item_names[2]);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 16);
        BOOST_CHECK_EQUAL(list->get_num_items(), 3);

        list->move(0, 1);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), item_names[1]);
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), item_names[0]);
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), item_names[2]);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 16);
        BOOST_CHECK_EQUAL(list->get_num_items(), 3);

        list->remove(item_names[0], 30);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), item_names[1]);
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), item_names[2]);
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 16);
        BOOST_CHECK(item_slots.at(2).item == none_entry);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 0);
        BOOST_CHECK_EQUAL(list->get_num_items(), 2);

        list->add(item_names[3], 90);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), item_names[1]);
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), item_names[2]);
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 16);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), item_names[3]);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 90);
        BOOST_CHECK_EQUAL(list->get_num_items(), 3);

        list->add(item_names[4], 2);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), item_names[1]);
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), item_names[2]);
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 16);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), item_names[3]);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 90);
        BOOST_CHECK_EQUAL(item_slots.at(3).item.get_name(), item_names[4]);
        BOOST_CHECK_EQUAL(item_slots.at(3).amount, 2);
        BOOST_CHECK_EQUAL(list->get_num_items(), 4);

        list->remove(item_names[1], 30);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), item_names[1]);
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 49);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), item_names[2]);
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 16);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), item_names[3]);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 90);
        BOOST_CHECK_EQUAL(item_slots.at(3).item.get_name(), item_names[4]);
        BOOST_CHECK_EQUAL(item_slots.at(3).amount, 2);
        BOOST_CHECK_EQUAL(list->get_num_items(), 4);

        list->add(item_names[5], 12);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), item_names[1]);
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 49);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), item_names[2]);
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 16);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), item_names[3]);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 90);
        BOOST_CHECK_EQUAL(item_slots.at(3).item.get_name(), item_names[4]);
        BOOST_CHECK_EQUAL(item_slots.at(3).amount, 2);
        BOOST_CHECK_EQUAL(item_slots.at(4).item.get_name(), item_names[5]);
        BOOST_CHECK_EQUAL(item_slots.at(4).amount, 12);
        BOOST_CHECK_EQUAL(list->get_num_items(), 5);

        list->remove(item_names[2], 16);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), item_names[1]);
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 49);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), item_names[3]);
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 90);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), item_names[4]);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 2);
        BOOST_CHECK_EQUAL(item_slots.at(3).item.get_name(), item_names[5]);
        BOOST_CHECK_EQUAL(item_slots.at(3).amount, 12);
        BOOST_CHECK_EQUAL(list->get_num_items(), 4);

        list->add(item_names[6], 65);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), item_names[1]);
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 49);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), item_names[3]);
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 90);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), item_names[4]);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 2);
        BOOST_CHECK_EQUAL(item_slots.at(3).item.get_name(), item_names[5]);
        BOOST_CHECK_EQUAL(item_slots.at(3).amount, 12);
        BOOST_CHECK_EQUAL(item_slots.at(4).item.get_name(), item_names[6]);
        BOOST_CHECK_EQUAL(item_slots.at(4).amount, 65);
        BOOST_CHECK_EQUAL(list->get_num_items(), 5);

        list->add(item_names[7], 6);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), item_names[1]);
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 49);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), item_names[3]);
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 90);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), item_names[4]);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 2);
        BOOST_CHECK_EQUAL(item_slots.at(3).item.get_name(), item_names[5]);
        BOOST_CHECK_EQUAL(item_slots.at(3).amount, 12);
        BOOST_CHECK_EQUAL(item_slots.at(4).item.get_name(), item_names[6]);
        BOOST_CHECK_EQUAL(item_slots.at(4).amount, 65);
        BOOST_CHECK_EQUAL(item_slots.at(5).item.get_name(), item_names[7]);
        BOOST_CHECK_EQUAL(item_slots.at(5).amount, 6);
        BOOST_CHECK_EQUAL(list->get_num_items(), 6);

        // Remove everything
        list->remove(item_names[4], 2);
        list->remove(item_names[1], 49);
        list->remove(item_names[7], 6);
        list->remove(item_names[5], 12);
        list->remove(item_names[3], 90);
        list->remove(item_names[6], 65);
        BOOST_CHECK_EQUAL(list->get_num_items(), 0);
    }
}
