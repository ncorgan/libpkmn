/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include "c_items_test.h"

#include <pkmn.h>

// Red

static void gen1_red_item_list_test() {
    pkmn_item_list_handle_t items = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &items, "Items", "Red"
        )
    );

    gen1_item_list_test(items, "Red");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &items
        )
    );
    TEST_ASSERT_NULL(items);
}

static void gen1_red_item_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &pc, "PC", "Red"
        )
    );

    gen1_pc_test(pc, "Red");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &pc
        )
    );
    TEST_ASSERT_NULL(pc);
}

static void gen1_red_item_bag_test() {
    pkmn_item_bag_handle_t bag = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_make(
            &bag, "Red"
        )
    );

    gen1_item_bag_test(bag, "Red");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_free(
            &bag
        )
    );
    TEST_ASSERT_NULL(bag);
}

// Blue

static void gen1_blue_item_list_test() {
    pkmn_item_list_handle_t items = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &items, "Items", "Blue"
        )
    );

    gen1_item_list_test(items, "Blue");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &items
        )
    );
    TEST_ASSERT_NULL(items);
}

static void gen1_blue_item_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &pc, "PC", "Blue"
        )
    );

    gen1_pc_test(pc, "Blue");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &pc
        )
    );
    TEST_ASSERT_NULL(pc);
}

static void gen1_blue_item_bag_test() {
    pkmn_item_bag_handle_t bag = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_make(
            &bag, "Blue"
        )
    );

    gen1_item_bag_test(bag, "Blue");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_free(
            &bag
        )
    );
    TEST_ASSERT_NULL(bag);
}

// Yellow

static void gen1_yellow_item_list_test() {
    pkmn_item_list_handle_t items = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &items, "Items", "Yellow"
        )
    );

    gen1_item_list_test(items, "Yellow");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &items
        )
    );
    TEST_ASSERT_NULL(items);
}

static void gen1_yellow_item_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &pc, "PC", "Yellow"
        )
    );

    gen1_pc_test(pc, "Yellow");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &pc
        )
    );
    TEST_ASSERT_NULL(pc);
}

static void gen1_yellow_item_bag_test() {
    pkmn_item_bag_handle_t bag = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_make(
            &bag, "Yellow"
        )
    );

    gen1_item_bag_test(bag, "Yellow");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_free(
            &bag
        )
    );
    TEST_ASSERT_NULL(bag);
}

// Gold

static void gen2_gold_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &item_pocket, "Items", "Gold"
        )
    );

    gen2_item_pocket_test(item_pocket, "Gold");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &item_pocket
        )
    );
    TEST_ASSERT_NULL(item_pocket);
}

static void gen2_gold_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &key_item_pocket, "KeyItems", "Gold"
        )
    );

    gen2_key_item_pocket_test(key_item_pocket, "Gold");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &key_item_pocket
        )
    );
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gen2_gold_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &ball_pocket, "Balls", "Gold"
        )
    );

    gen2_ball_pocket_test(ball_pocket, "Gold");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &ball_pocket
        )
    );
    TEST_ASSERT_NULL(ball_pocket);
}

static void gen2_gold_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &tmhm_pocket, "TM/HM", "Gold"
        )
    );

    gen2_tmhm_pocket_test(tmhm_pocket, "Gold");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &tmhm_pocket
        )
    );
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gen2_gold_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &pc, "PC", "Gold"
        )
    );

    gen2_pc_test(pc, "Gold");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &pc
        )
    );
    TEST_ASSERT_NULL(pc);
}

static void gen2_gold_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_make(
            &item_bag, "Gold"
        )
    );

    gen2_item_bag_test(item_bag, "Gold");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_free(
            &item_bag
        )
    );
    TEST_ASSERT_NULL(item_bag);
}

// Silver

static void gen2_silver_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &item_pocket, "Items", "Silver"
        )
    );

    gen2_item_pocket_test(item_pocket, "Silver");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &item_pocket
        )
    );
    TEST_ASSERT_NULL(item_pocket);
}

static void gen2_silver_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &key_item_pocket, "KeyItems", "Silver"
        )
    );

    gen2_key_item_pocket_test(key_item_pocket, "Silver");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &key_item_pocket
        )
    );
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gen2_silver_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &ball_pocket, "Balls", "Silver"
        )
    );

    gen2_ball_pocket_test(ball_pocket, "Silver");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &ball_pocket
        )
    );
    TEST_ASSERT_NULL(ball_pocket);
}

