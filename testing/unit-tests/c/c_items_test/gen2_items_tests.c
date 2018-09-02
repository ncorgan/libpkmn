/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_items_test.h"

#include <pkmn-c/database/lists.h>

#include <unity.h>

#include <string.h>

static const enum pkmn_item ALL_POCKET_ITEMS[] =
{
    PKMN_ITEM_POTION,
    PKMN_ITEM_BICYCLE,
    PKMN_ITEM_GREAT_BALL,
    PKMN_ITEM_TM28,
    PKMN_ITEM_BERRY,
    PKMN_ITEM_SQUIRT_BOTTLE,
    PKMN_ITEM_FRIEND_BALL,
    PKMN_ITEM_HM01
};
static const enum pkmn_item CRYSTAL_ITEMS[] =
{
    PKMN_ITEM_CLEAR_BELL,
    PKMN_ITEM_GS_BALL,
    PKMN_ITEM_BLUE_CARD,
    PKMN_ITEM_EGG_TICKET
};
static const enum pkmn_item WRONG_GENERATION_ALL_POCKET_ITEMS[] =
{
    PKMN_ITEM_BLACK_SLUDGE,
    PKMN_ITEM_POFFIN_CASE,
    PKMN_ITEM_TM51,
    PKMN_ITEM_OCCA_BERRY
};

static void gen2_item_pocket_test(
    const struct pkmn_item_list* p_item_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_item_pocket);
    TEST_ASSERT_NOT_NULL(p_item_pocket->p_internal);

    TEST_ASSERT_EQUAL_STRING("Items", p_item_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_item_pocket->game);
    TEST_ASSERT_EQUAL(20, p_item_pocket->capacity);

    enum pkmn_error error = PKMN_ERROR_NONE;

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_item_pocket,
        PKMN_ITEM_POTION
    );

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item wrong_pocket_items[] =
    {
        PKMN_ITEM_BICYCLE,
        PKMN_ITEM_MASTER_BALL,
        PKMN_ITEM_HM01
    };
    test_item_list_invalid_items(p_item_pocket, wrong_pocket_items, 3);

    // Make sure we can't add items from later generations.
    static const enum pkmn_item wrong_generation_items[] =
    {
        PKMN_ITEM_BLACK_FLUTE,
        PKMN_ITEM_BLACK_SLUDGE,
        PKMN_ITEM_BINDING_BAND,
        PKMN_ITEM_BEEDRILLITE
    };
    test_item_list_invalid_items(p_item_pocket, wrong_generation_items, 4);

    static const enum pkmn_item items[] =
    {
        PKMN_ITEM_POTION,
        PKMN_ITEM_HP_UP,
        PKMN_ITEM_WHITE_APRICORN,
        PKMN_ITEM_LUCKY_EGG,
        PKMN_ITEM_FLOWER_MAIL,
        PKMN_ITEM_BURN_HEAL,
        PKMN_ITEM_PSNCUREBERRY,
        PKMN_ITEM_STICK
    };

    // Test setting items by index.
    test_item_list_set_item(
        p_item_pocket,
        items,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_item_pocket,
        items,
        8
    );

    struct pkmn_item_enum_list valid_items =
    {
        .p_enums = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                p_item_pocket,
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
                p_item_pocket,
                &valid_item_names
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_item_names.pp_strings);
    TEST_ASSERT_EQUAL(
        valid_items.length,
        valid_item_names.length
    );

    error = pkmn_item_enum_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.p_enums);
    TEST_ASSERT_EQUAL(0, valid_items.length);

    error = pkmn_string_list_free(&valid_item_names);
    TEST_ASSERT_NULL(valid_item_names.pp_strings);
    TEST_ASSERT_EQUAL(0, valid_item_names.length);
}

