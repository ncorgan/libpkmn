/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_test_common.hpp"

#include <pkmn/enums/enum_to_string.hpp>

void test_item_list_empty_slots(
    const pkmn::item_list::sptr& list
)
{
    ASSERT_EQ(0, list->get_num_items());

    const pkmn::item_slots_t& item_slots = list->as_vector();
    ASSERT_EQ(list->get_capacity(), item_slots.size());
    for(const pkmn::item_slot& item_slot: item_slots)
    {
        EXPECT_EQ(pkmn::e_item::NONE, item_slot.item);
        EXPECT_EQ(0, item_slot.amount);
    }
}

void test_item_list_out_of_range_error(
    const pkmn::item_list::sptr& list,
    pkmn::e_item item
)
{
    int num_items = list->get_num_items();

    EXPECT_THROW(
        list->add(item, 0)
    , std::out_of_range);
    EXPECT_THROW(
        list->add(item, 100)
    , std::out_of_range);
    EXPECT_THROW(
        list->remove(item, 0)
    , std::out_of_range);
    EXPECT_THROW(
        list->remove(item, 100)
    , std::out_of_range);

    // This shouldn't have added any items
    EXPECT_EQ(num_items, list->get_num_items());
}

void test_item_bag_invalid_items(
    const pkmn::item_bag::sptr& bag,
    const std::vector<pkmn::e_item>& items
)
{
    for(pkmn::e_item item: items)
    {
        EXPECT_THROW(
            bag->add(item, 1);
        , std::invalid_argument);
    }
}

void test_item_list_invalid_items(
    const pkmn::item_list::sptr& list,
    const std::vector<pkmn::e_item>& items
)
{
    int num_items = list->get_num_items();

    for(pkmn::e_item item: items)
    {
        EXPECT_THROW(
            list->add(item, 1);
        , std::invalid_argument);
    }

    // This shouldn't have added any items
    EXPECT_EQ(num_items, list->get_num_items());
}