static void gen2_silver_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &tmhm_pocket, "TM/HM", "Silver"
        )
    );

    gen2_tmhm_pocket_test(tmhm_pocket, "Silver");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &tmhm_pocket
        )
    );
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gen2_silver_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &pc, "PC", "Silver"
        )
    );

    gen2_pc_test(pc, "Silver");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &pc
        )
    );
    TEST_ASSERT_NULL(pc);
}

static void gen2_silver_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_make(
            &item_bag, "Silver"
        )
    );

    gen2_item_bag_test(item_bag, "Silver");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_free(
            &item_bag
        )
    );
    TEST_ASSERT_NULL(item_bag);
}

// Crystal

static void gen2_crystal_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &item_pocket, "Items", "Crystal"
        )
    );

    gen2_item_pocket_test(item_pocket, "Crystal");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &item_pocket
        )
    );
    TEST_ASSERT_NULL(item_pocket);
}

static void gen2_crystal_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &key_item_pocket, "KeyItems", "Crystal"
        )
    );

    gen2_key_item_pocket_test(key_item_pocket, "Crystal");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &key_item_pocket
        )
    );
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gen2_crystal_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &ball_pocket, "Balls", "Crystal"
        )
    );

    gen2_ball_pocket_test(ball_pocket, "Crystal");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &ball_pocket
        )
    );
    TEST_ASSERT_NULL(ball_pocket);
}

static void gen2_crystal_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &tmhm_pocket, "TM/HM", "Crystal"
        )
    );

    gen2_tmhm_pocket_test(tmhm_pocket, "Crystal");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &tmhm_pocket
        )
    );
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gen2_crystal_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &pc, "PC", "Crystal"
        )
    );

    gen2_pc_test(pc, "Crystal");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &pc
        )
    );
    TEST_ASSERT_NULL(pc);
}

static void gen2_crystal_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_make(
            &item_bag, "Crystal"
        )
    );

    gen2_item_bag_test(item_bag, "Crystal");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_free(
            &item_bag
        )
    );
    TEST_ASSERT_NULL(item_bag);
}

// Ruby

static void gba_ruby_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &item_pocket, "Items", "Ruby"
        )
    );

    gba_item_pocket_test(item_pocket, "Ruby");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &item_pocket
        )
    );
    TEST_ASSERT_NULL(item_pocket);
}

static void gba_ruby_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &key_item_pocket, "Key Items", "Ruby"
        )
    );

    gba_key_item_pocket_test(key_item_pocket, "Ruby");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &key_item_pocket
        )
    );
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gba_ruby_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &ball_pocket, "Poké Balls", "Ruby"
        )
    );

    gba_ball_pocket_test(ball_pocket, "Ruby");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &ball_pocket
        )
    );
    TEST_ASSERT_NULL(ball_pocket);
}

static void gba_ruby_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &tmhm_pocket, "TMs & HMs", "Ruby"
        )
    );

    gba_tmhm_pocket_test(tmhm_pocket, "Ruby");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &tmhm_pocket
        )
    );
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gba_ruby_berry_pocket_test() {
    pkmn_item_list_handle_t berry_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &berry_pocket, "Berries", "Ruby"
        )
    );

    gba_berry_pocket_test(berry_pocket, "Ruby");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &berry_pocket
        )
    );
    TEST_ASSERT_NULL(berry_pocket);
}

static void gba_ruby_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &pc, "PC", "Ruby"
        )
    );

    gba_pc_test(pc, "Ruby");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &pc
        )
    );
    TEST_ASSERT_NULL(pc);
}

static void gba_ruby_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_make(
            &item_bag, "Ruby"
        )
    );

    gba_item_bag_test(item_bag, "Ruby");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_free(
            &item_bag
        )
    );
    TEST_ASSERT_NULL(item_bag);
}

// Sapphire

static void gba_sapphire_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &item_pocket, "Items", "Sapphire"
        )
    );

    gba_item_pocket_test(item_pocket, "Sapphire");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &item_pocket
        )
    );
    TEST_ASSERT_NULL(item_pocket);
}

static void gba_sapphire_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &key_item_pocket, "Key Items", "Sapphire"
        )
    );

    gba_key_item_pocket_test(key_item_pocket, "Sapphire");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &key_item_pocket
        )
    );
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gba_sapphire_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &ball_pocket, "Poké Balls", "Sapphire"
        )
    );

    gba_ball_pocket_test(ball_pocket, "Sapphire");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &ball_pocket
        )
    );
    TEST_ASSERT_NULL(ball_pocket);
}

