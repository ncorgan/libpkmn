/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest-c/gen1_items_tests.h>

#include <unity.h>

#define STRBUFFER_LEN 1024

static void pkmntest_gen1_item_list_common(
    pkmn_item_list_handle_t items
) {
    /*
     * Confirm expected error codes are returned.
     */
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          items, "Potion", 0
                      ), PKMN_ERROR_OUT_OF_RANGE);
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          items, "Potion", 100
                      ), PKMN_ERROR_OUT_OF_RANGE);
    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          items, "Potion", 0
                      ), PKMN_ERROR_OUT_OF_RANGE);
    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          items, "Potion", 100
                      ), PKMN_ERROR_OUT_OF_RANGE);

    /*
     * Start adding and removing stuff, and make sure
     * the numbers are accurate.
     */
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          items, "Potion", 30
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          items, "Great Ball", 99
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_item_list_add(
                          items, "Ether", 1
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          items, "Great Ball", 20
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_item_list_move(
                          items, 0, 1
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_item_list_remove(
                          items, "Potion", 30
                      ), PKMN_ERROR_NONE);

    int num_items = 0;
    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          items, &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 2);

    pkmn_item_slots_t item_slots = NULL;
    size_t num_slots = 0;
    TEST_ASSERT_EQUAL(pkmn_item_list_as_array(
                          items, &item_slots, &num_slots
                      ), PKMN_ERROR_NONE);

    TEST_ASSERT_EQUAL_STRING(item_slots[0].item, "Great Ball");
    TEST_ASSERT_EQUAL(item_slots[0].amount, 79);
    TEST_ASSERT_EQUAL_STRING(item_slots[1].item, "Ether");
    TEST_ASSERT_EQUAL(item_slots[1].amount, 1);

    pkmn_item_slots_free(&item_slots, num_slots);
    TEST_ASSERT_NULL(item_slots);
}

void pkmntest_gen1_item_list_test(
    pkmn_item_list_handle_t items
) {
    char strbuffer[STRBUFFER_LEN];
    size_t actual_strlen;

    TEST_ASSERT_EQUAL(pkmn_item_list_get_name(
                          items,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Items");
    TEST_ASSERT_EQUAL(actual_strlen, 5);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_game(
                          items,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Red");
    TEST_ASSERT_EQUAL(actual_strlen, 3);

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

    pkmntest_gen1_item_list_common(items);
}

void pkmntest_gen1_pc_test(
    pkmn_item_list_handle_t pc
) {
    char strbuffer[STRBUFFER_LEN];
    size_t actual_strlen;

    TEST_ASSERT_EQUAL(pkmn_item_list_get_name(
                          pc,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "PC");
    TEST_ASSERT_EQUAL(actual_strlen, 2);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_game(
                          pc,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Red");
    TEST_ASSERT_EQUAL(actual_strlen, 3);

    int capacity = 0;
    TEST_ASSERT_EQUAL(pkmn_item_list_get_capacity(
                          pc, &capacity
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(capacity, 50);

    int num_pc = 0;
    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          pc, &num_pc
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_pc, 0);

    pkmntest_gen1_item_list_common(pc);
}

void pkmntest_gen1_item_bag_test(
    pkmn_item_bag_handle_t bag
) {
    pkmn_string_list_t pocket_names = NULL;
    size_t num_pockets = 0;

    TEST_ASSERT_EQUAL(pkmn_item_bag_get_pocket_names(
                          bag, &pocket_names, &num_pockets
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_pockets, 1);

    pkmn_string_list_free(&pocket_names, num_pockets);
    TEST_ASSERT_NULL(pocket_names);

    pkmn_item_list_handle_t items;
    TEST_ASSERT_EQUAL(pkmn_item_bag_get_pocket(
                          bag, "Items", &items
                      ), PKMN_ERROR_NONE);

    pkmntest_gen1_item_list_test(items);

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(items);
}