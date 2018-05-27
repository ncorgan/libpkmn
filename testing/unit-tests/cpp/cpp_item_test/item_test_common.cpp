/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_test_common.hpp"

void test_item_list_empty_slots(
    pkmn::item_list::sptr list
) {
    ASSERT_EQ(0, list->get_num_items());

    std::string game = list->get_game();
    const pkmn::item_slots_t& item_slots = list->as_vector();
    ASSERT_EQ(list->get_capacity(), item_slots.size());
    for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
        EXPECT_EQ("None", iter->item);
        EXPECT_EQ(0, iter->amount);
    }
}

void test_item_list_out_of_range_error(
    pkmn::item_list::sptr list,
    const std::string& item_name
) {
    int num_items = list->get_num_items();

    EXPECT_THROW(
        list->add(item_name, 0)
    , std::out_of_range);
    EXPECT_THROW(
        list->add(item_name, 100)
    , std::out_of_range);
    EXPECT_THROW(
        list->remove(item_name, 0)
    , std::out_of_range);
    EXPECT_THROW(
        list->remove(item_name, 100)
    , std::out_of_range);

    // This shouldn't have added any items
    EXPECT_EQ(num_items, list->get_num_items());
}

void test_item_bag_invalid_items(
    pkmn::item_bag::sptr bag,
    const std::vector<std::string> &item_names
) {
    for(auto iter = item_names.begin(); iter != item_names.end(); ++iter) {
        EXPECT_THROW(
            bag->add(*iter, 1)
        , std::invalid_argument);
    }
}

void test_item_list_invalid_items(
    pkmn::item_list::sptr list,
    const std::vector<std::string> &item_names
) {
    int num_items = list->get_num_items();

    for(auto iter = item_names.begin(); iter != item_names.end(); ++iter) {
        EXPECT_THROW(
            list->add(*iter, 1)
        , std::invalid_argument);
    }

    // This shouldn't have added any items
    EXPECT_EQ(num_items, list->get_num_items());
}