static void gba_sapphire_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &tmhm_pocket, "TMs & HMs", "Sapphire"
        )
    );

    gba_tmhm_pocket_test(tmhm_pocket, "Sapphire");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &tmhm_pocket
        )
    );
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gba_sapphire_berry_pocket_test() {
    pkmn_item_list_handle_t berry_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &berry_pocket, "Berries", "Sapphire"
        )
    );

    gba_berry_pocket_test(berry_pocket, "Sapphire");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &berry_pocket
        )
    );
    TEST_ASSERT_NULL(berry_pocket);
}

static void gba_sapphire_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &pc, "PC", "Sapphire"
        )
    );

    gba_pc_test(pc, "Sapphire");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &pc
        )
    );
    TEST_ASSERT_NULL(pc);
}

static void gba_sapphire_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_make(
            &item_bag, "Sapphire"
        )
    );

    gba_item_bag_test(item_bag, "Sapphire");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_free(
            &item_bag
        )
    );
    TEST_ASSERT_NULL(item_bag);
}

// Emerald

static void gba_emerald_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &item_pocket, "Items", "Emerald"
        )
    );

    gba_item_pocket_test(item_pocket, "Emerald");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &item_pocket
        )
    );
    TEST_ASSERT_NULL(item_pocket);
}

static void gba_emerald_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &key_item_pocket, "Key Items", "Emerald"
        )
    );

    gba_key_item_pocket_test(key_item_pocket, "Emerald");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &key_item_pocket
        )
    );
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gba_emerald_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &ball_pocket, "Poké Balls", "Emerald"
        )
    );

    gba_ball_pocket_test(ball_pocket, "Emerald");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &ball_pocket
        )
    );
    TEST_ASSERT_NULL(ball_pocket);
}

static void gba_emerald_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &tmhm_pocket, "TMs & HMs", "Emerald"
        )
    );

    gba_tmhm_pocket_test(tmhm_pocket, "Emerald");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &tmhm_pocket
        )
    );
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gba_emerald_berry_pocket_test() {
    pkmn_item_list_handle_t berry_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &berry_pocket, "Berries", "Emerald"
        )
    );

    gba_berry_pocket_test(berry_pocket, "Emerald");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &berry_pocket
        )
    );
    TEST_ASSERT_NULL(berry_pocket);
}

static void gba_emerald_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &pc, "PC", "Emerald"
        )
    );

    gba_pc_test(pc, "Emerald");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &pc
        )
    );
    TEST_ASSERT_NULL(pc);
}

static void gba_emerald_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_make(
            &item_bag, "Emerald"
        )
    );

    gba_item_bag_test(item_bag, "Emerald");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_free(
            &item_bag
        )
    );
    TEST_ASSERT_NULL(item_bag);
}

// FireRed

static void gba_firered_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &item_pocket, "Items", "FireRed"
        )
    );

    gba_item_pocket_test(item_pocket, "FireRed");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &item_pocket
        )
    );
    TEST_ASSERT_NULL(item_pocket);
}

static void gba_firered_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &key_item_pocket, "Key Items", "FireRed"
        )
    );

    gba_key_item_pocket_test(key_item_pocket, "FireRed");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &key_item_pocket
        )
    );
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gba_firered_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &ball_pocket, "Poké Balls", "FireRed"
        )
    );

    gba_ball_pocket_test(ball_pocket, "FireRed");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &ball_pocket
        )
    );
    TEST_ASSERT_NULL(ball_pocket);
}

static void gba_firered_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &tmhm_pocket, "TM Case", "FireRed"
        )
    );

    gba_tmhm_pocket_test(tmhm_pocket, "FireRed");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &tmhm_pocket
        )
    );
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gba_firered_berry_pocket_test() {
    pkmn_item_list_handle_t berry_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &berry_pocket, "Berry Pouch", "FireRed"
        )
    );

    gba_berry_pocket_test(berry_pocket, "FireRed");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &berry_pocket
        )
    );
    TEST_ASSERT_NULL(berry_pocket);
}

static void gba_firered_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &pc, "PC", "FireRed"
        )
    );

    gba_pc_test(pc, "FireRed");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &pc
        )
    );
    TEST_ASSERT_NULL(pc);
}

