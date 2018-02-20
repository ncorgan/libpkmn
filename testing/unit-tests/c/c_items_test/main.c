/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include "c_items_test.h"

#include <pkmn-c.h>

#define GEN1_UNITY_TESTS(game) \
{ \
    PKMN_C_TEST(test_gen1_item_pocket_ ## game ) \
    PKMN_C_TEST(test_gen1_item_pc_ ## game ) \
    PKMN_C_TEST(test_gen1_item_bag_ ## game ) \
}

#define GEN2_UNITY_TESTS(game) \
{ \
    PKMN_C_TEST(test_gen2_item_pocket_ ## game ) \
    PKMN_C_TEST(test_gen2_key_item_pocket_ ## game ) \
    PKMN_C_TEST(test_gen2_ball_pocket_ ## game ) \
    PKMN_C_TEST(test_gen_tmhm_pocket_ ## game ) \
    PKMN_C_TEST(test_gen2_item_pc_ ## game ) \
    PKMN_C_TEST(test_gen2_item_bag_ ## game ) \
}

#define GBA_UNITY_TESTS(game) \
{ \
    PKMN_C_TEST(test_gba_item_pocket_ ## game ) \
    PKMN_C_TEST(test_gba_key_item_pocket_ ## game ) \
    PKMN_C_TEST(test_gba_ball_pocket_ ## game ) \
    PKMN_C_TEST(test_gba_tmhm_pocket_ ## game ) \
    PKMN_C_TEST(test_gba_berry_pocket_ ## game ) \
    PKMN_C_TEST(test_gba_item_pc_ ## game ) \
    PKMN_C_TEST(test_gba_item_bag_ ## game ) \
}

#define GCN_UNITY_TESTS(game) \
{ \
    PKMN_C_TEST(test_gcn_item_pocket_ ## game ) \
    PKMN_C_TEST(test_gcn_key_item_pocket_ ## game ) \
    PKMN_C_TEST(test_gcn_ball_pocket_ ## game ) \
    PKMN_C_TEST(test_gcn_tm_pocket_ ## game ) \
    PKMN_C_TEST(test_gcn_berry_pocket_ ## game ) \
    PKMN_C_TEST(test_gcn_cologne_pocket_ ## game ) \
    PKMN_C_TEST(test_gcn_item_pc_ ## game ) \
    PKMN_C_TEST(test_gcn_item_bag_ ## game ) \
}

PKMN_C_TEST_MAIN(
    GEN1_UNITY_TESTS(Red)
    GEN1_UNITY_TESTS(Blue)
    GEN1_UNITY_TESTS(Yellow)
    GEN2_UNITY_TESTS(Gold)
    GEN2_UNITY_TESTS(Silver)
    GEN2_UNITY_TESTS(Crystal)
    GBA_UNITY_TESTS(Ruby)
    GBA_UNITY_TESTS(Sapphire)
    GBA_UNITY_TESTS(Emerald)
    GBA_UNITY_TESTS(FireRed)
    GBA_UNITY_TESTS(LeafGreen)
    GCN_UNITY_TESTS(Colosseum)
    GCN_UNITY_TESTS(XD)
    PKMN_C_TEST(test_gcn_battle_cd_pocket_XD)
)
