/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef C_ITEMS_TEST_H
#define C_ITEMS_TEST_H

#include <pkmn-c/item_list.h>
#include <pkmn-c/item_bag.h>

#include <stdlib.h>

// Helper functions

void check_num_items(
    pkmn_item_list_t* item_list_ptr,
    size_t expected_num_items
);

void check_item_at_index(
    pkmn_item_list_t* item_list_ptr,
    size_t index,
    const char* expected_item_name,
    size_t expected_item_amount
);

// Test functions

void test_item_list_empty_slots(
    pkmn_item_list_t* list
);

void test_item_list_out_of_range_error(
    pkmn_item_list_t* item_list_ptr,
    const char* item_name
);

void test_item_list_invalid_items(
    pkmn_item_list_t* item_list_ptr,
    const char** item_names,
    size_t num_items
);

void test_item_bag_invalid_items(
    pkmn_item_bag_t* item_bag_ptr,
    const char** item_names,
    size_t num_items
);

void test_item_list_set_item(
    pkmn_item_list_t* item_list_ptr,
    const char** item_names,
    size_t num_items
);

void test_item_list_add_remove(
    pkmn_item_list_t* item_list_ptr,
    const char** item_names,
    size_t num_items
);

#define GEN1_TEST_FCNS(game) \
    void test_gen1_item_pocket_ ## game (); \
    void test_gen1_item_pc_ ## game (); \
    void test_gen1_item_bag_ ## game ();

GEN1_TEST_FCNS(Red);
GEN1_TEST_FCNS(Blue);
GEN1_TEST_FCNS(Yellow);

#define GEN2_TEST_FCNS(game) \
    void test_gen2_item_pocket_ ## game (); \
    void test_gen2_key_item_pocket_ ## game (); \
    void test_gen2_ball_pocket_ ## game (); \
    void test_gen2_tmhm_pocket_ ## game (); \
    void test_gen2_item_pc_ ## game (); \
    void test_gen2_item_bag_ ## game ();

GEN2_TEST_FCNS(Gold);
GEN2_TEST_FCNS(Silver);
GEN2_TEST_FCNS(Crystal);

#define GBA_TEST_FCNS(game) \
    void test_gba_item_pocket_ ## game (); \
    void test_gba_key_item_pocket_ ## game (); \
    void test_gba_ball_pocket_ ## game (); \
    void test_gba_tmhm_pocket_ ## game (); \
    void test_gba_berry_pocket_ ## game (); \
    void test_gba_item_pc_ ## game (); \
    void test_gba_item_bag_ ## game ();

GBA_TEST_FCNS(Ruby);
GBA_TEST_FCNS(Sapphire);
GBA_TEST_FCNS(Emerald);
GBA_TEST_FCNS(FireRed);
GBA_TEST_FCNS(LeafGreen);

#define GCN_TEST_FCNS(game) \
    void test_gcn_item_pocket_ ## game (); \
    void test_gcn_key_item_pocket_ ## game (); \
    void test_gcn_ball_pocket_ ## game (); \
    void test_gcn_tm_pocket_ ## game (); \
    void test_gcn_berry_pocket_ ## game (); \
    void test_gcn_cologne_pocket_ ## game (); \
    void test_gcn_item_pc_ ## game (); \
    void test_gcn_item_bag_ ## game ();

GCN_TEST_FCNS(Colosseum);
GCN_TEST_FCNS(XD);

void test_gcn_battle_cd_pocket_XD();

#endif /* C_ITEMS_TEST_H */