static void gba_firered_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_make(
            &item_bag, "FireRed"
        )
    );

    gba_item_bag_test(item_bag, "FireRed");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_free(
            &item_bag
        )
    );
    TEST_ASSERT_NULL(item_bag);
}

// LeafGreen

static void gba_leafgreen_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &item_pocket, "Items", "LeafGreen"
        )
    );

    gba_item_pocket_test(item_pocket, "LeafGreen");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &item_pocket
        )
    );
    TEST_ASSERT_NULL(item_pocket);
}

static void gba_leafgreen_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &key_item_pocket, "Key Items", "LeafGreen"
        )
    );

    gba_key_item_pocket_test(key_item_pocket, "LeafGreen");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &key_item_pocket
        )
    );
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gba_leafgreen_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &ball_pocket, "Poké Balls", "LeafGreen"
        )
    );

    gba_ball_pocket_test(ball_pocket, "LeafGreen");

    TEST_ASSERT_EQUAL(
        pkmn_item_list_free(
            &ball_pocket
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_NULL(ball_pocket);
}

static void gba_leafgreen_tmhm_pocket_test() {
    pkmn_item_list_handle_t tmhm_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &tmhm_pocket, "TM Case", "LeafGreen"
        )
    );

    gba_tmhm_pocket_test(tmhm_pocket, "LeafGreen");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &tmhm_pocket
        )
    );
    TEST_ASSERT_NULL(tmhm_pocket);
}

static void gba_leafgreen_berry_pocket_test() {
    pkmn_item_list_handle_t berry_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &berry_pocket, "Berry Pouch", "LeafGreen"
        )
    );

    gba_berry_pocket_test(berry_pocket, "LeafGreen");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &berry_pocket
        )
    );
    TEST_ASSERT_NULL(berry_pocket);
}

static void gba_leafgreen_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &pc, "PC", "LeafGreen"
        )
    );

    gba_pc_test(pc, "LeafGreen");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &pc
        )
    );
    TEST_ASSERT_NULL(pc);
}

static void gba_leafgreen_item_bag_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_make(
            &item_bag, "LeafGreen"
        )
    );

    gba_item_bag_test(item_bag, "LeafGreen");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_free(
            &item_bag
        )
    );
    TEST_ASSERT_NULL(item_bag);
}

// Colosseum

static void gcn_colosseum_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &item_pocket, "Items", "Colosseum"
        )
    );

    gcn_item_pocket_test(item_pocket, "Colosseum");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &item_pocket
        )
    );
    TEST_ASSERT_NULL(item_pocket);
}

static void gcn_colosseum_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &key_item_pocket, "Key Items", "Colosseum"
        )
    );

    gcn_key_item_pocket_test(key_item_pocket, "Colosseum");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &key_item_pocket
        )
    );
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gcn_colosseum_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &ball_pocket, "Poké Balls", "Colosseum"
        )
    );

    gcn_ball_pocket_test(ball_pocket, "Colosseum");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &ball_pocket
        )
    );
    TEST_ASSERT_NULL(ball_pocket);
}

static void gcn_colosseum_tm_pocket_test() {
    pkmn_item_list_handle_t tm_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &tm_pocket, "TMs", "Colosseum"
        )
    );

    gcn_tm_pocket_test(tm_pocket, "Colosseum");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &tm_pocket
        )
    );
    TEST_ASSERT_NULL(tm_pocket);
}

static void gcn_colosseum_berry_pocket_test() {
    pkmn_item_list_handle_t berry_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &berry_pocket, "Berries", "Colosseum"
        )
    );

    gcn_berry_pocket_test(berry_pocket, "Colosseum");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &berry_pocket
        )
    );
    TEST_ASSERT_NULL(berry_pocket);
}

static void gcn_colosseum_cologne_pocket_test() {
    pkmn_item_list_handle_t cologne_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &cologne_pocket, "Colognes", "Colosseum"
        )
    );

    gcn_cologne_pocket_test(cologne_pocket, "Colosseum");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &cologne_pocket
        )
    );
    TEST_ASSERT_NULL(cologne_pocket);
}

static void gcn_colosseum_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &pc, "PC", "Colosseum"
        )
    );

    gcn_item_pc_test(pc, "Colosseum");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &pc
        )
    );
    TEST_ASSERT_NULL(pc);
}

