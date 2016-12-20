/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "items_tests_common.h"

#include <pkmntest-c/gen1_items_tests.h>

#include <pkmn-c/database/lists.h>

#include <unity.h>

#include <string.h>

#define STRBUFFER_LEN 1024

static const char* item_names[] = {
    "Potion",
    "Great Ball",
    "Ether",
    "PP Up",
    "TM34",
    "Moon Stone",
    "Bicycle",
    "Full Heal"
};
static const char* wrong_generation_item_names[] = {
    "Amulet Coin",
    "Apicot Berry",
    "Air Mail",
    "Air Balloon",
    "Aqua Suit"
};

static void pkmntest_gen1_item_list_common(
    pkmn_item_list_handle_t items,
    const char* game
) {
    pkmn_string_list_t full_item_list = {
        .strings = NULL,
        .length = 0
    };
    pkmn_string_list_t valid_items = {
        .strings = NULL,
        .length = 0
    };

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(items);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        items,
        "Potion"
    );

    // Confirm items from later generations can't be added.
    test_item_list_invalid_items(
        items,
        wrong_generation_item_names,
        5
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    test_item_list_add_remove(
        items,
        item_names,
        8
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_get_valid_items(
                          items, &valid_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_database_item_list(
                          game, &full_item_list
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(valid_items.length, full_item_list.length);

    pkmn_string_list_free(&full_item_list);
    TEST_ASSERT_NULL(full_item_list.strings);
    TEST_ASSERT_EQUAL(full_item_list.length, 0);

    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(valid_items.length, 0);
}

void pkmntest_gen1_item_list_test(
    pkmn_item_list_handle_t items,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN] = "";
    size_t actual_strlen = 0;

    // Check unchanging and initial values.
    const char* expected_name = "Items";
    TEST_ASSERT_EQUAL(pkmn_item_list_get_name(
                          items,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(expected_name)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_game(
                          items,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(game)+1);

    int capacity = 0;
    TEST_ASSERT_EQUAL(pkmn_item_list_get_capacity(
                          items, &capacity
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(capacity, 20);

    int num_items = 0;
    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          items, &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 0);

    pkmntest_gen1_item_list_common(items, game);
}

void pkmntest_gen1_pc_test(
    pkmn_item_list_handle_t pc,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN] = "";
    size_t actual_strlen = 0;

    // Check unchanging and initial values.
    const char* expected_name = "PC";
    TEST_ASSERT_EQUAL(pkmn_item_list_get_name(
                          pc,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(expected_name)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_game(
                          pc,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(game)+1);

    int capacity = 0;
    TEST_ASSERT_EQUAL(pkmn_item_list_get_capacity(
                          pc, &capacity
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(capacity, 50);

    int num_items = 0;
    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          pc, &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 0);

    pkmntest_gen1_item_list_common(pc, game);
}

void pkmntest_gen1_item_bag_test(
    pkmn_item_bag_handle_t bag,
    const char* game
) {
    pkmn_string_list_t pocket_names = {
        .strings = NULL,
        .length = 0
    };
    pkmn_item_slots_t item_slots = {
        .item_slots = NULL,
        .length = 0
    };

    char strbuffer[STRBUFFER_LEN] = "";
    size_t actual_strlen = 0;

    // Check unchanging and initial values.
    TEST_ASSERT_EQUAL(pkmn_item_bag_get_game(
                          bag,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(game)+1);

    test_item_bag_invalid_items(
        bag,
        wrong_generation_item_names,
        5
    );

    TEST_ASSERT_EQUAL(pkmn_item_bag_get_pocket_names(
                          bag, &pocket_names
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pocket_names.length, 1);

    pkmn_string_list_free(&pocket_names);
    TEST_ASSERT_NULL(pocket_names.strings);
    TEST_ASSERT_EQUAL(pocket_names.length, 0);

    pkmn_item_list_handle_t items;
    TEST_ASSERT_EQUAL(pkmn_item_bag_get_pocket(
                          bag, "Items", &items
                      ), PKMN_ERROR_NONE);

    pkmntest_gen1_item_list_test(items, game);

    // Make sure adding items through the bag adds to the pocket.
    int num_items = 0;
    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          items, &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 0);

    for(int i = 0; i < 8; ++i) {
        TEST_ASSERT_EQUAL(pkmn_item_bag_add(
                              bag,
                              item_names[i],
                              i+1
                          ), PKMN_ERROR_NONE);
    }

    TEST_ASSERT_EQUAL(pkmn_item_list_as_array(
                          items,
                          &item_slots
                      ), PKMN_ERROR_NONE);

    for(int i = 0; i < 8; ++i) {
        TEST_ASSERT_EQUAL_STRING(item_slots.item_slots[i].item, item_names[i]);
        TEST_ASSERT_EQUAL(item_slots.item_slots[i].amount, i+1);
    }

    TEST_ASSERT_EQUAL_STRING(item_slots.item_slots[8].item, "None");
    TEST_ASSERT_EQUAL(item_slots.item_slots[8].amount, 0);

    TEST_ASSERT_EQUAL(pkmn_item_slots_free(&item_slots),
                      PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL(item_slots.length, 0);

    for(int i = 0; i < 8; ++i) {
        TEST_ASSERT_EQUAL(pkmn_item_bag_remove(
                              bag,
                              item_names[i],
                              i+1
                          ), PKMN_ERROR_NONE);
    }

    TEST_ASSERT_EQUAL(pkmn_item_list_as_array(
                          items,
                          &item_slots
                      ), PKMN_ERROR_NONE);

    for(int i = 0; i < 9; ++i) {
        TEST_ASSERT_EQUAL_STRING(item_slots.item_slots[i].item, "None");
        TEST_ASSERT_EQUAL(item_slots.item_slots[i].amount, 0);
    }

    TEST_ASSERT_EQUAL(pkmn_item_slots_free(&item_slots),
                      PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL(item_slots.length, 0);

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(items);
}
