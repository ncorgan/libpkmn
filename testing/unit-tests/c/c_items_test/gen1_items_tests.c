/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_items_test.h"

#include <pkmn-c/database/lists.h>

#include <unity.h>

#include <string.h>

static const char* ITEM_NAMES[] =
{
    "Potion", "Great Ball", "Ether", "PP Up",
    "TM34", "Moon Stone", "Bicycle", "Full Heal"
};

static const char* WRONG_GENERATION_ITEM_NAMES[] =
{
    "Amulet Coin", "Apicot Berry", "Air Mail",
    "Air Balloon", "Aqua Suit"
};

static void gen1_item_list_test_common(
    pkmn_item_list_t* list
)
{
    TEST_ASSERT_NOT_NULL(list);

    pkmn_error_t error = PKMN_ERROR_NONE;

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(list);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        list,
        "Potion"
    );

    // Confirm items from later generations can't be added.
    test_item_list_invalid_items(
        list,
        WRONG_GENERATION_ITEM_NAMES,
        sizeof(WRONG_GENERATION_ITEM_NAMES)/sizeof(WRONG_GENERATION_ITEM_NAMES[0])
    );

    // Test setting items by index.
    test_item_list_set_item(
        list,
        ITEM_NAMES,
        3
    );
    TEST_ASSERT_EQUAL(0, list->num_items);

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        list,
        ITEM_NAMES,
        8
    );
    TEST_ASSERT_EQUAL(6, list->num_items);

    pkmn_string_list_t valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                list,
                &valid_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    pkmn_string_list_t full_item_list =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_database_item_list(
                list->game,
                &full_item_list
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(full_item_list.strings);
    TEST_ASSERT_TRUE(full_item_list.length > 0);

    TEST_ASSERT_EQUAL(full_item_list.length, valid_items.length);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_string_list_free(&full_item_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void gen1_item_pocket_test(
    pkmn_item_list_t* item_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(item_pocket);
    TEST_ASSERT_NOT_NULL(game);

    TEST_ASSERT_EQUAL_STRING("Items", item_pocket->name);
    TEST_ASSERT_EQUAL_STRING(game, item_pocket->game);
    TEST_ASSERT_EQUAL(20, item_pocket->capacity);

    gen1_item_list_test_common(item_pocket);
}

static void gen1_item_pc_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_item_list_t item_pc =
    {
        .name = NULL,
        .game = NULL,
        .num_items = 0,
        .capacity = 0,
        .item_slots =
        {
            .item_slots = NULL,
            .length = 0
        },
        ._internal = NULL
    };

    error = pkmn_item_list_init(
                "PC",
                game,
                &item_pc
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_pc._internal);

    TEST_ASSERT_EQUAL_STRING("PC", item_pc.name);
    TEST_ASSERT_EQUAL_STRING(game, item_pc.game);
    TEST_ASSERT_EQUAL(50, item_pc.capacity);

    gen1_item_list_test_common(&item_pc);

    error = pkmn_item_list_free(&item_pc);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(item_pc._internal);
}

static void gen1_item_bag_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_item_bag_t bag =
    {
        .game = NULL,
        .pockets =
        {
            .pockets = NULL,
            .pocket_names =
            {
                .strings = NULL,
                .length = 0
            },
        },
        ._internal = NULL
    };

    error = pkmn_item_bag_init(
                game,
                &bag
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_EQUAL_STRING(game, bag.game);

    TEST_ASSERT_EQUAL(1, bag.pockets.num_pockets);
    TEST_ASSERT_EQUAL(1, bag.pockets.pocket_names.length);
    TEST_ASSERT_EQUAL_STRING("Items", bag.pockets.pocket_names.strings[0]);
    TEST_ASSERT_EQUAL_STRING("Items", bag.pockets.pockets[0].name);

    gen1_item_pocket_test(
        &bag.pockets.pockets[0],
        game
    );

    // Reset the bag.
    error = pkmn_item_bag_free(&bag);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_bag_init(
                game,
                &bag
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    // Make sure adding items through the bag adds to the pocket.
    pkmn_item_list_t* item_pocket_ptr = NULL;

    error = pkmn_item_bag_get_pocket(
                &bag,
                "Items",
                &item_pocket_ptr
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_pocket_ptr);
    TEST_ASSERT_EQUAL_STRING("Items", item_pocket_ptr->name);
    TEST_ASSERT_EQUAL(0, item_pocket_ptr->num_items);

    for(int i = 0; i < 8; ++i)
    {
        error = pkmn_item_bag_add(
                    &bag,
                    ITEM_NAMES[i],
                    i+1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    TEST_ASSERT_EQUAL(8, item_pocket_ptr->num_items);
    for(size_t i = 0; i < 8; ++i)
    {
        TEST_ASSERT_EQUAL_STRING(ITEM_NAMES[i], item_pocket_ptr->item_slots.item_slots[i].item);
        TEST_ASSERT_EQUAL(i+1, item_pocket_ptr->item_slots.item_slots[i].amount);
    }
    TEST_ASSERT_EQUAL_STRING("None", item_pocket_ptr->item_slots.item_slots[8].item);
    TEST_ASSERT_EQUAL(0, item_pocket_ptr->item_slots.item_slots[8].amount);

    // Make sure removing items through the bag removes from the pocket.
    for(int i = 0; i < 8; ++i)
    {
        error = pkmn_item_bag_remove(
                    &bag,
                    ITEM_NAMES[i],
                    i+1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    TEST_ASSERT_EQUAL(0, item_pocket_ptr->num_items);
    for(size_t i = 0; i < 9; ++i)
    {
        TEST_ASSERT_EQUAL_STRING("None", item_pocket_ptr->item_slots.item_slots[i].item);
        TEST_ASSERT_EQUAL(0, item_pocket_ptr->item_slots.item_slots[i].amount);
    }
}

#define GEN1_ITEM_TESTS(test_game) \
void test_gen1_item_pocket_ ## test_game () \
{ \
    pkmn_error_t error = PKMN_ERROR_NONE; \
 \
    pkmn_item_list_t item_pocket = \
    { \
        .name = NULL, \
        .game = NULL, \
        .num_items = 0, \
        .capacity = 0, \
        .item_slots = \
        { \
            .item_slots = NULL, \
            .length = 0 \
        }, \
        ._internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "Items", \
                #test_game, \
                &item_pocket \
            ); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NOT_NULL(item_pocket._internal); \
 \
    gen1_item_pocket_test( \
        &item_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&item_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NULL(item_pocket._internal); \
} \
void test_gen1_item_pc_ ## test_game () \
{ \
    gen1_item_pc_test(#test_game); \
} \
void test_gen1_item_bag_ ## test_game () \
{ \
    gen1_item_bag_test(#test_game); \
}

GEN1_ITEM_TESTS(Red)
GEN1_ITEM_TESTS(Blue)
GEN1_ITEM_TESTS(Yellow)