static void gen2_key_item_pocket_test(
    const struct pkmn_item_list* p_key_item_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_key_item_pocket);
    TEST_ASSERT_NOT_NULL(p_key_item_pocket->p_internal);

    TEST_ASSERT_EQUAL_STRING("KeyItems", p_key_item_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_key_item_pocket->game);
    TEST_ASSERT_EQUAL(25, p_key_item_pocket->capacity);

    enum pkmn_error error = PKMN_ERROR_NONE;

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_key_item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_key_item_pocket,
        PKMN_ITEM_BICYCLE
    );

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item wrong_pocket_items[] =
    {
        PKMN_ITEM_POTION,
        PKMN_ITEM_MASTER_BALL,
        PKMN_ITEM_HM01
    };
    test_item_list_invalid_items(p_key_item_pocket, wrong_pocket_items, 3);

    static const enum pkmn_item wrong_generation_items[] =
    {
        PKMN_ITEM_MACH_BIKE,
        PKMN_ITEM_JADE_ORB,
        PKMN_ITEM_LIGHT_STONE,
        PKMN_ITEM_AQUA_SUIT
    };
    test_item_list_invalid_items(p_key_item_pocket, wrong_generation_items, 4);

    // Crystal-specific items.
    for(size_t item_index = 0; item_index < 4; ++item_index)
    {
        error = pkmn_item_list_add(
                    p_key_item_pocket,
                    CRYSTAL_ITEMS[item_index],
                    1
                );
        if(game == PKMN_GAME_CRYSTAL)
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }

        error = pkmn_item_list_remove(
                    p_key_item_pocket,
                    CRYSTAL_ITEMS[item_index],
                    1
                );
        if(game == PKMN_GAME_CRYSTAL)
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }

        check_num_items(p_key_item_pocket, 0);
    }

    // Make sure we can't add or remove more than a single item.
    error = pkmn_item_list_add(
                p_key_item_pocket,
                PKMN_ITEM_BICYCLE,
                5
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_item_list_add(
                p_key_item_pocket,
                PKMN_ITEM_BICYCLE,
                1
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_num_items(p_key_item_pocket, 1);

    error = pkmn_item_list_remove(
                p_key_item_pocket,
                PKMN_ITEM_BICYCLE,
                5
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_item_list_remove(
                p_key_item_pocket,
                PKMN_ITEM_BICYCLE,
                1
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_num_items(p_key_item_pocket, 0);

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const enum pkmn_item items[] =
    {
        PKMN_ITEM_BICYCLE,
        PKMN_ITEM_BASEMENT_KEY,
        PKMN_ITEM_SECRETPOTION,
        PKMN_ITEM_MYSTERY_EGG,
        PKMN_ITEM_SILVER_WING,
        PKMN_ITEM_LOST_ITEM,
        PKMN_ITEM_SQUIRT_BOTTLE,
        PKMN_ITEM_RAINBOW_WING
    };
    for(size_t item_index = 0; item_index < 8; ++item_index)
    {
        if(item_index < 4)
        {
            error = pkmn_item_list_add(
                        p_key_item_pocket,
                        items[item_index],
                        1
                    );
        }
        else
        {
            error = pkmn_item_list_set_item(
                        p_key_item_pocket,
                        item_index,
                        items[item_index],
                        1
                    );
        }
        PKMN_TEST_ASSERT_SUCCESS(error);
    }

    error = pkmn_item_list_remove(
                p_key_item_pocket,
                items[2],
                1
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_list_set_item(
                p_key_item_pocket,
                2,
                PKMN_ITEM_NONE,
                0
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    size_t num_items = 0;
    do
    {
        error = pkmn_item_list_set_item(
                    p_key_item_pocket,
                    0,
                    PKMN_ITEM_NONE,
                    0
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_item_list_get_num_items(
                    p_key_item_pocket,
                    &num_items
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
    } while(num_items > 0);

    struct pkmn_item_enum_list valid_items =
    {
        .p_enums = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                p_key_item_pocket,
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
                p_key_item_pocket,
                &valid_item_names
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_item_names.pp_strings);
    TEST_ASSERT_EQUAL(
        valid_items.length,
        valid_item_names.length
    );

    error = pkmn_item_enum_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.p_enums);
    TEST_ASSERT_EQUAL(0, valid_items.length);

    error = pkmn_string_list_free(&valid_item_names);
    TEST_ASSERT_NULL(valid_item_names.pp_strings);
    TEST_ASSERT_EQUAL(0, valid_item_names.length);
}

static void gen2_ball_pocket_test(
    const struct pkmn_item_list* p_ball_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_ball_pocket);
    TEST_ASSERT_NOT_NULL(p_ball_pocket->p_internal);

    TEST_ASSERT_EQUAL_STRING("Balls", p_ball_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_ball_pocket->game);
    TEST_ASSERT_EQUAL(12, p_ball_pocket->capacity);

    enum pkmn_error error = PKMN_ERROR_NONE;

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_ball_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_ball_pocket,
        PKMN_ITEM_MASTER_BALL
    );

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item wrong_pocket_items[] =
    {
        PKMN_ITEM_POTION,
        PKMN_ITEM_BICYCLE,
        PKMN_ITEM_HM01
    };
    test_item_list_invalid_items(p_ball_pocket, wrong_pocket_items, 3);

    // Make sure we can't add items from later generations.
    static const enum pkmn_item wrong_generation_items[] =
    {
        PKMN_ITEM_PREMIER_BALL,
        PKMN_ITEM_HEAL_BALL,
        PKMN_ITEM_DREAM_BALL
    };
    test_item_list_invalid_items(p_ball_pocket, wrong_generation_items, 3);

    static const enum pkmn_item items[] =
    {
        PKMN_ITEM_GREAT_BALL,
        PKMN_ITEM_POKE_BALL,
        PKMN_ITEM_PARK_BALL,
        PKMN_ITEM_FAST_BALL,
        PKMN_ITEM_MASTER_BALL,
        PKMN_ITEM_FRIEND_BALL,
        PKMN_ITEM_LOVE_BALL,
        PKMN_ITEM_LEVEL_BALL
    };

    // Test setting items by index.
    test_item_list_set_item(
        p_ball_pocket,
        items,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_ball_pocket,
        items,
        8
    );

    struct pkmn_item_enum_list valid_items =
    {
        .p_enums = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                p_ball_pocket,
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
                p_ball_pocket,
                &valid_item_names
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_item_names.pp_strings);
    TEST_ASSERT_EQUAL(
        valid_items.length,
        valid_item_names.length
    );

    error = pkmn_item_enum_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.p_enums);
    TEST_ASSERT_EQUAL(0, valid_items.length);

    error = pkmn_string_list_free(&valid_item_names);
    TEST_ASSERT_NULL(valid_item_names.pp_strings);
    TEST_ASSERT_EQUAL(0, valid_item_names.length);
}

static void gen2_tmhm_pocket_test(
    const struct pkmn_item_list* p_tmhm_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_tmhm_pocket);
    TEST_ASSERT_NOT_NULL(p_tmhm_pocket->p_internal);

    TEST_ASSERT_EQUAL_STRING("TM/HM", p_tmhm_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_tmhm_pocket->game);
    TEST_ASSERT_EQUAL(57, p_tmhm_pocket->capacity);

    enum pkmn_error error = PKMN_ERROR_NONE;

    // Make sure item slots start as correctly empty.
    for(int tm_num = 1; tm_num <= 50; ++tm_num)
    {
        const enum pkmn_item expected_item = PKMN_ITEM_TM01 + (tm_num - 1);
        check_item_at_index(p_tmhm_pocket, tm_num-1, expected_item, 0);
    }
    for(int hm_num = 1; hm_num <= 7; ++hm_num)
    {
        const enum pkmn_item expected_item = PKMN_ITEM_HM01 + (hm_num - 1);
        check_item_at_index(p_tmhm_pocket, hm_num-1+50, expected_item, 0);
    }

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_tmhm_pocket,
        PKMN_ITEM_TM01
    );

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item wrong_pocket_items[] =
    {
        PKMN_ITEM_POTION,
        PKMN_ITEM_BICYCLE,
        PKMN_ITEM_MASTER_BALL
    };
    test_item_list_invalid_items(p_tmhm_pocket, wrong_pocket_items, 3);

    // Make sure we can't add items from later generations.
    static const enum pkmn_item wrong_generation_items[] =
    {
        PKMN_ITEM_TM51,
        PKMN_ITEM_HM08
    };
    test_item_list_invalid_items(p_tmhm_pocket, wrong_generation_items, 2);

    // Start adding and removing items, and make sure the numbers are accurate.
    for(int tm_num = 1; tm_num <= 50; ++tm_num)
    {
        const enum pkmn_item expected_item = PKMN_ITEM_TM01 + (tm_num - 1);
        error = pkmn_item_list_add(
                    p_tmhm_pocket,
                    expected_item,
                    50
                );
        check_num_items(p_tmhm_pocket, tm_num);
        check_item_at_index(p_tmhm_pocket, tm_num-1, expected_item, 50);
    }
    for(int tm_num = 50; tm_num >= 1; --tm_num)
    {
        const enum pkmn_item expected_item = PKMN_ITEM_TM01 + (tm_num - 1);
        error = pkmn_item_list_remove(
                    p_tmhm_pocket,
                    expected_item,
                    50
                );
        check_num_items(p_tmhm_pocket, tm_num-1);
        check_item_at_index(p_tmhm_pocket, tm_num-1, expected_item, 0);
    }

    for(int hm_num = 1; hm_num <= 7; ++hm_num)
    {
        const enum pkmn_item expected_item = PKMN_ITEM_HM01 + (hm_num - 1);
        error = pkmn_item_list_add(
                    p_tmhm_pocket,
                    expected_item,
                    50
                );
        check_num_items(p_tmhm_pocket, hm_num);
        check_item_at_index(p_tmhm_pocket, hm_num-1+50, expected_item, 50);
    }
    for(int hm_num = 7; hm_num >= 1; --hm_num)
    {
        const enum pkmn_item expected_item = PKMN_ITEM_HM01 + (hm_num - 1);
        error = pkmn_item_list_remove(
                    p_tmhm_pocket,
                    expected_item,
                    50
                );
        check_num_items(p_tmhm_pocket, hm_num-1);
        check_item_at_index(p_tmhm_pocket, hm_num-1+50, expected_item, 0);
    }

    struct pkmn_item_enum_list valid_items =
    {
        .p_enums = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                p_tmhm_pocket,
                &valid_items
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.p_enums);
    TEST_ASSERT_EQUAL(57, valid_items.length);

    struct pkmn_string_list valid_item_names =
    {
        .pp_strings = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_item_names(
                p_tmhm_pocket,
                &valid_item_names
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_item_names.pp_strings);
    TEST_ASSERT_EQUAL(57, valid_item_names.length);

    error = pkmn_item_enum_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.p_enums);
    TEST_ASSERT_EQUAL(0, valid_items.length);

    error = pkmn_string_list_free(&valid_item_names);
    TEST_ASSERT_NULL(valid_item_names.pp_strings);
    TEST_ASSERT_EQUAL(0, valid_item_names.length);
}

static void gen2_item_pc_test(
    enum pkmn_game game
)
{
    struct pkmn_item_list item_pc =
    {
        .p_name = NULL,
        .game = PKMN_GAME_NONE,
        .capacity = 0,
        .p_internal = NULL
    };

    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_item_list_init(
                "PC",
                game,
                &item_pc
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL_STRING("PC", item_pc.p_name);
    TEST_ASSERT_EQUAL(game, item_pc.game);
    TEST_ASSERT_EQUAL(50, item_pc.capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(&item_pc);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        &item_pc,
        PKMN_ITEM_POTION
    );

    // Make sure we can't add items from later generations.
    test_item_list_invalid_items(
        &item_pc,
        WRONG_GENERATION_ALL_POCKET_ITEMS,
        4
    );

    // Test setting items by index.
    test_item_list_set_item(
        &item_pc,
        ALL_POCKET_ITEMS,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        &item_pc,
        ALL_POCKET_ITEMS,
        8
    );

    struct pkmn_item_enum_list valid_items =
    {
        .p_enums = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                &item_pc,
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
                &item_pc,
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
                item_pc.game,
                &full_item_list
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(full_item_list.p_enums);

    struct pkmn_string_list full_item_name_list =
    {
        .pp_strings = NULL,
        .length = 0
    };
    error = pkmn_database_item_name_list(
                item_pc.game,
                &full_item_name_list
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(full_item_name_list.pp_strings);

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

    error = pkmn_item_list_free(&item_pc);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(item_pc.p_internal);
}

static void get_bag_pockets(
    struct pkmn_item_bag* p_item_bag,
    struct pkmn_item_list* p_item_pocket_out,
    struct pkmn_item_list* p_key_item_pocket_out,
    struct pkmn_item_list* p_ball_pocket_out,
    struct pkmn_item_list* p_tmhm_pocket_out
)
{
    TEST_ASSERT_NOT_NULL(p_item_bag);
    TEST_ASSERT_NOT_NULL(p_item_pocket_out);
    TEST_ASSERT_NOT_NULL(p_key_item_pocket_out);
    TEST_ASSERT_NOT_NULL(p_ball_pocket_out);
    TEST_ASSERT_NOT_NULL(p_tmhm_pocket_out);

    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_item_bag_get_pocket(
                p_item_bag,
                "Items",
                p_item_pocket_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                p_item_bag,
                "KeyItems",
                p_key_item_pocket_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                p_item_bag,
                "Balls",
                p_ball_pocket_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                p_item_bag,
                "TM/HM",
                p_tmhm_pocket_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
}

static void gen2_item_bag_test(enum pkmn_game game)
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
    TEST_ASSERT_NOT_NULL(item_bag.p_internal);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_item_bag_strerror(&item_bag)
    );

    TEST_ASSERT_NOT_NULL(item_bag.pocket_names.pp_strings);
    TEST_ASSERT_EQUAL(4, item_bag.pocket_names.length);

    test_item_bag_pocket_names(&item_bag);

    struct pkmn_item_list item_pocket;
    struct pkmn_item_list key_item_pocket;
    struct pkmn_item_list ball_pocket;
    struct pkmn_item_list tmhm_pocket;

    get_bag_pockets(
        &item_bag,
        &item_pocket,
        &key_item_pocket,
        &ball_pocket,
        &tmhm_pocket
    );
    TEST_ASSERT_NOT_NULL(item_pocket.p_internal);
    TEST_ASSERT_NOT_NULL(key_item_pocket.p_internal);
    TEST_ASSERT_NOT_NULL(ball_pocket.p_internal);
    TEST_ASSERT_NOT_NULL(tmhm_pocket.p_internal);

    gen2_item_pocket_test(&item_pocket, game);
    gen2_key_item_pocket_test(&key_item_pocket, game);
    gen2_ball_pocket_test(&ball_pocket, game);
    gen2_tmhm_pocket_test(&tmhm_pocket, game);

    // Make sure adding items through the bag adds to the proper pockets.
    check_num_items(&item_pocket, 0);
    check_num_items(&key_item_pocket, 0);
    check_num_items(&ball_pocket, 0);
    check_num_items(&tmhm_pocket, 0);

    for(size_t item_index = 0; item_index < 8; ++item_index)
    {
        pkmn_item_bag_add(
            &item_bag,
            ALL_POCKET_ITEMS[item_index],
            1
        );
        PKMN_TEST_ASSERT_SUCCESS(error);
    }

    check_item_at_index(&item_pocket, 0, PKMN_ITEM_POTION, 1);
    check_item_at_index(&item_pocket, 1, PKMN_ITEM_BERRY, 1);
    check_item_at_index(&item_pocket, 2, PKMN_ITEM_NONE, 0);

    check_item_at_index(&key_item_pocket, 0, PKMN_ITEM_BICYCLE, 1);
    check_item_at_index(&key_item_pocket, 1, PKMN_ITEM_SQUIRT_BOTTLE, 1);
    check_item_at_index(&key_item_pocket, 2, PKMN_ITEM_NONE, 0);

    check_item_at_index(&ball_pocket, 0, PKMN_ITEM_GREAT_BALL, 1);
    check_item_at_index(&ball_pocket, 1, PKMN_ITEM_FRIEND_BALL, 1);
    check_item_at_index(&ball_pocket, 2, PKMN_ITEM_NONE, 0);

    check_item_at_index(&tmhm_pocket, 0, PKMN_ITEM_TM01, 0);
    check_item_at_index(&tmhm_pocket, 1, PKMN_ITEM_TM02, 0);
    check_item_at_index(&tmhm_pocket, 27, PKMN_ITEM_TM28, 1);
    check_item_at_index(&tmhm_pocket, 50, PKMN_ITEM_HM01, 1);

    // Make sure removing items through the bag removes from the proper pockets.
    for(size_t item_index = 0; item_index < 8; ++item_index)
    {
        pkmn_item_bag_remove(
            &item_bag,
            ALL_POCKET_ITEMS[item_index],
            1
        );
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    check_num_items(&item_pocket, 0);
    check_num_items(&key_item_pocket, 0);
    check_num_items(&ball_pocket, 0);
    check_num_items(&tmhm_pocket, 0);

    check_item_at_index(&item_pocket, 0, PKMN_ITEM_NONE, 0);
    check_item_at_index(&item_pocket, 1, PKMN_ITEM_NONE, 0);
    check_item_at_index(&item_pocket, 2, PKMN_ITEM_NONE, 0);

    check_item_at_index(&key_item_pocket, 0, PKMN_ITEM_NONE, 0);
    check_item_at_index(&key_item_pocket, 1, PKMN_ITEM_NONE, 0);
    check_item_at_index(&key_item_pocket, 2, PKMN_ITEM_NONE, 0);

    check_item_at_index(&ball_pocket, 0, PKMN_ITEM_NONE, 0);
    check_item_at_index(&ball_pocket, 1, PKMN_ITEM_NONE, 0);
    check_item_at_index(&ball_pocket, 2, PKMN_ITEM_NONE, 0);

    check_item_at_index(&tmhm_pocket, 0, PKMN_ITEM_TM01, 0);
    check_item_at_index(&tmhm_pocket, 1, PKMN_ITEM_TM02, 0);
    check_item_at_index(&tmhm_pocket, 27, PKMN_ITEM_TM28, 0);
    check_item_at_index(&tmhm_pocket, 50, PKMN_ITEM_HM01, 0);

    // Make sure Crystal-exclusive items can only be added in Crystal.
    if(game == PKMN_GAME_CRYSTAL)
    {
        for(size_t item_index = 0; item_index < 4; ++item_index)
        {
            error = pkmn_item_bag_add(
                        &item_bag,
                        CRYSTAL_ITEMS[item_index],
                        1
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        for(size_t item_index = 0; item_index < 4; ++item_index)
        {
            check_item_at_index(&key_item_pocket, item_index, CRYSTAL_ITEMS[item_index], 1);
        }
        for(size_t item_index = 0; item_index < 4; ++item_index)
        {
            error = pkmn_item_bag_remove(
                        &item_bag,
                        CRYSTAL_ITEMS[item_index],
                        1
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        for(size_t item_index = 0; item_index < 4; ++item_index)
        {
            check_item_at_index(&key_item_pocket, item_index, PKMN_ITEM_NONE, 0);
        }
    }
    else
    {
        test_item_bag_invalid_items(
            &item_bag,
            CRYSTAL_ITEMS,
            4
        );
    }

    // Make sure we can't add items from later generations.
    test_item_bag_invalid_items(
        &item_bag,
        WRONG_GENERATION_ALL_POCKET_ITEMS,
        4
    );

    // Free pockets and bag.

    error = pkmn_item_list_free(&item_pocket);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_list_free(&key_item_pocket);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_list_free(&ball_pocket);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_list_free(&tmhm_pocket);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_free(&item_bag);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

#define GEN2_ITEM_TESTS(test_game_enum, test_game) \
void test_gen2_item_pocket_ ## test_game () \
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
    gen2_item_pocket_test( \
        &item_pocket, \
        test_game_enum \
    ); \
 \
    error = pkmn_item_list_free(&item_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(item_pocket.p_internal); \
} \
void test_gen2_key_item_pocket_ ## test_game () \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
 \
    struct pkmn_item_list key_item_pocket = \
    { \
        .p_name = NULL, \
        .game = PKMN_GAME_NONE, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "KeyItems", \
                test_game_enum, \
                &key_item_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(key_item_pocket.p_internal); \
 \
    gen2_key_item_pocket_test( \
        &key_item_pocket, \
        test_game_enum \
    ); \
 \
    error = pkmn_item_list_free(&key_item_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(key_item_pocket.p_internal); \
} \
void test_gen2_ball_pocket_ ## test_game () \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
 \
    struct pkmn_item_list ball_pocket = \
    { \
        .p_name = NULL, \
        .game = PKMN_GAME_NONE, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "Balls", \
                test_game_enum, \
                &ball_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(ball_pocket.p_internal); \
 \
    gen2_ball_pocket_test( \
        &ball_pocket, \
        test_game_enum \
    ); \
 \
    error = pkmn_item_list_free(&ball_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(ball_pocket.p_internal); \
} \
void test_gen2_tmhm_pocket_ ## test_game () \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
 \
    struct pkmn_item_list tmhm_pocket = \
    { \
        .p_name = NULL, \
        .game = PKMN_GAME_NONE, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "TM/HM", \
                test_game_enum, \
                &tmhm_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(tmhm_pocket.p_internal); \
 \
    gen2_tmhm_pocket_test( \
        &tmhm_pocket, \
        test_game_enum \
    ); \
 \
    error = pkmn_item_list_free(&tmhm_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(tmhm_pocket.p_internal); \
} \
void test_gen2_item_pc_ ## test_game () \
{ \
    gen2_item_pc_test(test_game_enum); \
} \
void test_gen2_item_bag_ ## test_game () \
{ \
    gen2_item_bag_test(test_game_enum); \
}

GEN2_ITEM_TESTS(PKMN_GAME_GOLD, Gold)
GEN2_ITEM_TESTS(PKMN_GAME_SILVER, Silver)
GEN2_ITEM_TESTS(PKMN_GAME_CRYSTAL, Crystal)
