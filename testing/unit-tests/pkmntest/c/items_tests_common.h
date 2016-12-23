/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef ITEMS_TESTS_COMMON_H
#define ITEMS_TESTS_COMMON_H

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

#ifdef __cplusplus
}
#endif

#endif /* ITEMS_TESTS_COMMON_H */