static void gcn_colosseum_bag_test() {
    pkmn_item_bag_handle_t bag = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_make(
            &bag, "Colosseum"
        )
    );

    gcn_item_bag_test(bag, "Colosseum");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_free(
            &bag
        )
    );
    TEST_ASSERT_NULL(bag);
}

// XD

static void gcn_xd_item_pocket_test() {
    pkmn_item_list_handle_t item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &item_pocket, "Items", "XD"
        )
    );

    gcn_item_pocket_test(item_pocket, "XD");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &item_pocket
        )
    );
    TEST_ASSERT_NULL(item_pocket);
}

static void gcn_xd_key_item_pocket_test() {
    pkmn_item_list_handle_t key_item_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &key_item_pocket, "Key Items", "XD"
        )
    );

    gcn_key_item_pocket_test(key_item_pocket, "XD");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &key_item_pocket
        )
    );
    TEST_ASSERT_NULL(key_item_pocket);
}

static void gcn_xd_ball_pocket_test() {
    pkmn_item_list_handle_t ball_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &ball_pocket, "Poké Balls", "XD"
        )
    );

    gcn_ball_pocket_test(ball_pocket, "XD");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &ball_pocket
        )
    );
    TEST_ASSERT_NULL(ball_pocket);
}

static void gcn_xd_tm_pocket_test() {
    pkmn_item_list_handle_t tm_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &tm_pocket, "TMs", "XD"
        )
    );

    gcn_tm_pocket_test(tm_pocket, "XD");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &tm_pocket
        )
    );
    TEST_ASSERT_NULL(tm_pocket);
}

static void gcn_xd_berry_pocket_test() {
    pkmn_item_list_handle_t berry_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &berry_pocket, "Berries", "XD"
        )
    );

    gcn_berry_pocket_test(berry_pocket, "XD");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &berry_pocket
        )
    );
    TEST_ASSERT_NULL(berry_pocket);
}

static void gcn_xd_cologne_pocket_test() {
    pkmn_item_list_handle_t cologne_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &cologne_pocket, "Colognes", "XD"
        )
    );

    gcn_cologne_pocket_test(cologne_pocket, "XD");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &cologne_pocket
        )
    );
    TEST_ASSERT_NULL(cologne_pocket);
}

static void gcn_xd_battle_cd_pocket_test() {
    pkmn_item_list_handle_t battle_cd_pocket = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &battle_cd_pocket, "Battle CDs", "XD"
        )
    );

    xd_battle_cd_pocket_test(battle_cd_pocket);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &battle_cd_pocket
        )
    );
    TEST_ASSERT_NULL(battle_cd_pocket);
}

static void gcn_xd_pc_test() {
    pkmn_item_list_handle_t pc = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_make(
            &pc, "PC", "XD"
        )
    );

    gcn_item_pc_test(pc, "XD");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(
            &pc
        )
    );
    TEST_ASSERT_NULL(pc);
}

static void gcn_xd_bag_test() {
    pkmn_item_bag_handle_t bag = NULL;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_make(
            &bag, "XD"
        )
    );

    gcn_item_bag_test(bag, "XD");

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_free(
            &bag
        )
    );
    TEST_ASSERT_NULL(bag);
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

    PKMN_C_TEST(gcn_colosseum_item_pocket_test)
    PKMN_C_TEST(gcn_colosseum_key_item_pocket_test)
    PKMN_C_TEST(gcn_colosseum_ball_pocket_test)
    PKMN_C_TEST(gcn_colosseum_tm_pocket_test)
    PKMN_C_TEST(gcn_colosseum_berry_pocket_test)
    PKMN_C_TEST(gcn_colosseum_cologne_pocket_test)
    PKMN_C_TEST(gcn_colosseum_pc_test)
    PKMN_C_TEST(gcn_colosseum_bag_test)

    PKMN_C_TEST(gcn_xd_item_pocket_test)
    PKMN_C_TEST(gcn_xd_key_item_pocket_test)
    PKMN_C_TEST(gcn_xd_ball_pocket_test)
    PKMN_C_TEST(gcn_xd_tm_pocket_test)
    PKMN_C_TEST(gcn_xd_berry_pocket_test)
    PKMN_C_TEST(gcn_xd_cologne_pocket_test)
    PKMN_C_TEST(gcn_xd_battle_cd_pocket_test)
    PKMN_C_TEST(gcn_xd_pc_test)
    PKMN_C_TEST(gcn_xd_bag_test)
)
