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

void test_item_list_empty_slots(
    pkmn_item_list2_t* list
);

void test_item_list_out_of_range_error(
    pkmn_item_list2_t* list,
    const char* item_name
);

void test_item_list_invalid_items(
    pkmn_item_list2_t* list,
    const char** item_names,
    size_t num_items
);

void test_item_list_set_item(
    pkmn_item_list2_t* list,
    const char** item_names,
    size_t num_items
);

void test_item_list_add_remove(
    pkmn_item_list2_t* list,
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

#endif /* C_ITEMS_TEST_H */
