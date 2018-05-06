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
    struct pkmn_item_list* item_list_ptr
)
{
    TEST_ASSERT_NOT_NULL(item_list_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;

    // Make sure item slots start as correctly empty.
    test_item_list_initial_values(item_list_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        item_list_ptr,
        "Potion"
    );

    // Confirm items from later generations can't be added.
    test_item_list_invalid_items(
        item_list_ptr,
        WRONG_GENERATION_ITEM_NAMES,
        sizeof(WRONG_GENERATION_ITEM_NAMES)/sizeof(WRONG_GENERATION_ITEM_NAMES[0])
    );

    // Test setting items by index.
    test_item_list_set_item(
        item_list_ptr,
        ITEM_NAMES,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        item_list_ptr,
        ITEM_NAMES,
        8
    );

    struct pkmn_string_list valid_items =
    {
        .pp_strings = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                item_list_ptr,
                &valid_items
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.pp_strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    struct pkmn_string_list full_item_list =
    {
        .pp_strings = NULL,
        .length = 0
    };
    error = pkmn_database_item_list(
                item_list_ptr->p_game,
                &full_item_list
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(full_item_list.pp_strings);
    TEST_ASSERT_TRUE(full_item_list.length > 0);

    TEST_ASSERT_EQUAL(full_item_list.length, valid_items.length);

    error = pkmn_string_list_free(&valid_items);
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_string_list_free(&full_item_list);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

static void gen1_item_pocket_test(
    struct pkmn_item_list* item_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(game);

    TEST_ASSERT_EQUAL_STRING("Items", item_pocket_ptr->p_name);
    TEST_ASSERT_EQUAL_STRING(game, item_pocket_ptr->p_game);
    TEST_ASSERT_EQUAL(20, item_pocket_ptr->capacity);

    gen1_item_list_test_common(item_pocket_ptr);
}

static void gen1_item_pc_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_item_list item_pc =
    {
        .p_name = NULL,
        .p_game = NULL,
        .capacity = 0,
        .p_internal = NULL
    };

    error = pkmn_item_list_init(
                "PC",
                game,
                &item_pc
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(item_pc.p_internal);

    TEST_ASSERT_EQUAL_STRING("PC", item_pc.p_name);
    TEST_ASSERT_EQUAL_STRING(game, item_pc.p_game);
    TEST_ASSERT_EQUAL(50, item_pc.capacity);

    gen1_item_list_test_common(&item_pc);

    error = pkmn_item_list_free(&item_pc);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(item_pc.p_internal);
}

static void gen1_item_bag_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_item_bag item_bag =
    {
        .p_game = NULL,
        .pocket_names =
        {
            .pp_strings = NULL,
            .length = 0
        },
        .p_internal = NULL
    };

    error = pkmn_item_bag_init(
                game,
                &item_bag
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING(game, item_bag.p_game);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_item_bag_strerror(&item_bag)
    );

    TEST_ASSERT_EQUAL(1, item_bag.pocket_names.length);
    TEST_ASSERT_EQUAL_STRING("Items", item_bag.pocket_names.pp_strings[0]);

    struct pkmn_item_list item_pocket =
    {
        .p_name = NULL,
        .p_game = NULL,
        .capacity = 0,
        .p_internal = NULL
    };
    error = pkmn_item_bag_get_pocket(
                &item_bag,
                "Items",
                &item_pocket
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    gen1_item_pocket_test(
        &item_pocket,
        game
    );

    // Make sure adding items through the bag removes from the pocket.
    for(size_t item_index = 0; item_index < 8; ++item_index)
    {
        error = pkmn_item_bag_add(
                    &item_bag,
                    ITEM_NAMES[item_index],
                    (item_index+1)
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    check_num_items(&item_pocket, 8);

    for(size_t item_index = 0; item_index < 8; ++item_index)
    {
        check_item_at_index(
            &item_pocket,
            item_index,
            ITEM_NAMES[item_index],
            (item_index+1)
        );
    }
    check_item_at_index(&item_pocket, 8, "None", 0);

    // Make sure removing items through the bag removes from the pocket.
    for(size_t item_index = 0; item_index < 8; ++item_index)
    {
        error = pkmn_item_bag_remove(
                    &item_bag,
                    ITEM_NAMES[item_index],
                    (item_index+1)
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    check_num_items(&item_pocket, 0);

    for(size_t item_index = 0; item_index < 8; ++item_index)
    {
        check_item_at_index(
            &item_pocket,
            item_index,
            "None",
            0
        );
    }

    // Free the pocket and bag.

    error = pkmn_item_list_free(&item_pocket);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_free(&item_bag);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

#define GEN1_ITEM_TESTS(test_game) \
void test_gen1_item_pocket_ ## test_game () \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
 \
    struct pkmn_item_list item_pocket = \
    { \
        .p_name = NULL, \
        .p_game = NULL, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "Items", \
                #test_game, \
                &item_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(item_pocket.p_internal); \
 \
    gen1_item_pocket_test( \
        &item_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&item_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(item_pocket.p_internal); \
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
