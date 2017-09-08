/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef C_ITEMS_TEST_H
#define C_ITEMS_TEST_H

#include <pkmn-c/item_list.h>
#include <pkmn-c/item_bag.h>

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void test_item_list_empty_slots(
    pkmn_item_list_handle_t list
);

void test_item_list_out_of_range_error(
    pkmn_item_list_handle_t list,
    const char* item_name
);

void test_item_bag_invalid_items(
    pkmn_item_bag_handle_t bag,
    const char** item_names,
    size_t item_names_list_length
);

void test_item_list_invalid_items(
    pkmn_item_list_handle_t list,
    const char** item_names,
    size_t item_names_list_length
);

void test_item_list_add_remove(
    pkmn_item_list_handle_t list,
    const char** item_names,
    size_t item_names_list_length
);

void gen1_item_list_test(
    pkmn_item_list_handle_t items,
    const char* game
);

void gen1_pc_test(
    pkmn_item_list_handle_t pc,
    const char* game
);

void gen1_item_bag_test(
    pkmn_item_bag_handle_t bag,
    const char* game
);

void gen2_item_pocket_test(
    pkmn_item_list_handle_t item_pocket,
    const char* game
);

void gen2_key_item_pocket_test(
    pkmn_item_list_handle_t key_item_pocket,
    const char* game
);

void gen2_ball_pocket_test(
    pkmn_item_list_handle_t ball_pocket,
    const char* game
);

void gen2_tmhm_pocket_test(
    pkmn_item_list_handle_t tmhm_pocket,
    const char* game
);

void gen2_pc_test(
    pkmn_item_list_handle_t pc,
    const char* game
);

void gen2_item_bag_test(
    pkmn_item_bag_handle_t bag,
    const char* game
);

void gba_item_pocket_test(
    pkmn_item_list_handle_t item_pocket,
    const char* game
);

void gba_key_item_pocket_test(
    pkmn_item_list_handle_t key_item_pocket,
    const char* game
);

void gba_ball_pocket_test(
    pkmn_item_list_handle_t ball_pocket,
    const char* game
);

void gba_tmhm_pocket_test(
    pkmn_item_list_handle_t tmhm_pocket,
    const char* game
);

void gba_berry_pocket_test(
    pkmn_item_list_handle_t berry_pocket,
    const char* game
);

void gba_pc_test(
    pkmn_item_list_handle_t pc,
    const char* game
);

void gba_item_bag_test(
    pkmn_item_bag_handle_t bag,
    const char* game
);

void gcn_item_pocket_test(
    pkmn_item_list_handle_t item_pocket,
    const char* game
);

void gcn_key_item_pocket_test(
    pkmn_item_list_handle_t key_item_pocket,
    const char* game
);

void gcn_ball_pocket_test(
    pkmn_item_list_handle_t ball_pocket,
    const char* game
);

void gcn_tm_pocket_test(
    pkmn_item_list_handle_t tm_pocket,
    const char* game
);

void gcn_berry_pocket_test(
    pkmn_item_list_handle_t berry_pocket,
    const char* game
);

void gcn_cologne_pocket_test(
    pkmn_item_list_handle_t cologne_pocket,
    const char* game
);

void xd_battle_cd_pocket_test(
    pkmn_item_list_handle_t battle_cd_pocket
);

void gcn_item_pc_test(
    pkmn_item_list_handle_t pc,
    const char* game
);

void gcn_item_bag_test(
    pkmn_item_bag_handle_t bag,
    const char* game
);

#ifdef __cplusplus
}
#endif

#endif /* C_ITEMS_TEST_H */
