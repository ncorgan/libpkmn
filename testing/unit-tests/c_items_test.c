/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmntest-c/gen1_items_tests.h>
#include <pkmntest-c/gen2_items_tests.h>
#include <pkmntest-c/gba_items_tests.h>

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

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_pocket);
}

static void gen2_gold_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &key_item_pocket, "KeyItems", "Gold"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_key_item_pocket_test(key_item_pocket, "Gold");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &key_item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gen2_gold_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &ball_pocket, "Balls", "Gold"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_ball_pocket_test(ball_pocket, "Gold");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &ball_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(ball_pocket);
}

static void gen2_gold_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &tmhm_pocket, "TM/HM", "Gold"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_tmhm_pocket_test(tmhm_pocket, "Gold");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &tmhm_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gen2_gold_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &pc, "PC", "Gold"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_pc_test(pc, "Gold");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &pc
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pc);
}

static void gen2_gold_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_bag_make(
                          &item_bag, "Gold"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_item_bag_test(item_bag, "Gold");

    TEST_ASSERT_EQUAL(pkmn_item_bag_free(
                          &item_bag
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_bag);
}

// Silver

static void gen2_silver_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &item_pocket, "Items", "Silver"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_item_pocket_test(item_pocket, "Silver");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_pocket);
}

static void gen2_silver_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &key_item_pocket, "KeyItems", "Silver"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_key_item_pocket_test(key_item_pocket, "Silver");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &key_item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gen2_silver_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &ball_pocket, "Balls", "Silver"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_ball_pocket_test(ball_pocket, "Silver");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &ball_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(ball_pocket);
}

static void gen2_silver_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &tmhm_pocket, "TM/HM", "Silver"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_tmhm_pocket_test(tmhm_pocket, "Silver");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &tmhm_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gen2_silver_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &pc, "PC", "Silver"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_pc_test(pc, "Silver");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &pc
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pc);
}

static void gen2_silver_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_bag_make(
                          &item_bag, "Silver"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_item_bag_test(item_bag, "Silver");

    TEST_ASSERT_EQUAL(pkmn_item_bag_free(
                          &item_bag
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_bag);
}

// Crystal

static void gen2_crystal_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &item_pocket, "Items", "Crystal"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_item_pocket_test(item_pocket, "Crystal");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_pocket);
}

static void gen2_crystal_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &key_item_pocket, "KeyItems", "Crystal"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_key_item_pocket_test(key_item_pocket, "Crystal");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &key_item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gen2_crystal_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &ball_pocket, "Balls", "Crystal"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_ball_pocket_test(ball_pocket, "Crystal");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &ball_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(ball_pocket);
}

static void gen2_crystal_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &tmhm_pocket, "TM/HM", "Crystal"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_tmhm_pocket_test(tmhm_pocket, "Crystal");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &tmhm_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gen2_crystal_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &pc, "PC", "Crystal"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_pc_test(pc, "Crystal");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &pc
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pc);
}

static void gen2_crystal_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_bag_make(
                          &item_bag, "Crystal"
                      ), PKMN_ERROR_NONE);

    pkmntest_gen2_item_bag_test(item_bag, "Crystal");

    TEST_ASSERT_EQUAL(pkmn_item_bag_free(
                          &item_bag
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_bag);
}

// Ruby

static void gba_ruby_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &item_pocket, "Items", "Ruby"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_item_pocket_test(item_pocket, "Ruby");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_pocket);
}

static void gba_ruby_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &key_item_pocket, "Key Items", "Ruby"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_key_item_pocket_test(key_item_pocket, "Ruby");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &key_item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gba_ruby_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &ball_pocket, "Poké Balls", "Ruby"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_ball_pocket_test(ball_pocket, "Ruby");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &ball_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(ball_pocket);
}

static void gba_ruby_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &tmhm_pocket, "TMs & HMs", "Ruby"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_tmhm_pocket_test(tmhm_pocket, "Ruby");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &tmhm_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gba_ruby_berry_pocket_test() {
    pkmn_item_list_handle_t berry_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &berry_pocket, "Berries", "Ruby"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_berry_pocket_test(berry_pocket, "Ruby");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &berry_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(berry_pocket);
}

