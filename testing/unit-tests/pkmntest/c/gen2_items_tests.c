/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest-c/gen2_items_tests.h>

#include <unity.h>

#include <string.h>

#define STRBUFFER_LEN 1024

void pkmntest_gen2_item_pocket_test(
    pkmn_item_list_handle_t item_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];
    size_t actual_strlen = 0;

    int capacity = 0;
    int num_items = 0;
    pkmn_item_slots_t item_slots = NULL;
    size_t list_length = 0;
    //pkmn_string_list_t valid_items = NULL;
    //size_t valid_items_list_length = 0;

    // Check unchanging and initial values.
    const char* expected_name = "Items";
    TEST_ASSERT_EQUAL(pkmn_item_list_get_name(
                          item_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(expected_name)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_game(
                          item_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(game)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_capacity(
                          item_pocket,
                          &capacity
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(capacity, 20);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          item_pocket,
                          &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 0);

    // Make sure item slots start as correctly empty.
    TEST_ASSERT_EQUAL(pkmn_item_list_as_array(
                          item_pocket,
                          &item_slots,
                          &list_length
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(list_length, 20);
    for(size_t i = 0; i < 20; ++i) {
        TEST_ASSERT_EQUAL_STRING(item_slots[i].item, "None");
        TEST_ASSERT_EQUAL(item_slots[i].amount, 0);
    }
    pkmn_item_slots_free(&item_slots, list_length);
    TEST_ASSERT_NULL(item_slots);

    // Confirm errors are returned when expected.
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          item_pocket,
                          "Potion",
                          0
                      ), PKMN_ERROR_OUT_OF_RANGE);
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          item_pocket,
                          "Potion",
                          100
                      ), PKMN_ERROR_OUT_OF_RANGE);
    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          item_pocket,
                          "Potion",
                          0
                      ), PKMN_ERROR_OUT_OF_RANGE);
    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          item_pocket,
                          "Potion",
                          100
                      ), PKMN_ERROR_OUT_OF_RANGE);

    // Make sure we can't add items from other pockets.
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          item_pocket,
                          "Bicycle",
                          1
                      ), PKMN_ERROR_INVALID_ARGUMENT);
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          item_pocket,
                          "Master Ball",
                          1
                      ), PKMN_ERROR_INVALID_ARGUMENT);
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          item_pocket,
                          "HM01",
                          1
                      ), PKMN_ERROR_INVALID_ARGUMENT);
    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          item_pocket,
                          &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 0);

    // Start adding and removing stuff, and make sure the numbers are accurate.
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          item_pocket,
                          "Potion",
                          30
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          item_pocket,
                          "PSNCureBerry",
                          99
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          item_pocket,
                          "Leftovers",
                          1
                      ), PKMN_ERROR_NONE);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          item_pocket,
                          &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 3);
    TEST_ASSERT_EQUAL(pkmn_item_list_as_array(
                          item_pocket,
                          &item_slots,
                          &list_length
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(item_slots[0].item, "Potion");
    TEST_ASSERT_EQUAL(item_slots[0].amount, 30);
    TEST_ASSERT_EQUAL_STRING(item_slots[1].item, "PSNCureBerry");
    TEST_ASSERT_EQUAL(item_slots[1].amount, 99);
    TEST_ASSERT_EQUAL_STRING(item_slots[2].item, "Leftovers");
    TEST_ASSERT_EQUAL(item_slots[2].amount, 1);
    pkmn_item_slots_free(&item_slots, list_length);
    TEST_ASSERT_NULL(item_slots);
}

void pkmntest_gen2_key_item_pocket_test(
    pkmn_item_list_handle_t key_item_pocket,
    const char* game
) {
    (void)key_item_pocket;
    (void)game;
}

void pkmntest_gen2_ball_pocket_test(
    pkmn_item_list_handle_t ball_pocket,
    const char* game
) {
    (void)ball_pocket;
    (void)game;
}

void pkmntest_gen2_tmhm_pocket_test(
    pkmn_item_list_handle_t tmhm_pocket,
    const char* game
) {
    (void)tmhm_pocket;
    (void)game;
}

void pkmntest_gen2_pc_test(
    pkmn_item_list_handle_t pc,
    const char* game
) {
    (void)pc;
    (void)game;
}

void pkmntest_gen2_item_bag_test(
    pkmn_item_bag_handle_t bag,
    const char* game
) {
    pkmn_string_list_t pocket_names = NULL;
    size_t num_pockets = 0;

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

    TEST_ASSERT_EQUAL(pkmn_item_bag_get_pocket_names(
                          bag,
                          &pocket_names,
                          &num_pockets
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_pockets, 4);

    pkmn_string_list_free(&pocket_names, num_pockets);
    TEST_ASSERT_NULL(pocket_names);

    // Test pockets.
    pkmn_item_list_handle_t item_list_handle = NULL;

    TEST_ASSERT_EQUAL(pkmn_item_bag_get_pocket(
                          bag,
                          "Items",
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    pkmntest_gen2_item_pocket_test(
        item_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_list_handle);

    TEST_ASSERT_EQUAL(pkmn_item_bag_get_pocket(
                          bag,
                          "KeyItems",
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    pkmntest_gen2_key_item_pocket_test(
        item_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_list_handle);

    TEST_ASSERT_EQUAL(pkmn_item_bag_get_pocket(
                          bag,
                          "Balls",
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    pkmntest_gen2_ball_pocket_test(
        item_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_list_handle);

    TEST_ASSERT_EQUAL(pkmn_item_bag_get_pocket(
                          bag,
                          "TM/HM",
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    pkmntest_gen2_tmhm_pocket_test(
        item_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_list_handle);
}
