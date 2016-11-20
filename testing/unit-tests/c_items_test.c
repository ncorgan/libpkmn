/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmntest-c/gen1_items_tests.h>

#include <pkmn.h>

static void gen1_item_list_test() {
    pkmn_item_list_handle_t items = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &items, "Items", "Red"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen1_item_list_test(items);

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(items);
}

static void gen1_item_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &pc, "PC", "Red"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen1_pc_test(pc);

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &pc
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pc);
}

static void gen1_item_bag_test() {
    pkmn_item_bag_handle_t bag = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_bag_make(
                          &bag, "Red"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen1_item_bag_test(bag);

    TEST_ASSERT_EQUAL(pkmn_item_bag_free(
                          &bag
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(bag);
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(gen1_item_list_test)
    PKMN_C_TEST(gen1_item_pc_test)
    PKMN_C_TEST(gen1_item_bag_test)
)
