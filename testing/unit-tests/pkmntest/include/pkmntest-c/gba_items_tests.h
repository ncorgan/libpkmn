/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GBA_ITEMS_TESTS_H
#define GBA_ITEMS_TESTS_H

#include <pkmn-c/config.h>
#include <pkmn-c/item_bag.h>
#include <pkmn-c/item_list.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API void pkmntest_gba_item_pocket_test(
    pkmn_item_list_handle_t item_pocket,
    const char* game
);

PKMN_API void pkmntest_gba_key_item_pocket_test(
    pkmn_item_list_handle_t key_item_pocket,
    const char* game
);

PKMN_API void pkmntest_gba_ball_pocket_test(
    pkmn_item_list_handle_t ball_pocket,
    const char* game
);

PKMN_API void pkmntest_gba_tmhm_pocket_test(
    pkmn_item_list_handle_t tmhm_pocket,
    const char* game
);

PKMN_API void pkmntest_gba_berry_pocket_test(
    pkmn_item_list_handle_t berry_pocket,
    const char* game
);

PKMN_API void pkmntest_gba_pc_test(
    pkmn_item_list_handle_t pc,
    const char* game
);

PKMN_API void pkmntest_gba_item_bag_test(
    pkmn_item_bag_handle_t bag,
    const char* game
);

#ifdef __cplusplus
}
#endif

#endif /* GBA_ITEMS_TESTS_H */