void test_item_list_add_remove(
    const pkmn::item_list::sptr& list,
    const std::vector<pkmn::e_item>& items
)
{
    ASSERT_EQ(0, list->get_num_items());
    ASSERT_EQ(8, items.size());
    const pkmn::item_slots_t& item_slots = list->as_vector();

    // Test setting by index.
    list->set_item(0, items[0], 50);
    list->set_item(1, items[1], 40);
    list->set_item(2, items[2], 30);
    EXPECT_EQ(items[0], item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(50, item_slots.at(0).amount);
    EXPECT_EQ(items[1], item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(40, item_slots.at(1).amount);
    EXPECT_EQ(items[2], item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(30, item_slots.at(2).amount);
    EXPECT_EQ(3, list->get_num_items());

    EXPECT_THROW(
        list->set_item(10, pkmn::e_item::ULTRA_BALL, 5)
    , std::out_of_range);
    EXPECT_THROW(
        list->set_item(0, pkmn::e_item::NONE, 5)
    , std::invalid_argument);
    EXPECT_THROW(
        list->set_item(1, items[0], 1)
    , std::invalid_argument);
    EXPECT_EQ(items[0], item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(50, item_slots.at(0).amount);
    EXPECT_EQ(items[1], item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(40, item_slots.at(1).amount);
    EXPECT_EQ(items[2], item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(30, item_slots.at(2).amount);
    EXPECT_EQ(3, list->get_num_items());

    list->set_item(1, pkmn::e_item::NONE, 0);
    EXPECT_EQ(items[0], item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(50, item_slots.at(0).amount);
    EXPECT_EQ(items[2], item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(30, item_slots.at(1).amount);
    EXPECT_EQ(pkmn::e_item::NONE, item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(0, item_slots.at(2).amount);
    EXPECT_EQ(2, list->get_num_items());

    list->set_item(0, pkmn::e_item::NONE, 0);
    list->set_item(0, pkmn::e_item::NONE, 0);
    EXPECT_EQ(pkmn::e_item::NONE, item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(0, item_slots.at(0).amount);
    EXPECT_EQ(pkmn::e_item::NONE, item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(0, item_slots.at(1).amount);
    EXPECT_EQ(pkmn::e_item::NONE, item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(0, item_slots.at(2).amount);
    EXPECT_EQ(0, list->get_num_items());

    // Test add/remove functions.
    list->add(items[0], 30);
    list->add(items[1], 99);
    list->add(items[2], 1);

    EXPECT_EQ(items[0], item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(30, item_slots.at(0).amount);
    EXPECT_EQ(items[1], item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(99, item_slots.at(1).amount);
    EXPECT_EQ(items[2], item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(1, item_slots.at(2).amount);
    EXPECT_EQ(3, list->get_num_items());

    list->add(items[2], 15);
    EXPECT_EQ(items[0], item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(30, item_slots.at(0).amount);
    EXPECT_EQ(items[1], item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(99, item_slots.at(1).amount);
    EXPECT_EQ(items[2], item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(16, item_slots.at(2).amount);
    EXPECT_EQ(3, list->get_num_items());

    list->remove(items[1], 20);
    EXPECT_EQ(items[0], item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(30, item_slots.at(0).amount);
    EXPECT_EQ(items[1], item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(79, item_slots.at(1).amount);
    EXPECT_EQ(items[2], item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(16, item_slots.at(2).amount);
    EXPECT_EQ(3, list->get_num_items());

    list->move(0, 1);
    EXPECT_EQ(items[1], item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(79, item_slots.at(0).amount);
    EXPECT_EQ(items[0], item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(30, item_slots.at(1).amount);
    EXPECT_EQ(items[2], item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(16, item_slots.at(2).amount);
    EXPECT_EQ(3, list->get_num_items());

    list->remove(items[0], 30);
    EXPECT_EQ(items[1], item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(79, item_slots.at(0).amount);
    EXPECT_EQ(items[2], item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(16, item_slots.at(1).amount);
    EXPECT_EQ(pkmn::e_item::NONE, item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(0, item_slots.at(2).amount);
    EXPECT_EQ(2, list->get_num_items());

    list->add(items[3], 90);
    EXPECT_EQ(items[1], item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(79, item_slots.at(0).amount);
    EXPECT_EQ(items[2], item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(16, item_slots.at(1).amount);
    EXPECT_EQ(items[3], item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(90, item_slots.at(2).amount);
    EXPECT_EQ(3, list->get_num_items());

    list->add(items[4], 2);
    EXPECT_EQ(items[1], item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(79, item_slots.at(0).amount);
    EXPECT_EQ(items[2], item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(16, item_slots.at(1).amount);
    EXPECT_EQ(items[3], item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(90, item_slots.at(2).amount);
    EXPECT_EQ(items[4], item_slots.at(3).item) << pkmn::item_to_string(item_slots.at(3).item);
    EXPECT_EQ(2, item_slots.at(3).amount);
    EXPECT_EQ(4, list->get_num_items());

    list->remove(items[1], 30);
    EXPECT_EQ(items[1], item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(49, item_slots.at(0).amount);
    EXPECT_EQ(items[2], item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(16, item_slots.at(1).amount);
    EXPECT_EQ(items[3], item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(90, item_slots.at(2).amount);
    EXPECT_EQ(items[4], item_slots.at(3).item) << pkmn::item_to_string(item_slots.at(3).item);
    EXPECT_EQ(2, item_slots.at(3).amount);
    EXPECT_EQ(4, list->get_num_items());

    list->add(items[5], 12);
    EXPECT_EQ(items[1], item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(49, item_slots.at(0).amount);
    EXPECT_EQ(items[2], item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(16, item_slots.at(1).amount);
    EXPECT_EQ(items[3], item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(90, item_slots.at(2).amount);
    EXPECT_EQ(items[4], item_slots.at(3).item) << pkmn::item_to_string(item_slots.at(3).item);
    EXPECT_EQ(2, item_slots.at(3).amount);
    EXPECT_EQ(items[5], item_slots.at(4).item) << pkmn::item_to_string(item_slots.at(4).item);
    EXPECT_EQ(12, item_slots.at(4).amount);
    EXPECT_EQ(5, list->get_num_items());

    list->remove(items[2], 16);
    EXPECT_EQ(items[1], item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(49, item_slots.at(0).amount);
    EXPECT_EQ(items[3], item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(90, item_slots.at(1).amount);
    EXPECT_EQ(items[4], item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(2, item_slots.at(2).amount);
    EXPECT_EQ(items[5], item_slots.at(3).item) << pkmn::item_to_string(item_slots.at(3).item);
    EXPECT_EQ(12, item_slots.at(3).amount);
    EXPECT_EQ(4, list->get_num_items());

    list->add(items[6], 65);
    EXPECT_EQ(items[1], item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(49, item_slots.at(0).amount);
    EXPECT_EQ(items[3], item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(90, item_slots.at(1).amount);
    EXPECT_EQ(items[4], item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(2, item_slots.at(2).amount);
    EXPECT_EQ(items[5], item_slots.at(3).item) << pkmn::item_to_string(item_slots.at(3).item);
    EXPECT_EQ(12, item_slots.at(3).amount);
    EXPECT_EQ(items[6], item_slots.at(4).item) << pkmn::item_to_string(item_slots.at(4).item);
    EXPECT_EQ(65, item_slots.at(4).amount);
    EXPECT_EQ(5, list->get_num_items());

    list->add(items[7], 6);
    EXPECT_EQ(items[1], item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(49, item_slots.at(0).amount);
    EXPECT_EQ(items[3], item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(90, item_slots.at(1).amount);
    EXPECT_EQ(items[4], item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(2, item_slots.at(2).amount);
    EXPECT_EQ(items[5], item_slots.at(3).item) << pkmn::item_to_string(item_slots.at(3).item);
    EXPECT_EQ(12, item_slots.at(3).amount);
    EXPECT_EQ(items[6], item_slots.at(4).item) << pkmn::item_to_string(item_slots.at(4).item);
    EXPECT_EQ(65, item_slots.at(4).amount);
    EXPECT_EQ(items[7], item_slots.at(5).item) << pkmn::item_to_string(item_slots.at(5).item);
    EXPECT_EQ(6, item_slots.at(5).amount);
    EXPECT_EQ(6, list->get_num_items());
}
