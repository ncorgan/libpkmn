/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GEN1_ITEMS_TESTS_H
#define GEN1_ITEMS_TESTS_H

#include <pkmn-c/config.h>
#include <pkmn-c/item_bag.h>
#include <pkmn-c/item_list.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API void pkmntest_gen1_item_list_test(
    pkmn_item_list_handle_t items
);

PKMN_API void pkmntest_gen1_pc_test(
    pkmn_item_list_handle_t pc
);

PKMN_API void pkmntest_gen1_item_bag_test(
    pkmn_item_bag_handle_t bag
);

#ifdef __cplusplus
}
#endif

#endif /* GEN1_ITEMS_TESTS_H */
