/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GEN1_ITEMS_TESTS_H
#define GEN1_ITEMS_TESTS_H

#include <pkmntest-c/config.h>

#include <pkmn-c/item_bag.h>
#include <pkmn-c/item_list.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMNTEST_API void pkmntest_gen1_item_list_test(
    pkmn_item_list_handle_t items,
    const char* game
);

PKMNTEST_API void pkmntest_gen1_pc_test(
    pkmn_item_list_handle_t pc,
    const char* game
);

PKMNTEST_API void pkmntest_gen1_item_bag_test(
    pkmn_item_bag_handle_t bag,
    const char* game
);

#ifdef __cplusplus
}
#endif

#endif /* GEN1_ITEMS_TESTS_H */
