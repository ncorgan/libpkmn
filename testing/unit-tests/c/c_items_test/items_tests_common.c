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

#define STRBUFFER_LEN 1024

void test_item_list_empty_slots(
    pkmn_item_list_handle_t list
) {
    TEST_ASSERT_NOT_NULL(list);

    pkmn_item_slots_t item_slots = {
        .item_slots = NULL,
        .length = 0
    };
    int capacity = 0;

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_capacity(
            list,
            &capacity
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_as_array(
            list,
            &item_slots
        )
    );
    TEST_ASSERT_EQUAL((int)item_slots.length, capacity);

    for(size_t i = 0; i < item_slots.length; ++i) {
        TEST_ASSERT_EQUAL_STRING("None", item_slots.item_slots[i].item);
        TEST_ASSERT_EQUAL(0, item_slots.item_slots[i].amount);
    }
    pkmn_item_slots_free(&item_slots);
    TEST_ASSERT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL(0, item_slots.length);
}

void test_item_list_out_of_range_error(
    pkmn_item_list_handle_t list,
    const char* item_name
) {
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NOT_NULL(item_name);

    int num_items_before = 0;
    int num_items_after = 0;

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            list,
            &num_items_before
        )
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_OUT_OF_RANGE,
        pkmn_item_list_add(
            list,
            item_name,
            0
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_OUT_OF_RANGE,
        pkmn_item_list_add(
            list,
            item_name,
            100
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_OUT_OF_RANGE,
        pkmn_item_list_remove(
            list,
            item_name,
            0
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_OUT_OF_RANGE,
        pkmn_item_list_remove(
            list,
            item_name,
            100
        )
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            list,
            &num_items_after
        )
    );

    // This shouldn't have added any items
    TEST_ASSERT_EQUAL(num_items_after, num_items_before);
}

void test_item_bag_invalid_items(
    pkmn_item_bag_handle_t bag,
    const char** item_names,
    size_t item_names_list_length
) {
    TEST_ASSERT_NOT_NULL(bag);
    TEST_ASSERT_NOT_NULL(item_names);

    for(size_t i = 0; i < item_names_list_length; ++i) {
        TEST_ASSERT_EQUAL(
            PKMN_ERROR_INVALID_ARGUMENT,
            pkmn_item_bag_add(
                bag,
                item_names[i],
                1
            )
        );
    }
}

void test_item_list_invalid_items(
    pkmn_item_list_handle_t list,
    const char** item_names,
    size_t item_names_list_length
) {
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NOT_NULL(item_names);

    int num_items_before = 0;
    int num_items_after = 0;

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            list,
            &num_items_before
        )
    );

    for(size_t i = 0; i < item_names_list_length; ++i) {
        TEST_ASSERT_EQUAL(
            PKMN_ERROR_INVALID_ARGUMENT,
            pkmn_item_list_add(
                list,
                item_names[i],
                1
            )
        );
    }

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            list,
            &num_items_after
        )
    );

    // This shouldn't have added any items
    TEST_ASSERT_EQUAL(num_items_after, num_items_before);
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

    pkmn_item_slots_t item_slots = {
        .item_slots = NULL,
        .length = 0
    };
    int num_items = 0;

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_as_array(
            list,
            &item_slots
        )
    );
    for(int i = 0; i < expected_num_items; ++i) {
        TEST_ASSERT_EQUAL_STRING(expected_item_names[i], item_slots.item_slots[i].item);
        TEST_ASSERT_EQUAL(expected_item_amounts[i], item_slots.item_slots[i].amount);
    }

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            list,
            &num_items
        )
    );
    TEST_ASSERT_EQUAL(expected_num_items, num_items);

    pkmn_item_slots_free(&item_slots);
    TEST_ASSERT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL(0, item_slots.length);
}