static void gba_ruby_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &pc, "PC", "Ruby"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_pc_test(pc, "Ruby");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &pc
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pc);
}

static void gba_ruby_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_bag_make(
                          &item_bag, "Ruby"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_item_bag_test(item_bag, "Ruby");

    TEST_ASSERT_EQUAL(pkmn_item_bag_free(
                          &item_bag
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_bag);
}

// Sapphire

static void gba_sapphire_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &item_pocket, "Items", "Sapphire"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_item_pocket_test(item_pocket, "Sapphire");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_pocket);
}

static void gba_sapphire_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &key_item_pocket, "Key Items", "Sapphire"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_key_item_pocket_test(key_item_pocket, "Sapphire");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &key_item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gba_sapphire_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &ball_pocket, "Poké Balls", "Sapphire"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_ball_pocket_test(ball_pocket, "Sapphire");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &ball_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(ball_pocket);
}

static void gba_sapphire_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &tmhm_pocket, "TMs & HMs", "Sapphire"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_tmhm_pocket_test(tmhm_pocket, "Sapphire");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &tmhm_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gba_sapphire_berry_pocket_test() {
    pkmn_item_list_handle_t berry_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &berry_pocket, "Berries", "Sapphire"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_berry_pocket_test(berry_pocket, "Sapphire");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &berry_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(berry_pocket);
}

static void gba_sapphire_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &pc, "PC", "Sapphire"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_pc_test(pc, "Sapphire");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &pc
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pc);
}

static void gba_sapphire_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_bag_make(
                          &item_bag, "Sapphire"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_item_bag_test(item_bag, "Sapphire");

    TEST_ASSERT_EQUAL(pkmn_item_bag_free(
                          &item_bag
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_bag);
}

// Emerald

static void gba_emerald_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &item_pocket, "Items", "Emerald"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_item_pocket_test(item_pocket, "Emerald");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_pocket);
}

static void gba_emerald_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &key_item_pocket, "Key Items", "Emerald"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_key_item_pocket_test(key_item_pocket, "Emerald");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &key_item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gba_emerald_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &ball_pocket, "Poké Balls", "Emerald"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_ball_pocket_test(ball_pocket, "Emerald");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &ball_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(ball_pocket);
}

static void gba_emerald_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &tmhm_pocket, "TMs & HMs", "Emerald"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_tmhm_pocket_test(tmhm_pocket, "Emerald");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &tmhm_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gba_emerald_berry_pocket_test() {
    pkmn_item_list_handle_t berry_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &berry_pocket, "Berries", "Emerald"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_berry_pocket_test(berry_pocket, "Emerald");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &berry_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(berry_pocket);
}

static void gba_emerald_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &pc, "PC", "Emerald"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_pc_test(pc, "Emerald");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &pc
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pc);
}

static void gba_emerald_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_bag_make(
                          &item_bag, "Emerald"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_item_bag_test(item_bag, "Emerald");

    TEST_ASSERT_EQUAL(pkmn_item_bag_free(
                          &item_bag
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_bag);
}

// FireRed

static void gba_firered_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &item_pocket, "Items", "FireRed"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_item_pocket_test(item_pocket, "FireRed");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_pocket);
}

static void gba_firered_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &key_item_pocket, "Key Items", "FireRed"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_key_item_pocket_test(key_item_pocket, "FireRed");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &key_item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gba_firered_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &ball_pocket, "Poké Balls", "FireRed"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_ball_pocket_test(ball_pocket, "FireRed");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &ball_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(ball_pocket);
}

static void gba_firered_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &tmhm_pocket, "TM Case", "FireRed"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_tmhm_pocket_test(tmhm_pocket, "FireRed");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &tmhm_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gba_firered_berry_pocket_test() {
    pkmn_item_list_handle_t berry_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &berry_pocket, "Berry Pouch", "FireRed"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_berry_pocket_test(berry_pocket, "FireRed");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &berry_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(berry_pocket);
}

static void gba_firered_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &pc, "PC", "FireRed"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_pc_test(pc, "FireRed");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &pc
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pc);
}