void test_item_list_add_remove(
    pkmn::item_list::sptr list,
    const std::vector<std::string> &item_names
) {
    ASSERT_EQ(0, list->get_num_items());
    ASSERT_EQ(8, item_names.size());
    const pkmn::item_slots_t& item_slots = list->as_vector();

    // Test setting by index.
    list->set_item(0, item_names[0], 50);
    list->set_item(1, item_names[1], 40);
    list->set_item(2, item_names[2], 30);
    EXPECT_EQ(item_names[0], item_slots.at(0).item);
    EXPECT_EQ(50, item_slots.at(0).amount);
    EXPECT_EQ(item_names[1], item_slots.at(1).item);
    EXPECT_EQ(40, item_slots.at(1).amount);
    EXPECT_EQ(item_names[2], item_slots.at(2).item);
    EXPECT_EQ(30, item_slots.at(2).amount);
    EXPECT_EQ(3, list->get_num_items());

    EXPECT_THROW(
        list->set_item(10, "Ultra Ball", 5)
    , std::out_of_range);
    EXPECT_THROW(
        list->set_item(0, "None", 5)
    , std::invalid_argument);
    EXPECT_THROW(
        list->set_item(1, item_names[0], 1)
    , std::invalid_argument);
    EXPECT_EQ(item_names[0], item_slots.at(0).item);
    EXPECT_EQ(50, item_slots.at(0).amount);
    EXPECT_EQ(item_names[1], item_slots.at(1).item);
    EXPECT_EQ(40, item_slots.at(1).amount);
    EXPECT_EQ(item_names[2], item_slots.at(2).item);
    EXPECT_EQ(30, item_slots.at(2).amount);
    EXPECT_EQ(3, list->get_num_items());

    list->set_item(1, "None", 0);
    EXPECT_EQ(item_names[0], item_slots.at(0).item);
    EXPECT_EQ(50, item_slots.at(0).amount);
    EXPECT_EQ(item_names[2], item_slots.at(1).item);
    EXPECT_EQ(30, item_slots.at(1).amount);
    EXPECT_EQ("None", item_slots.at(2).item);
    EXPECT_EQ(0, item_slots.at(2).amount);
    EXPECT_EQ(2, list->get_num_items());

    list->set_item(0, "None", 0);
    list->set_item(0, "None", 0);
    EXPECT_EQ("None", item_slots.at(0).item);
    EXPECT_EQ(0, item_slots.at(0).amount);
    EXPECT_EQ("None", item_slots.at(1).item);
    EXPECT_EQ(0, item_slots.at(1).amount);
    EXPECT_EQ("None", item_slots.at(2).item);
    EXPECT_EQ(0, item_slots.at(2).amount);
    EXPECT_EQ(0, list->get_num_items());

    // Test add/remove functions.
    list->add(item_names[0], 30);
    list->add(item_names[1], 99);
    list->add(item_names[2], 1);

    EXPECT_EQ(item_names[0], item_slots.at(0).item);
    EXPECT_EQ(30, item_slots.at(0).amount);
    EXPECT_EQ(item_names[1], item_slots.at(1).item);
    EXPECT_EQ(99, item_slots.at(1).amount);
    EXPECT_EQ(item_names[2], item_slots.at(2).item);
    EXPECT_EQ(1, item_slots.at(2).amount);
    EXPECT_EQ(3, list->get_num_items());

    list->add(item_names[2], 15);
    EXPECT_EQ(item_names[0], item_slots.at(0).item);
    EXPECT_EQ(30, item_slots.at(0).amount);
    EXPECT_EQ(item_names[1], item_slots.at(1).item);
    EXPECT_EQ(99, item_slots.at(1).amount);
    EXPECT_EQ(item_names[2], item_slots.at(2).item);
    EXPECT_EQ(16, item_slots.at(2).amount);
    EXPECT_EQ(3, list->get_num_items());

    list->remove(item_names[1], 20);
    EXPECT_EQ(item_names[0], item_slots.at(0).item);
    EXPECT_EQ(30, item_slots.at(0).amount);
    EXPECT_EQ(item_names[1], item_slots.at(1).item);
    EXPECT_EQ(79, item_slots.at(1).amount);
    EXPECT_EQ(item_names[2], item_slots.at(2).item);
    EXPECT_EQ(16, item_slots.at(2).amount);
    EXPECT_EQ(3, list->get_num_items());

    list->move(0, 1);
    EXPECT_EQ(item_names[1], item_slots.at(0).item);
    EXPECT_EQ(79, item_slots.at(0).amount);
    EXPECT_EQ(item_names[0], item_slots.at(1).item);
    EXPECT_EQ(30, item_slots.at(1).amount);
    EXPECT_EQ(item_names[2], item_slots.at(2).item);
    EXPECT_EQ(16, item_slots.at(2).amount);
    EXPECT_EQ(3, list->get_num_items());

    list->remove(item_names[0], 30);
    EXPECT_EQ(item_names[1], item_slots.at(0).item);
    EXPECT_EQ(79, item_slots.at(0).amount);
    EXPECT_EQ(item_names[2], item_slots.at(1).item);
    EXPECT_EQ(16, item_slots.at(1).amount);
    EXPECT_EQ("None", item_slots.at(2).item);
    EXPECT_EQ(0, item_slots.at(2).amount);
    EXPECT_EQ(2, list->get_num_items());

    list->add(item_names[3], 90);
    EXPECT_EQ(item_names[1], item_slots.at(0).item);
    EXPECT_EQ(79, item_slots.at(0).amount);
    EXPECT_EQ(item_names[2], item_slots.at(1).item);
    EXPECT_EQ(16, item_slots.at(1).amount);
    EXPECT_EQ(item_names[3], item_slots.at(2).item);
    EXPECT_EQ(90, item_slots.at(2).amount);
    EXPECT_EQ(3, list->get_num_items());

    list->add(item_names[4], 2);
    EXPECT_EQ(item_names[1], item_slots.at(0).item);
    EXPECT_EQ(79, item_slots.at(0).amount);
    EXPECT_EQ(item_names[2], item_slots.at(1).item);
    EXPECT_EQ(16, item_slots.at(1).amount);
    EXPECT_EQ(item_names[3], item_slots.at(2).item);
    EXPECT_EQ(90, item_slots.at(2).amount);
    EXPECT_EQ(item_names[4], item_slots.at(3).item);
    EXPECT_EQ(2, item_slots.at(3).amount);
    EXPECT_EQ(4, list->get_num_items());

    list->remove(item_names[1], 30);
    EXPECT_EQ(item_names[1], item_slots.at(0).item);
    EXPECT_EQ(49, item_slots.at(0).amount);
    EXPECT_EQ(item_names[2], item_slots.at(1).item);
    EXPECT_EQ(16, item_slots.at(1).amount);
    EXPECT_EQ(item_names[3], item_slots.at(2).item);
    EXPECT_EQ(90, item_slots.at(2).amount);
    EXPECT_EQ(item_names[4], item_slots.at(3).item);
    EXPECT_EQ(2, item_slots.at(3).amount);
    EXPECT_EQ(4, list->get_num_items());

    list->add(item_names[5], 12);
    EXPECT_EQ(item_names[1], item_slots.at(0).item);
    EXPECT_EQ(49, item_slots.at(0).amount);
    EXPECT_EQ(item_names[2], item_slots.at(1).item);
    EXPECT_EQ(16, item_slots.at(1).amount);
    EXPECT_EQ(item_names[3], item_slots.at(2).item);
    EXPECT_EQ(90, item_slots.at(2).amount);
    EXPECT_EQ(item_names[4], item_slots.at(3).item);
    EXPECT_EQ(2, item_slots.at(3).amount);
    EXPECT_EQ(item_names[5], item_slots.at(4).item);
    EXPECT_EQ(12, item_slots.at(4).amount);
    EXPECT_EQ(5, list->get_num_items());

    list->remove(item_names[2], 16);
    EXPECT_EQ(item_names[1], item_slots.at(0).item);
    EXPECT_EQ(49, item_slots.at(0).amount);
    EXPECT_EQ(item_names[3], item_slots.at(1).item);
    EXPECT_EQ(90, item_slots.at(1).amount);
    EXPECT_EQ(item_names[4], item_slots.at(2).item);
    EXPECT_EQ(2, item_slots.at(2).amount);
    EXPECT_EQ(item_names[5], item_slots.at(3).item);
    EXPECT_EQ(12, item_slots.at(3).amount);
    EXPECT_EQ(4, list->get_num_items());

    list->add(item_names[6], 65);
    EXPECT_EQ(item_names[1], item_slots.at(0).item);
    EXPECT_EQ(49, item_slots.at(0).amount);
    EXPECT_EQ(item_names[3], item_slots.at(1).item);
    EXPECT_EQ(90, item_slots.at(1).amount);
    EXPECT_EQ(item_names[4], item_slots.at(2).item);
    EXPECT_EQ(2, item_slots.at(2).amount);
    EXPECT_EQ(item_names[5], item_slots.at(3).item);
    EXPECT_EQ(12, item_slots.at(3).amount);
    EXPECT_EQ(item_names[6], item_slots.at(4).item);
    EXPECT_EQ(65, item_slots.at(4).amount);
    EXPECT_EQ(5, list->get_num_items());

    list->add(item_names[7], 6);
    EXPECT_EQ(item_names[1], item_slots.at(0).item);
    EXPECT_EQ(49, item_slots.at(0).amount);
    EXPECT_EQ(item_names[3], item_slots.at(1).item);
    EXPECT_EQ(90, item_slots.at(1).amount);
    EXPECT_EQ(item_names[4], item_slots.at(2).item);
    EXPECT_EQ(2, item_slots.at(2).amount);
    EXPECT_EQ(item_names[5], item_slots.at(3).item);
    EXPECT_EQ(12, item_slots.at(3).amount);
    EXPECT_EQ(item_names[6], item_slots.at(4).item);
    EXPECT_EQ(65, item_slots.at(4).amount);
    EXPECT_EQ(item_names[7], item_slots.at(5).item);
    EXPECT_EQ(6, item_slots.at(5).amount);
    EXPECT_EQ(6, list->get_num_items());
}
