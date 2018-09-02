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

static const enum pkmn_item ITEMS[] =
{
    PKMN_ITEM_POTION,
    PKMN_ITEM_GREAT_BALL,
    PKMN_ITEM_ETHER,
    PKMN_ITEM_PP_UP,
    PKMN_ITEM_TM34,
    PKMN_ITEM_MOON_STONE,
    PKMN_ITEM_BICYCLE,
    PKMN_ITEM_FULL_HEAL
};
static const enum pkmn_item WRONG_GENERATION_ITEMS[] =
{
    PKMN_ITEM_AMULET_COIN,
    PKMN_ITEM_APICOT_BERRY,
    PKMN_ITEM_AIR_MAIL,
    PKMN_ITEM_AIR_BALLOON,
    PKMN_ITEM_AQUA_SUIT
};

static void gen1_item_list_test_common(
    const struct pkmn_item_list* p_item_list
)
{
    TEST_ASSERT_NOT_NULL(p_item_list);

    enum pkmn_error error = PKMN_ERROR_NONE;

    // Make sure item slots start as correctly empty.
    test_item_list_initial_values(p_item_list);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_item_list,
        PKMN_ITEM_POTION
    );

    // Confirm items from later generations can't be added.
    test_item_list_invalid_items(
        p_item_list,
        WRONG_GENERATION_ITEMS,
        sizeof(WRONG_GENERATION_ITEMS)/sizeof(WRONG_GENERATION_ITEMS[0])
    );

    // Test setting items by index.
    test_item_list_set_item(
        p_item_list,
        ITEMS,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_item_list,
        ITEMS,
        8
    );

    struct pkmn_item_enum_list valid_items =
    {
        .p_enums = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
        p_item_list,
        &valid_items
    );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.p_enums);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    struct pkmn_string_list valid_item_names =
    {
        .pp_strings = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_item_names(
                p_item_list,
                &valid_item_names
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_item_names.pp_strings);
    TEST_ASSERT_EQUAL(
        valid_items.length,
        valid_item_names.length
    );

    struct pkmn_item_enum_list full_item_list =
    {
        .p_enums = NULL,
        .length = 0
    };
    error = pkmn_database_item_list(
                p_item_list->game,
                &full_item_list
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(full_item_list.p_enums);
    TEST_ASSERT_TRUE(full_item_list.length > 0);

    struct pkmn_string_list full_item_name_list =
    {
        .pp_strings = NULL,
        .length = 0
    };
    error = pkmn_database_item_name_list(
                p_item_list->game,
                &full_item_name_list
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(full_item_name_list.pp_strings);
    TEST_ASSERT_TRUE(full_item_name_list.length > 0);

    TEST_ASSERT_EQUAL(
        full_item_list.length,
        valid_items.length
    );
    TEST_ASSERT_EQUAL(
        full_item_name_list.length,
        valid_item_names.length
    );

    error = pkmn_item_enum_list_free(&valid_items);
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_string_list_free(&valid_item_names);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_enum_list_free(&full_item_list);
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_string_list_free(&full_item_name_list);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

static void gen1_item_pocket_test(
    const struct pkmn_item_list* p_item_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_item_pocket);

    TEST_ASSERT_EQUAL_STRING("Items", p_item_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_item_pocket->game);
    TEST_ASSERT_EQUAL(20, p_item_pocket->capacity);

    gen1_item_list_test_common(p_item_pocket);
}

static void gen1_item_pc_test(enum pkmn_game game)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_item_list item_pc =
    {
        .p_name = NULL,
        .game = PKMN_GAME_NONE,
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
    TEST_ASSERT_EQUAL(game, item_pc.game);
    TEST_ASSERT_EQUAL(50, item_pc.capacity);

    gen1_item_list_test_common(&item_pc);

    error = pkmn_item_list_free(&item_pc);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(item_pc.p_internal);
}

static void gen1_item_bag_test(enum pkmn_game game)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_item_bag item_bag =
    {
        .game = PKMN_GAME_NONE,
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
    TEST_ASSERT_EQUAL(game, item_bag.game);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_item_bag_strerror(&item_bag)
    );

    TEST_ASSERT_EQUAL(1, item_bag.pocket_names.length);
    TEST_ASSERT_EQUAL_STRING("Items", item_bag.pocket_names.pp_strings[0]);

    struct pkmn_item_list item_pocket =
    {
        .p_name = NULL,
        .game = PKMN_GAME_NONE,
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
                    ITEMS[item_index],
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
            ITEMS[item_index],
            (item_index+1)
        );
    }
    check_item_at_index(&item_pocket, 8, PKMN_ITEM_NONE, 0);

    // Make sure removing items through the bag removes from the pocket.
    for(size_t item_index = 0; item_index < 8; ++item_index)
    {
        error = pkmn_item_bag_remove(
                    &item_bag,
                    ITEMS[item_index],
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
            PKMN_ITEM_NONE,
            0
        );
    }

    // Free the pocket and bag.

    error = pkmn_item_list_free(&item_pocket);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_free(&item_bag);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

#define GEN1_ITEM_TESTS(test_game_enum, test_game) \
void test_gen1_item_pocket_ ## test_game () \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
 \
    struct pkmn_item_list item_pocket = \
    { \
        .p_name = NULL, \
        .game = PKMN_GAME_NONE, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "Items", \
                test_game_enum, \
                &item_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(item_pocket.p_internal); \
 \
    gen1_item_pocket_test( \
        &item_pocket, \
        test_game_enum \
    ); \
 \
    error = pkmn_item_list_free(&item_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(item_pocket.p_internal); \
} \
void test_gen1_item_pc_ ## test_game () \
{ \
    gen1_item_pc_test(test_game_enum); \
} \
void test_gen1_item_bag_ ## test_game () \
{ \
    gen1_item_bag_test(test_game_enum); \
}

GEN1_ITEM_TESTS(PKMN_GAME_RED, Red)
GEN1_ITEM_TESTS(PKMN_GAME_BLUE, Blue)
GEN1_ITEM_TESTS(PKMN_GAME_YELLOW, Yellow)
