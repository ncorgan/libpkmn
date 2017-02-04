/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GEN2_ITEMS_TESTS_H
#define GEN2_ITEMS_TESTS_H

#include <pkmntest-c/config.h>

#include <pkmn-c/item_bag.h>
#include <pkmn-c/item_list.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMNTEST_API void pkmntest_gen2_item_pocket_test(
    pkmn_item_list_handle_t item_pocket,
    const char* game
);

PKMNTEST_API void pkmntest_gen2_key_item_pocket_test(
    pkmn_item_list_handle_t key_item_pocket,
    const char* game
);

PKMNTEST_API void pkmntest_gen2_ball_pocket_test(
    pkmn_item_list_handle_t ball_pocket,
    const char* game
);

PKMNTEST_API void pkmntest_gen2_tmhm_pocket_test(
    pkmn_item_list_handle_t tmhm_pocket,
    const char* game
);

PKMNTEST_API void pkmntest_gen2_pc_test(
    pkmn_item_list_handle_t pc,
    const char* game
);

PKMNTEST_API void pkmntest_gen2_item_bag_test(
    pkmn_item_bag_handle_t bag,
    const char* game
);

#ifdef __cplusplus
}
#endif

#endif /* GEN2_ITEMS_TESTS_H */
