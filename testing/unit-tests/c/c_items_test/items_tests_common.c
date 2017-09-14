/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_items_test.h"

#include <pkmn-c/database/lists.h>

#include <unity.h>

#include <string.h>

void test_item_list_empty_slots(
    pkmn_item_list_t* list
)
{
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NOT_NULL(list->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(list->_internal);

    TEST_ASSERT_EQUAL(0, list->num_items);
    TEST_ASSERT_EQUAL(list->capacity, list->item_slots.length);

    for(size_t i = 0; i < list->num_items; ++i)
    {
        TEST_ASSERT_EQUAL_STRING("None", list->item_slots.item_slots[i].item);
        TEST_ASSERT_EQUAL(0, list->item_slots.item_slots[i].amount);
    }
}

void test_item_list_out_of_range_error(
    pkmn_item_list_t* list,
    const char* item_name
)
{
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NOT_NULL(list->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(list->_internal);
    TEST_ASSERT_NOT_NULL(item_name);

    pkmn_error_t error = PKMN_ERROR_NONE;

    size_t old_num_items = list->num_items;

    error = pkmn_item_list_add(
                list,
                item_name,
                0
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_item_list_add(
                list,
                item_name,
                100
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_item_list_remove(
                list,
                item_name,
                0
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_item_list_remove(
                list,
                item_name,
                100
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_item_list_set_item(
                list,
                0,
                item_name,
                0
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_item_list_set_item(
                list,
                0,
                item_name,
                100
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // This should not have changed.
    TEST_ASSERT_EQUAL(old_num_items, list->num_items);
}

void test_item_list_invalid_items(
    pkmn_item_list_t* list,
    const char** item_names,
    size_t num_items
)
{
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NOT_NULL(list->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(list->_internal);
    TEST_ASSERT_NOT_NULL(item_names);
    TEST_ASSERT_TRUE(num_items > 0);

    pkmn_error_t error = PKMN_ERROR_NONE;

    size_t old_num_items = list->num_items;

    for(size_t i = 0; i < num_items; ++i)
    {
        error = pkmn_item_list_add(
                    list,
                    item_names[i],
                    num_items
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }

    // This should not have changed.
    TEST_ASSERT_EQUAL(old_num_items, list->num_items);
}

void test_item_bag_invalid_items(
    pkmn_item_bag_t* bag,
    const char** item_names,
    size_t num_items
)
{
    TEST_ASSERT_NOT_NULL(bag);

    pkmn_error_t error = PKMN_ERROR_NONE;

    for(size_t i = 0; i < num_items; ++i)
    {
        error = pkmn_item_bag_add(
                    bag,
                    item_names[i],
                    num_items
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
}

void test_item_list_set_item(
    pkmn_item_list_t* list,
    const char** item_names,
    size_t num_items
)
{
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NOT_NULL(list->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(list->_internal);
    TEST_ASSERT_NOT_NULL(item_names);
    TEST_ASSERT_EQUAL(3, num_items);

    pkmn_error_t error = PKMN_ERROR_NONE;

    // Test setting by index.
    error = pkmn_item_list_set_item(
        list,
        0,
        item_names[0],
        50
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_set_item(
        list,
        1,
        item_names[1],
        40
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_set_item(
        list,
        2,
        item_names[2],
        30
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_EQUAL_STRING(item_names[0], list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(50, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[1], list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(40, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[2], list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(30, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL(3, list->num_items);

    error = pkmn_item_list_set_item(
                list,
                10,
                item_names[0],
                5
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_item_list_set_item(
                list,
                0,
                "None",
                5
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_item_list_set_item(
                list,
                1,
                item_names[0],
                1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    TEST_ASSERT_EQUAL_STRING(item_names[0], list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(50, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[1], list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(40, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[2], list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(30, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL(3, list->num_items);

    error = pkmn_item_list_set_item(
                list,
                1,
                "None",
                0
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_EQUAL_STRING(item_names[0], list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(50, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[2], list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(30, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL(2, list->num_items);

    error = pkmn_item_list_set_item(
                list,
                0,
                "None",
                0
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_set_item(
                list,
                0,
                "None",
                0
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_EQUAL_STRING("None", list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(0, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL(0, list->num_items);
}

void test_item_list_add_remove(
    pkmn_item_list_t* list,
    const char** item_names,
    size_t num_items
)
{
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NOT_NULL(list->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(list->_internal);
    TEST_ASSERT_NOT_NULL(item_names);
    TEST_ASSERT_EQUAL(8, num_items);

    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_item_list_add(
                list,
                item_names[0],
                30
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_add(
                list,
                item_names[1],
                99
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_add(
                list,
                item_names[2],
                1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_EQUAL_STRING(item_names[0], list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(30, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[1], list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(99, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[2], list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(1, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL(3, list->num_items);

    error = pkmn_item_list_add(
                list,
                item_names[2],
                15
            );

    TEST_ASSERT_EQUAL_STRING(item_names[0], list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(30, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[1], list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(99, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[2], list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(16, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL(3, list->num_items);

    error = pkmn_item_list_remove(
                list,
                item_names[1],
                20
            );

    TEST_ASSERT_EQUAL_STRING(item_names[0], list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(30, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[1], list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(79, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[2], list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(16, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL(3, list->num_items);

    error = pkmn_item_list_move(
                list,
                0,
                1
            );

    TEST_ASSERT_EQUAL_STRING(item_names[1], list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(79, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[0], list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(30, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[2], list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(16, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL(3, list->num_items);

    error = pkmn_item_list_remove(
                list,
                item_names[0],
                30
            );

    TEST_ASSERT_EQUAL_STRING(item_names[1], list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(79, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[2], list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(16, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL(2, list->num_items);

    error = pkmn_item_list_add(
                list,
                item_names[3],
                90
            );

    TEST_ASSERT_EQUAL_STRING(item_names[1], list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(79, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[2], list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(16, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[3], list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(90, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL(3, list->num_items);

    error = pkmn_item_list_add(
                list,
                item_names[4],
                2
            );

    TEST_ASSERT_EQUAL_STRING(item_names[1], list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(79, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[2], list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(16, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[3], list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(90, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[4], list->item_slots.item_slots[3].item);
    TEST_ASSERT_EQUAL(2, list->item_slots.item_slots[3].amount);
    TEST_ASSERT_EQUAL(4, list->num_items);

    error = pkmn_item_list_remove(
                list,
                item_names[1],
                30
            );

    TEST_ASSERT_EQUAL_STRING(item_names[1], list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(49, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[2], list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(16, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[3], list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(90, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[4], list->item_slots.item_slots[3].item);
    TEST_ASSERT_EQUAL(2, list->item_slots.item_slots[3].amount);
    TEST_ASSERT_EQUAL(4, list->num_items);

    error = pkmn_item_list_add(
                list,
                item_names[5],
                12
            );

    TEST_ASSERT_EQUAL_STRING(item_names[1], list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(49, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[2], list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(16, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[3], list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(90, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[4], list->item_slots.item_slots[3].item);
    TEST_ASSERT_EQUAL(2, list->item_slots.item_slots[3].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[5], list->item_slots.item_slots[4].item);
    TEST_ASSERT_EQUAL(12, list->item_slots.item_slots[4].amount);
    TEST_ASSERT_EQUAL(5, list->num_items);

    error = pkmn_item_list_remove(
                list,
                item_names[2],
                16
            );

    TEST_ASSERT_EQUAL_STRING(item_names[1], list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(49, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[3], list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(90, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[4], list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(2, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[5], list->item_slots.item_slots[3].item);
    TEST_ASSERT_EQUAL(12, list->item_slots.item_slots[3].amount);
    TEST_ASSERT_EQUAL(4, list->num_items);

    error = pkmn_item_list_add(
                list,
                item_names[6],
                65
            );

    TEST_ASSERT_EQUAL_STRING(item_names[1], list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(49, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[3], list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(90, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[4], list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(2, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[5], list->item_slots.item_slots[3].item);
    TEST_ASSERT_EQUAL(12, list->item_slots.item_slots[3].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[6], list->item_slots.item_slots[4].item);
    TEST_ASSERT_EQUAL(65, list->item_slots.item_slots[4].amount);
    TEST_ASSERT_EQUAL(5, list->num_items);

    error = pkmn_item_list_add(
                list,
                item_names[7],
                6
            );

    TEST_ASSERT_EQUAL_STRING(item_names[1], list->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(49, list->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[3], list->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(90, list->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[4], list->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(2, list->item_slots.item_slots[2].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[5], list->item_slots.item_slots[3].item);
    TEST_ASSERT_EQUAL(12, list->item_slots.item_slots[3].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[6], list->item_slots.item_slots[4].item);
    TEST_ASSERT_EQUAL(65, list->item_slots.item_slots[4].amount);
    TEST_ASSERT_EQUAL_STRING(item_names[7], list->item_slots.item_slots[5].item);
    TEST_ASSERT_EQUAL(6, list->item_slots.item_slots[5].amount);
    TEST_ASSERT_EQUAL(6, list->num_items);
}
