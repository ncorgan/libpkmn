/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "items_tests_common.h"

#include <pkmn-c/database/lists.h>

#include <unity.h>

#include <string.h>

#define STRBUFFER_LEN 1024

void test_item_list_empty_slots(
    pkmn_item_list_handle_t list
) {
    TEST_ASSERT_NOT_NULL(list);

    pkmn_item_slots_t item_slots = NULL;
    size_t list_length = 0;
    int capacity = 0;

    TEST_ASSERT_EQUAL(pkmn_item_list_get_capacity(
                          list,
                          &capacity
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_item_list_as_array(
                          list,
                          &item_slots,
                          &list_length
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(capacity, (int)list_length);

    for(size_t i = 0; i < list_length; ++i) {
        TEST_ASSERT_EQUAL_STRING(item_slots[i].item, "None");
        TEST_ASSERT_EQUAL(item_slots[i].amount, 0);
    }
    pkmn_item_slots_free(&item_slots, list_length);
    TEST_ASSERT_NULL(item_slots);

}

void test_item_list_out_of_range_error(
    pkmn_item_list_handle_t list,
    const char* item_name
) {
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NOT_NULL(item_name);

    int num_items_before = 0;
    int num_items_after = 0;

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          list,
                          &num_items_before
                      ), PKMN_ERROR_NONE);

    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          list,
                          item_name,
                          0
                      ), PKMN_ERROR_OUT_OF_RANGE);
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          list,
                          item_name,
                          100
                      ), PKMN_ERROR_OUT_OF_RANGE);
    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          list,
                          item_name,
                          0
                      ), PKMN_ERROR_OUT_OF_RANGE);
    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          list,
                          item_name,
                          100
                      ), PKMN_ERROR_OUT_OF_RANGE);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          list,
                          &num_items_after
                      ), PKMN_ERROR_NONE);

    // This shouldn't have added any items
    TEST_ASSERT_EQUAL(num_items_before, num_items_after);
}

void test_item_list_items_from_wrong_pocket(
    pkmn_item_list_handle_t list,
    const char** item_names,
    size_t item_names_list_length
) {
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NOT_NULL(item_names);

    int num_items_before = 0;
    int num_items_after = 0;

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          list,
                          &num_items_before
                      ), PKMN_ERROR_NONE);

    for(size_t i = 0; i < item_names_list_length; ++i) {
        TEST_ASSERT_EQUAL(pkmn_item_list_add(
                              list,
                              item_names[i],
                              1
                          ), PKMN_ERROR_INVALID_ARGUMENT);
    }

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          list,
                          &num_items_after
                      ), PKMN_ERROR_NONE);

    // This shouldn't have added any items
    TEST_ASSERT_EQUAL(num_items_before, num_items_after);
}

static void test_item_slots(
    pkmn_item_list_handle_t list,
    const char** expected_item_names,
    int* expected_item_amounts,
    int expected_num_items
) {
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NOT_NULL(expected_item_names);
    TEST_ASSERT_NOT_NULL(expected_item_amounts);

    pkmn_item_slots_t item_slots = NULL;
    size_t list_length = 0;
    int num_items = 0;

    TEST_ASSERT_EQUAL(pkmn_item_list_as_array(
                          list,
                          &item_slots,
                          &list_length
                      ), PKMN_ERROR_NONE);
    for(int i = 0; i < expected_num_items; ++i) {
        TEST_ASSERT_EQUAL_STRING(item_slots[i].item, expected_item_names[i]);
        TEST_ASSERT_EQUAL(item_slots[i].amount, expected_item_amounts[i]);
    }

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          list,
                          &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, expected_num_items);

    pkmn_item_slots_free(&item_slots, list_length);
    TEST_ASSERT_NULL(item_slots);
}