void test_item_list_add_remove(
    pkmn_item_list_handle_t list,
    const char** item_names,
    size_t item_names_list_length
) {
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NOT_NULL(item_names);
    TEST_ASSERT_EQUAL(8, item_names_list_length);

    int num_items_before = 0;
    int num_items_after = 0;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            list,
            &num_items_before
        )
    );
    TEST_ASSERT_EQUAL(0, num_items_before);

    // Test setting by index.
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_set_item(
            list,
            0,
            item_names[0],
            50
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_set_item(
            list,
            1,
            item_names[1],
            40
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_set_item(
            list,
            2,
            item_names[2],
            30
        )
    );

    const char* item_names1[] = {item_names[0], item_names[1], item_names[2]};
    int item_amounts1[] = {50, 40, 30};
    test_item_slots(
        list,
        item_names1,
        item_amounts1,
        3
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_OUT_OF_RANGE,
        pkmn_item_list_set_item(
            list,
            10,
            "Ultra Ball",
            5
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_INVALID_ARGUMENT,
        pkmn_item_list_set_item(
            list,
            0,
            "None",
            5
        )
    );

    const char* item_names2[] = {item_names[0], item_names[1], item_names[2]};
    int item_amounts2[] = {50, 40, 30};
    test_item_slots(
        list,
        item_names2,
        item_amounts2,
        3
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_set_item(
            list,
            1,
            "None",
            0
        )
    );

    const char* item_names3[] = {item_names[0], item_names[2], "None"};
    int item_amounts3[] = {50, 30, 0};
    test_item_slots(
        list,
        item_names3,
        item_amounts3,
        2
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_set_item(
            list,
            0,
            "None",
            0
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_set_item(
            list,
            0,
            "None",
            0
        )
    );

    const char* item_names4[] = {"None", "None", "None"};
    int item_amounts4[] = {0, 0, 0};
    test_item_slots(
        list,
        item_names4,
        item_amounts4,
        0
    );

    // Test add/remove functions.
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_add(
            list,
            item_names[0],
            30
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_add(
            list,
            item_names[1],
            99
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_add(
            list,
            item_names[2],
            1
        )
    );

    const char* item_names5[] = {item_names[0], item_names[1], item_names[2]};
    int item_amounts5[] = {30, 99, 1};
    test_item_slots(
        list,
        item_names5,
        item_amounts5,
        3
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_add(
            list,
            item_names[2],
            15
        )
    );

    const char* item_names6[] = {item_names[0], item_names[1], item_names[2]};
    int item_amounts6[] = {30, 99, 16};
    test_item_slots(
        list,
        item_names6,
        item_amounts6,
        3
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_remove(
            list,
            item_names[1],
            20
        )
    );

    const char* item_names7[] = {item_names[0], item_names[1], item_names[2]};
    int item_amounts7[] = {30, 79, 16};
    test_item_slots(
        list,
        item_names7,
        item_amounts7,
        3
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_move(
            list,
            0,
            1
        )
    );

    const char* item_names8[] = {item_names[1], item_names[0], item_names[2]};
    int item_amounts8[] = {79, 30, 16};
    test_item_slots(
        list,
        item_names8,
        item_amounts8,
        3
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_remove(
            list,
            item_names[0],
            30
        )
    );

    const char* item_names9[] = {item_names[1], item_names[2], "None"};
    int item_amounts9[] = {79, 16, 0};
    test_item_slots(
        list,
        item_names9,
        item_amounts9,
        2
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_add(
            list,
            item_names[3],
            90
        )
    );

    const char* item_names10[] = {item_names[1], item_names[2], item_names[3]};
    int item_amounts10[] = {79, 16, 90};
    test_item_slots(
        list,
        item_names10,
        item_amounts10,
        3
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_add(
            list,
            item_names[4],
            2
        )
    );

    const char* item_names11[] = {item_names[1], item_names[2], item_names[3], item_names[4]};
    int item_amounts11[] = {79, 16, 90, 2};
    test_item_slots(
        list,
        item_names11,
        item_amounts11,
        4
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_remove(
            list,
            item_names[1],
            30
        )
    );

    const char* item_names12[] = {item_names[1], item_names[2], item_names[3], item_names[4]};
    int item_amounts12[] = {49, 16, 90, 2};
    test_item_slots(
        list,
        item_names12,
        item_amounts12,
        4
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_add(
            list,
            item_names[5],
            12
        )
    );

    const char* item_names13[] = {item_names[1], item_names[2], item_names[3], item_names[4],
                                 item_names[5]};
    int item_amounts13[] = {49, 16, 90, 2, 12};
    test_item_slots(
        list,
        item_names13,
        item_amounts13,
        5
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_remove(
            list,
            item_names[2],
            16
        )
    );

    const char* item_names14[] = {item_names[1], item_names[3], item_names[4], item_names[5]};
    int item_amounts14[] = {49, 90, 2, 12};
    test_item_slots(
        list,
        item_names14,
        item_amounts14,
        4
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_add(
            list,
            item_names[6],
            65
        )
    );

    const char* item_names15[] = {item_names[1], item_names[3], item_names[4], item_names[5],
                                  item_names[6]};
    int item_amounts15[] = {49, 90, 2, 12, 65};
    test_item_slots(
        list,
        item_names15,
        item_amounts15,
        5
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_add(
            list,
            item_names[7],
            6
        )
    );

    const char* item_names16[] = {item_names[1], item_names[3], item_names[4], item_names[5],
                                  item_names[6], item_names[7]};
    int item_amounts16[] = {49, 90, 2, 12, 65, 6};
    test_item_slots(
        list,
        item_names16,
        item_amounts16,
        6
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_remove(
            list,
            item_names[4],
            2
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_remove(
            list,
            item_names[1],
            49
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_remove(
            list,
            item_names[7],
            6
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_remove(
            list,
            item_names[5],
            12
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_remove(
            list,
            item_names[3],
            90
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_remove(
            list,
            item_names[6],
            65
        )
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            list,
            &num_items_after
        )
    );
    TEST_ASSERT_EQUAL(0, num_items_after);
}
