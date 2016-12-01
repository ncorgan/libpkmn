/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "items_tests_common.h"

#include <pkmntest-c/gen2_items_tests.h>

#include <pkmn-c/database/lists.h>

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
    pkmn_string_list_t valid_items = NULL;
    size_t valid_items_list_length = 0;

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
    test_item_list_empty_slots(item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        item_pocket,
        "Potion"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_items[] = {
        "Bicycle",
        "Master Ball",
        "HM01"
    };
    test_item_list_items_from_wrong_pocket(
        item_pocket,
        wrong_items,
        3
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    const char* item_names[] = {
        "Potion",
        "HP Up",
        "Wht Apricorn",
        "Lucky Egg",
        "Flower Mail",
        "Burn Heal",
        "PSNCureBerry",
        "Stick"
    };
    test_item_list_add_remove(
        item_pocket,
        item_names,
        8
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_get_valid_items(
                          item_pocket,
                          &valid_items,
                          &valid_items_list_length
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT(valid_items_list_length > 0);
    pkmn_string_list_free(&valid_items, valid_items_list_length);
    TEST_ASSERT_NULL(valid_items);
}

void pkmntest_gen2_key_item_pocket_test(
    pkmn_item_list_handle_t key_item_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];
    size_t actual_strlen = 0;

    int capacity = 0;
    int num_items = 0;
    pkmn_string_list_t valid_items = NULL;
    size_t valid_items_list_length = 0;

    // Check unchanging and initial values.
    const char* expected_name = "KeyItems";
    TEST_ASSERT_EQUAL(pkmn_item_list_get_name(
                          key_item_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(expected_name)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_game(
                          key_item_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(game)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_capacity(
                          key_item_pocket,
                          &capacity
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(capacity, 26);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          key_item_pocket,
                          &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 0);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(key_item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        key_item_pocket,
        "Bicycle"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_items[] = {
        "Potion",
        "Master Ball",
        "HM01"
    };
    test_item_list_items_from_wrong_pocket(
        key_item_pocket,
        wrong_items,
        3
    );

    // Crystal-specific items.
    if(strcmp(game, "Crystal") == 0) {
        TEST_ASSERT_EQUAL(pkmn_item_list_add(
                              key_item_pocket,
                              "Clear Bell",
                              1
                          ), PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                              key_item_pocket,
                              "Clear Bell",
                              1
                          ), PKMN_ERROR_NONE);

        TEST_ASSERT_EQUAL(pkmn_item_list_add(
                              key_item_pocket,
                              "GS Ball",
                              1
                          ), PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                              key_item_pocket,
                              "GS Ball",
                              1
                          ), PKMN_ERROR_NONE);

        TEST_ASSERT_EQUAL(pkmn_item_list_add(
                              key_item_pocket,
                              "Blue Card",
                              1
                          ), PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                              key_item_pocket,
                              "Blue Card",
                              1
                          ), PKMN_ERROR_NONE);

        TEST_ASSERT_EQUAL(pkmn_item_list_add(
                              key_item_pocket,
                              "Egg Ticket",
                              1
                          ), PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                              key_item_pocket,
                              "Egg Ticket",
                              1
                          ), PKMN_ERROR_NONE);

        TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                             key_item_pocket,
                             &num_items
                          ), PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(num_items, 0);
    } else {
        TEST_ASSERT_EQUAL(pkmn_item_list_add(
                              key_item_pocket,
                              "Clear Bell",
                              1
                          ), PKMN_ERROR_INVALID_ARGUMENT);
        TEST_ASSERT_EQUAL(pkmn_item_list_add(
                              key_item_pocket,
                              "GS Ball",
                              1
                          ), PKMN_ERROR_INVALID_ARGUMENT);
        TEST_ASSERT_EQUAL(pkmn_item_list_add(
                              key_item_pocket,
                              "Blue Card",
                              1
                          ), PKMN_ERROR_INVALID_ARGUMENT);
        TEST_ASSERT_EQUAL(pkmn_item_list_add(
                              key_item_pocket,
                              "Egg Ticket",
                              1
                          ), PKMN_ERROR_INVALID_ARGUMENT);

        TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                             key_item_pocket,
                             &num_items
                          ), PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(num_items, 0);
    }

    // Start adding and removing stuff, and make sure the numbers are accurate.
    const char* item_names[] = {
        "Bicycle",
        "Basement Key",
        "SecretPotion",
        "Mystery Egg",
        "Silver Wing",
        "Lost Item",
        "SquirtBottle",
        "Rainbow Wing"
    };
    test_item_list_add_remove(
        key_item_pocket,
        item_names,
        8
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_get_valid_items(
                          key_item_pocket,
                          &valid_items,
                          &valid_items_list_length
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT(valid_items_list_length > 0);
    pkmn_string_list_free(&valid_items, valid_items_list_length);
    TEST_ASSERT_NULL(valid_items);
}

void pkmntest_gen2_ball_pocket_test(
    pkmn_item_list_handle_t ball_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];
    size_t actual_strlen = 0;

    int capacity = 0;
    int num_items = 0;
    pkmn_string_list_t valid_items = NULL;
    size_t valid_items_list_length = 0;

    // Check unchanging and initial values.
    const char* expected_name = "Balls";
    TEST_ASSERT_EQUAL(pkmn_item_list_get_name(
                          ball_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(expected_name)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_game(
                          ball_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(game)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_capacity(
                          ball_pocket,
                          &capacity
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(capacity, 12);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          ball_pocket,
                          &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 0);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(ball_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        ball_pocket,
        "Master Ball"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_items[] = {
        "Potion",
        "Bicycle",
        "HM01"
    };
    test_item_list_items_from_wrong_pocket(
        ball_pocket,
        wrong_items,
        3
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    const char* item_names[] = {
        "Great Ball",
        "Poké Ball",
        "Park Ball",
        "Fast Ball",
        "Master Ball",
        "Friend Ball",
        "Love Ball",
        "Level Ball"
    };
    test_item_list_add_remove(
        ball_pocket,
        item_names,
        8
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_get_valid_items(
                          ball_pocket,
                          &valid_items,
                          &valid_items_list_length
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT(valid_items_list_length > 0);
    pkmn_string_list_free(&valid_items, valid_items_list_length);
    TEST_ASSERT_NULL(valid_items);
}

void pkmntest_gen2_tmhm_pocket_test(
    pkmn_item_list_handle_t tmhm_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];
    size_t actual_strlen = 0;

    int capacity = 0;
    int num_items = 0;
    pkmn_item_slots_t item_slots = NULL;
    size_t list_length = 0;
    pkmn_string_list_t valid_items = NULL;
    size_t valid_items_list_length = 0;

    // Check unchanging and initial values.
    const char* expected_name = "TM/HM";
    TEST_ASSERT_EQUAL(pkmn_item_list_get_name(
                          tmhm_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(expected_name)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_game(
                          tmhm_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(game)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_capacity(
                          tmhm_pocket,
                          &capacity
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(capacity, 57);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          tmhm_pocket,
                          &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 0);

    // Make sure item slots start as correctly empty.
    TEST_ASSERT_EQUAL(pkmn_item_list_as_array(
                          tmhm_pocket,
                          &item_slots,
                          &list_length
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(list_length, 57);
    for(int i = 1; i <= 50; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "TM%02d", i);
        TEST_ASSERT_EQUAL_STRING(item_slots[i-1].item, name);
        TEST_ASSERT_EQUAL(item_slots[i-1].amount, 0);
    }
    for(int i = 1; i <= 7; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "HM%02d", i);
        TEST_ASSERT_EQUAL_STRING(item_slots[50+i-1].item, name);
        TEST_ASSERT_EQUAL(item_slots[50+i-1].amount, 0);
    }
    pkmn_item_slots_free(&item_slots, list_length);
    TEST_ASSERT_NULL(item_slots);

    // Make sure we can't add items from other pockets.
    const char* wrong_items[] = {
        "Potion",
        "Master Ball",
        "Bicycle"
    };
    test_item_list_items_from_wrong_pocket(
        tmhm_pocket,
        wrong_items,
        3
    );

    /*
     * Start adding and removing stuff, and make sure the numbers are accurate.
     *
     * While this is supposed to match the other pkmntest libraries, there are
     * so many allocations done with string lists that we'll just do it after
     * each set of commands.
     */

    for(int i = 1; i <= 50; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "TM%02d", i);
        TEST_ASSERT_EQUAL(pkmn_item_list_add(
                              tmhm_pocket,
                              name,
                              50
                          ), PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                              tmhm_pocket,
                              &num_items
                          ), PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(num_items, i);
    }
    TEST_ASSERT_EQUAL(pkmn_item_list_as_array(
                          tmhm_pocket,
                          &item_slots,
                          &list_length
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(list_length, 57);
    for(int i = 1; i <= 50; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "TM%02d", i);

        TEST_ASSERT_EQUAL_STRING(item_slots[i-1].item, name);
        TEST_ASSERT_EQUAL(item_slots[i-1].amount, 50);
    }
    pkmn_item_slots_free(&item_slots, list_length);
    TEST_ASSERT_NULL(item_slots);

    for(int i = 50; i >= 1; --i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "TM%02d", i);
        TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                              tmhm_pocket,
                              name,
                              50
                          ), PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                              tmhm_pocket,
                              &num_items
                          ), PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(num_items, i-1);
    }
    TEST_ASSERT_EQUAL(pkmn_item_list_as_array(
                          tmhm_pocket,
                          &item_slots,
                          &list_length
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(list_length, 57);
    for(int i = 1; i <= 50; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "TM%02d", i);

        TEST_ASSERT_EQUAL_STRING(item_slots[i-1].item, name);
        TEST_ASSERT_EQUAL(item_slots[i-1].amount, 0);
    }
    pkmn_item_slots_free(&item_slots, list_length);
    TEST_ASSERT_NULL(item_slots);

    for(int i = 1; i <= 7; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "HM%02d", i);
        TEST_ASSERT_EQUAL(pkmn_item_list_add(
                              tmhm_pocket,
                              name,
                              50
                          ), PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                              tmhm_pocket,
                              &num_items
                          ), PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(num_items, i);
    }
    TEST_ASSERT_EQUAL(pkmn_item_list_as_array(
                          tmhm_pocket,
                          &item_slots,
                          &list_length
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(list_length, 57);
    for(int i = 1; i <= 7; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "HM%02d", i);

        TEST_ASSERT_EQUAL_STRING(item_slots[50+i-1].item, name);
        TEST_ASSERT_EQUAL(item_slots[50+i-1].amount, 50);
    }
    pkmn_item_slots_free(&item_slots, list_length);
    TEST_ASSERT_NULL(item_slots);

    for(int i = 7; i >= 1; --i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "HM%02d", i);
        TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                              tmhm_pocket,
                              name,
                              50
                          ), PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                              tmhm_pocket,
                              &num_items
                          ), PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(num_items, i-1);
    }
    TEST_ASSERT_EQUAL(pkmn_item_list_as_array(
                          tmhm_pocket,
                          &item_slots,
                          &list_length
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(list_length, 57);
    for(int i = 1; i <= 7; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "HM%02d", i);

        TEST_ASSERT_EQUAL_STRING(item_slots[50+i-1].item, name);
        TEST_ASSERT_EQUAL(item_slots[50+i-1].amount, 0);
    }
    pkmn_item_slots_free(&item_slots, list_length);
    TEST_ASSERT_NULL(item_slots);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_valid_items(
                          tmhm_pocket,
                          &valid_items,
                          &valid_items_list_length
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(valid_items_list_length, 57);
    pkmn_string_list_free(&valid_items, valid_items_list_length);
    TEST_ASSERT_NULL(valid_items);
}

void pkmntest_gen2_pc_test(
    pkmn_item_list_handle_t pc,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];
    size_t actual_strlen = 0;

    int capacity = 0;
    int num_items = 0;
    pkmn_string_list_t full_item_list = NULL;
    size_t list_length = 0;
    pkmn_string_list_t valid_items = NULL;
    size_t valid_items_list_length = 0;

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

    TEST_ASSERT_EQUAL(pkmn_item_list_get_capacity(
                          pc,
                          &capacity
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(capacity, 50);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          pc,
                          &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 0);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(pc);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        pc,
        "Potion"
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    const char* item_names[] = {
        "Potion",
        "Bicycle",
        "Great Ball",
        "TM28",
        "Berry",
        "SquirtBottle",
        "Friend Ball",
        "HM01"
    };
    test_item_list_add_remove(
        pc,
        item_names,
        8
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_get_valid_items(
                          pc,
                          &valid_items,
                          &valid_items_list_length
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_database_item_list(
                          game,
                          &full_item_list,
                          &list_length
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(list_length, valid_items_list_length);

    pkmn_string_list_free(&full_item_list, list_length);
    TEST_ASSERT_NULL(full_item_list);

    pkmn_string_list_free(&valid_items, valid_items_list_length);
    TEST_ASSERT_NULL(valid_items);
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