void test_item_list_add_remove(
    pkmn_item_list_handle_t list,
    const char** item_names,
    size_t item_names_list_length
) {
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NOT_NULL(item_names);
    TEST_ASSERT_EQUAL(item_names_list_length, 8);

    int num_items_before = 0;
    int num_items_after = 0;
    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          list,
                          &num_items_before
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items_before, 0);

    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          list,
                          item_names[0],
                          30
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          list,
                          item_names[1],
                          99
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          list,
                          item_names[2],
                          1
                      ), PKMN_ERROR_NONE);

    const char* item_names1[] = {item_names[0], item_names[1], item_names[2]};
    int item_amounts1[] = {30, 99, 1};
    test_item_slots(
        list,
        item_names1,
        item_amounts1,
        3
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          list,
                          item_names[2],
                          15
                      ), PKMN_ERROR_NONE);

    const char* item_names2[] = {item_names[0], item_names[1], item_names[2]};
    int item_amounts2[] = {30, 99, 16};
    test_item_slots(
        list,
        item_names2,
        item_amounts2,
        3
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          list,
                          item_names[1],
                          20
                      ), PKMN_ERROR_NONE);

    const char* item_names3[] = {item_names[0], item_names[1], item_names[2]};
    int item_amounts3[] = {30, 79, 16};
    test_item_slots(
        list,
        item_names3,
        item_amounts3,
        3
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_move(
                          list,
                          0,
                          1
                      ), PKMN_ERROR_NONE);

    const char* item_names4[] = {item_names[1], item_names[0], item_names[2]};
    int item_amounts4[] = {79, 30, 16};
    test_item_slots(
        list,
        item_names4,
        item_amounts4,
        3
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          list,
                          item_names[0],
                          30
                      ), PKMN_ERROR_NONE);

    const char* item_names5[] = {item_names[1], item_names[2], "None"};
    int item_amounts5[] = {79, 16, 0};
    test_item_slots(
        list,
        item_names5,
        item_amounts5,
        2
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          list,
                          item_names[3],
                          90
                      ), PKMN_ERROR_NONE);

    const char* item_names6[] = {item_names[1], item_names[2], item_names[3]};
    int item_amounts6[] = {79, 16, 90};
    test_item_slots(
        list,
        item_names6,
        item_amounts6,
        3
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          list,
                          item_names[4],
                          2
                      ), PKMN_ERROR_NONE);

    const char* item_names7[] = {item_names[1], item_names[2], item_names[3], item_names[4]};
    int item_amounts7[] = {79, 16, 90, 2};
    test_item_slots(
        list,
        item_names7,
        item_amounts7,
        4
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          list,
                          item_names[1],
                          30
                      ), PKMN_ERROR_NONE);

    const char* item_names8[] = {item_names[1], item_names[2], item_names[3], item_names[4]};
    int item_amounts8[] = {49, 16, 90, 2};
    test_item_slots(
        list,
        item_names8,
        item_amounts8,
        4
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          list,
                          item_names[5],
                          12
                      ), PKMN_ERROR_NONE);

    const char* item_names9[] = {item_names[1], item_names[2], item_names[3], item_names[4],
                                 item_names[5]};
    int item_amounts9[] = {49, 16, 90, 2, 12};
    test_item_slots(
        list,
        item_names9,
        item_amounts9,
        5
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          list,
                          item_names[2],
                          16
                      ), PKMN_ERROR_NONE);

    const char* item_names10[] = {item_names[1], item_names[3], item_names[4], item_names[5]};
    int item_amounts10[] = {49, 90, 2, 12};
    test_item_slots(
        list,
        item_names10,
        item_amounts10,
        4
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          list,
                          item_names[6],
                          65
                      ), PKMN_ERROR_NONE);

    const char* item_names11[] = {item_names[1], item_names[3], item_names[4], item_names[5],
                                  item_names[6]};
    int item_amounts11[] = {49, 90, 2, 12, 65};
    test_item_slots(
        list,
        item_names11,
        item_amounts11,
        5
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          list,
                          item_names[7],
                          6
                      ), PKMN_ERROR_NONE);

    const char* item_names12[] = {item_names[1], item_names[3], item_names[4], item_names[5],
                                  item_names[6], item_names[7]};
    int item_amounts12[] = {49, 90, 2, 12, 65, 6};
    test_item_slots(
        list,
        item_names12,
        item_amounts12,
        6
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          list,
                          item_names[4],
                          2
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          list,
                          item_names[1],
                          49
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          list,
                          item_names[7],
                          6
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          list,
                          item_names[5],
                          12
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          list,
                          item_names[3],
                          90
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          list,
                          item_names[6],
                          65
                      ), PKMN_ERROR_NONE);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          list,
                          &num_items_after
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items_after, 0);
}
