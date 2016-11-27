/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmntest-c/gen1_items_tests.h>
#include <pkmntest-c/gen2_items_tests.h>

#include <pkmn.h>

// Red

static void gen1_red_item_list_test() {
    pkmn_item_list_handle_t items = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &items, "Items", "Red"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen1_item_list_test(items, "Red");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(items);
}

static void gen1_red_item_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &pc, "PC", "Red"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen1_pc_test(pc, "Red");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &pc
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pc);
}

static void gen1_red_item_bag_test() {
    pkmn_item_bag_handle_t bag = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_bag_make(
                          &bag, "Red"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen1_item_bag_test(bag, "Red");

    TEST_ASSERT_EQUAL(pkmn_item_bag_free(
                          &bag
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(bag);
}

// Blue

static void gen1_blue_item_list_test() {
    pkmn_item_list_handle_t items = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &items, "Items", "Blue"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen1_item_list_test(items, "Blue");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(items);
}

static void gen1_blue_item_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &pc, "PC", "Blue"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen1_pc_test(pc, "Blue");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &pc
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pc);
}

static void gen1_blue_item_bag_test() {
    pkmn_item_bag_handle_t bag = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_bag_make(
                          &bag, "Blue"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen1_item_bag_test(bag, "Blue");

    TEST_ASSERT_EQUAL(pkmn_item_bag_free(
                          &bag
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(bag);
}

// Yellow

static void gen1_yellow_item_list_test() {
    pkmn_item_list_handle_t items = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &items, "Items", "Yellow"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen1_item_list_test(items, "Yellow");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(items);
}

static void gen1_yellow_item_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &pc, "PC", "Yellow"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen1_pc_test(pc, "Yellow");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &pc
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pc);
}

static void gen1_yellow_item_bag_test() {
    pkmn_item_bag_handle_t bag = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_bag_make(
                          &bag, "Yellow"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen1_item_bag_test(bag, "Yellow");

    TEST_ASSERT_EQUAL(pkmn_item_bag_free(
                          &bag
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(bag);
}

// Gold

static void gen2_gold_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &item_pocket, "Items", "Gold"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_item_pocket_test(item_pocket, "Gold");
}

static void gen2_gold_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &key_item_pocket, "KeyItems", "Gold"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_key_item_pocket_test(key_item_pocket, "Gold");
}

static void gen2_gold_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &ball_pocket, "Balls", "Gold"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_ball_pocket_test(ball_pocket, "Gold");
}

static void gen2_gold_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &tmhm_pocket, "TM/HM", "Gold"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_tmhm_pocket_test(tmhm_pocket, "Gold");
}

static void gen2_gold_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &pc, "PC", "Gold"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_pc_test(pc, "Gold");
}

static void gen2_gold_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_bag_make(
                          &item_bag, "Gold"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_item_bag_test(item_bag, "Gold");
}

// Silver

static void gen2_silver_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &item_pocket, "Items", "Silver"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_item_pocket_test(item_pocket, "Silver");
}

static void gen2_silver_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &key_item_pocket, "KeyItems", "Silver"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_key_item_pocket_test(key_item_pocket, "Silver");
}

static void gen2_silver_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &ball_pocket, "Balls", "Silver"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_ball_pocket_test(ball_pocket, "Silver");
}

static void gen2_silver_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &tmhm_pocket, "TM/HM", "Silver"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_tmhm_pocket_test(tmhm_pocket, "Silver");
}

static void gen2_silver_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &pc, "PC", "Silver"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_pc_test(pc, "Silver");
}

static void gen2_silver_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_bag_make(
                          &item_bag, "Silver"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_item_bag_test(item_bag, "Silver");
}

// Crystal

static void gen2_crystal_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &item_pocket, "Items", "Crystal"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_item_pocket_test(item_pocket, "Crystal");
}

static void gen2_crystal_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &key_item_pocket, "KeyItems", "Crystal"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_key_item_pocket_test(key_item_pocket, "Crystal");
}

static void gen2_crystal_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &ball_pocket, "Balls", "Crystal"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_ball_pocket_test(ball_pocket, "Crystal");
}

static void gen2_crystal_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &tmhm_pocket, "TM/HM", "Crystal"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_tmhm_pocket_test(tmhm_pocket, "Crystal");
}

static void gen2_crystal_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &pc, "PC", "Crystal"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_pc_test(pc, "Crystal");
}

static void gen2_crystal_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_bag_make(
                          &item_bag, "Crystal"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_item_bag_test(item_bag, "Crystal");
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(gen1_red_item_list_test)
    PKMN_C_TEST(gen1_red_item_pc_test)
    PKMN_C_TEST(gen1_red_item_bag_test)

    PKMN_C_TEST(gen1_blue_item_list_test)
    PKMN_C_TEST(gen1_blue_item_pc_test)
    PKMN_C_TEST(gen1_blue_item_bag_test)

    PKMN_C_TEST(gen1_yellow_item_list_test)
    PKMN_C_TEST(gen1_yellow_item_pc_test)
    PKMN_C_TEST(gen1_yellow_item_bag_test)

    PKMN_C_TEST(gen2_gold_item_pocket_test)
    PKMN_C_TEST(gen2_gold_key_item_pocket_test)
    PKMN_C_TEST(gen2_gold_ball_pocket_test)
    PKMN_C_TEST(gen2_gold_tmhm_pocket_test)
    PKMN_C_TEST(gen2_gold_pc_test)
    PKMN_C_TEST(gen2_gold_item_bag_test)

    PKMN_C_TEST(gen2_silver_item_pocket_test)
    PKMN_C_TEST(gen2_silver_key_item_pocket_test)
    PKMN_C_TEST(gen2_silver_ball_pocket_test)
    PKMN_C_TEST(gen2_silver_tmhm_pocket_test)
    PKMN_C_TEST(gen2_silver_pc_test)
    PKMN_C_TEST(gen2_silver_item_bag_test)

    PKMN_C_TEST(gen2_crystal_item_pocket_test)
    PKMN_C_TEST(gen2_crystal_key_item_pocket_test)
    PKMN_C_TEST(gen2_crystal_ball_pocket_test)
    PKMN_C_TEST(gen2_crystal_tmhm_pocket_test)
    PKMN_C_TEST(gen2_crystal_pc_test)
    PKMN_C_TEST(gen2_crystal_item_bag_test)
)