static void gba_firered_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_bag_make(
                          &item_bag, "FireRed"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_item_bag_test(item_bag, "FireRed");

    TEST_ASSERT_EQUAL(pkmn_item_bag_free(
                          &item_bag
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_bag);
}

// LeafGreen

static void gba_leafgreen_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &item_pocket, "Items", "LeafGreen"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_item_pocket_test(item_pocket, "LeafGreen");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_pocket);
}

static void gba_leafgreen_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &key_item_pocket, "Key Items", "LeafGreen"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_key_item_pocket_test(key_item_pocket, "LeafGreen");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &key_item_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gba_leafgreen_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &ball_pocket, "Poké Balls", "LeafGreen"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_ball_pocket_test(ball_pocket, "LeafGreen");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &ball_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(ball_pocket);
}

static void gba_leafgreen_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &tmhm_pocket, "TM Case", "LeafGreen"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_tmhm_pocket_test(tmhm_pocket, "LeafGreen");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &tmhm_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gba_leafgreen_berry_pocket_test() {
    pkmn_item_list_handle_t berry_pocket = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &berry_pocket, "Berry Pouch", "LeafGreen"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_berry_pocket_test(berry_pocket, "LeafGreen");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &berry_pocket
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(berry_pocket);
}

static void gba_leafgreen_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_list_make(
                          &pc, "PC", "LeafGreen"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_pc_test(pc, "LeafGreen");

    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &pc
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pc);
}

static void gba_leafgreen_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(pkmn_item_bag_make(
                          &item_bag, "LeafGreen"
                      ), PKMN_ERROR_NONE);

    pkmntest_gba_item_bag_test(item_bag, "LeafGreen");

    TEST_ASSERT_EQUAL(pkmn_item_bag_free(
                          &item_bag
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_bag);
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

    PKMN_C_TEST(gba_ruby_item_pocket_test)
    PKMN_C_TEST(gba_ruby_key_item_pocket_test)
    PKMN_C_TEST(gba_ruby_ball_pocket_test)
    PKMN_C_TEST(gba_ruby_tmhm_pocket_test)
    PKMN_C_TEST(gba_ruby_berry_pocket_test)
    PKMN_C_TEST(gba_ruby_pc_test)
    PKMN_C_TEST(gba_ruby_item_bag_test)

    PKMN_C_TEST(gba_sapphire_item_pocket_test)
    PKMN_C_TEST(gba_sapphire_key_item_pocket_test)
    PKMN_C_TEST(gba_sapphire_ball_pocket_test)
    PKMN_C_TEST(gba_sapphire_tmhm_pocket_test)
    PKMN_C_TEST(gba_sapphire_berry_pocket_test)
    PKMN_C_TEST(gba_sapphire_pc_test)
    PKMN_C_TEST(gba_sapphire_item_bag_test)

    PKMN_C_TEST(gba_emerald_item_pocket_test)
    PKMN_C_TEST(gba_emerald_key_item_pocket_test)
    PKMN_C_TEST(gba_emerald_ball_pocket_test)
    PKMN_C_TEST(gba_emerald_tmhm_pocket_test)
    PKMN_C_TEST(gba_emerald_berry_pocket_test)
    PKMN_C_TEST(gba_emerald_pc_test)
    PKMN_C_TEST(gba_emerald_item_bag_test)

    PKMN_C_TEST(gba_firered_item_pocket_test)
    PKMN_C_TEST(gba_firered_key_item_pocket_test)
    PKMN_C_TEST(gba_firered_ball_pocket_test)
    PKMN_C_TEST(gba_firered_tmhm_pocket_test)
    PKMN_C_TEST(gba_firered_berry_pocket_test)
    PKMN_C_TEST(gba_firered_pc_test)
    PKMN_C_TEST(gba_firered_item_bag_test)

    PKMN_C_TEST(gba_leafgreen_item_pocket_test)
    PKMN_C_TEST(gba_leafgreen_key_item_pocket_test)
    PKMN_C_TEST(gba_leafgreen_ball_pocket_test)
    PKMN_C_TEST(gba_leafgreen_tmhm_pocket_test)
    PKMN_C_TEST(gba_leafgreen_berry_pocket_test)
    PKMN_C_TEST(gba_leafgreen_pc_test)
    PKMN_C_TEST(gba_leafgreen_item_bag_test)
)
