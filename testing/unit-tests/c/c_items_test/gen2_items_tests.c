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

static const char* ALL_POCKET_ITEM_NAMES[] =
{
    "Potion", "Bicycle", "Great Ball", "TM28",
    "Berry", "SquirtBottle", "Friend Ball", "HM01"
};

static const char* CRYSTAL_ITEM_NAMES[] =
{
    "Clear Bell", "GS Ball", "Blue Card", "Egg Ticket"
};

static const char* WRONG_GENERATION_ALL_POCKET_ITEM_NAMES[] =
{
    "Black Sludge", "Poffin Items", "TM51", "Occa Berry"
};

static void gen2_item_pocket_test(
    pkmn_item_list_t* item_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(item_pocket_ptr->_internal);
    TEST_ASSERT_NOT_NULL(game);

    TEST_ASSERT_EQUAL_STRING("Items", item_pocket_ptr->name);
    TEST_ASSERT_EQUAL_STRING(game, item_pocket_ptr->game);
    TEST_ASSERT_EQUAL(20, item_pocket_ptr->capacity);

    pkmn_error_t error = PKMN_ERROR_NONE;

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(item_pocket_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        item_pocket_ptr,
        "Potion"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Bicycle", "Master Ball", "HM01"};
    test_item_list_invalid_items(item_pocket_ptr, wrong_pocket_item_names, 3);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] =
    {
        "Black Flute", "Black Sludge", "Binding Band", "Beedrillite"
    };
    test_item_list_invalid_items(item_pocket_ptr, wrong_generation_item_names, 4);

    const char* item_names[] =
    {
        "Potion", "HP Up", "Wht Apricorn", "Lucky Egg",
        "Flower Mail", "Burn Heal", "PSNCureBerry", "Stick"
    };

    // Test setting items by index.
    test_item_list_set_item(
        item_pocket_ptr,
        item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        item_pocket_ptr,
        item_names,
        8
    );

    struct pkmn_string_list valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                item_pocket_ptr,
                &valid_items
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gen2_key_item_pocket_test(
    pkmn_item_list_t* key_item_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr->_internal);
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;

    TEST_ASSERT_EQUAL_STRING("KeyItems", key_item_pocket_ptr->name);
    TEST_ASSERT_EQUAL_STRING(game, key_item_pocket_ptr->game);
    TEST_ASSERT_EQUAL(26, key_item_pocket_ptr->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(key_item_pocket_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        key_item_pocket_ptr,
        "Bicycle"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Potion", "Master Ball", "HM01"};
    test_item_list_invalid_items(key_item_pocket_ptr, wrong_pocket_item_names, 3);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] =
    {
        "Mach Bike", "Jade Orb", "Light Stone", "Aqua Suit"
    };
    test_item_list_invalid_items(key_item_pocket_ptr, wrong_generation_item_names, 4);

    // Crystal-specific items.
    bool is_crystal = (bool)(!strcmp(key_item_pocket_ptr->game, "Crystal"));
    for(size_t i = 0; i < sizeof(CRYSTAL_ITEM_NAMES)/sizeof(CRYSTAL_ITEM_NAMES[0]); ++i)
    {
        error = pkmn_item_list_add(
                    key_item_pocket_ptr,
                    CRYSTAL_ITEM_NAMES[i],
                    1
                );
        TEST_ASSERT_EQUAL(
            is_crystal ? PKMN_ERROR_NONE : PKMN_ERROR_INVALID_ARGUMENT,
            error
        );

        error = pkmn_item_list_remove(
                    key_item_pocket_ptr,
                    CRYSTAL_ITEM_NAMES[i],
                    1
                );
        TEST_ASSERT_EQUAL(
            is_crystal ? PKMN_ERROR_NONE : PKMN_ERROR_INVALID_ARGUMENT,
            error
        );

        check_num_items(key_item_pocket_ptr, 0);
    }

    const char* item_names[] =
    {
        "Bicycle", "Basement Key", "SecretPotion", "Mystery Egg",
        "Silver Wing", "Lost Item", "SquirtBottle", "Rainbow Wing"
    };

    // Test setting items by index.
    test_item_list_set_item(
        key_item_pocket_ptr,
        item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        key_item_pocket_ptr,
        item_names,
        8
    );

    struct pkmn_string_list valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                key_item_pocket_ptr,
                &valid_items
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gen2_ball_pocket_test(
    pkmn_item_list_t* ball_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr);
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr->_internal);
    TEST_ASSERT_NOT_NULL(game);

    TEST_ASSERT_EQUAL_STRING("Balls", ball_pocket_ptr->name);
    TEST_ASSERT_EQUAL_STRING(game, ball_pocket_ptr->game);
    TEST_ASSERT_EQUAL(12, ball_pocket_ptr->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(ball_pocket_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        ball_pocket_ptr,
        "Master Ball"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Potion", "Bicycle", "HM01"};
    test_item_list_invalid_items(ball_pocket_ptr, wrong_pocket_item_names, 3);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"Premier Ball", "Heal Ball", "Dream Ball"};
    test_item_list_invalid_items(ball_pocket_ptr, wrong_generation_item_names, 3);

    const char* item_names[] =
    {
        "Great Ball", "Poké Ball", "Park Ball", "Fast Ball",
        "Master Ball", "Friend Ball", "Love Ball", "Level Ball"
    };

    // Test setting items by index.
    test_item_list_set_item(
        ball_pocket_ptr,
        item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        ball_pocket_ptr,
        item_names,
        8
    );

    struct pkmn_string_list valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = pkmn_item_list_get_valid_items(
                             ball_pocket_ptr,
                             &valid_items
                         );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gen2_tmhm_pocket_test(
    pkmn_item_list_t* tmhm_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(tmhm_pocket_ptr);
    TEST_ASSERT_NOT_NULL(tmhm_pocket_ptr->_internal);
    TEST_ASSERT_NOT_NULL(game);

    TEST_ASSERT_EQUAL_STRING("TM/HM", tmhm_pocket_ptr->name);
    TEST_ASSERT_EQUAL_STRING(game, tmhm_pocket_ptr->game);
    TEST_ASSERT_EQUAL(57, tmhm_pocket_ptr->capacity);

    pkmn_error_t error = PKMN_ERROR_NONE;

    // Make sure item slots start as correctly empty.
    for(int tm_num = 1; tm_num <= 50; ++tm_num)
    {
        char name[5] = {0};
        snprintf(name, sizeof(name), "TM%02d", tm_num);
        check_item_at_index(tmhm_pocket_ptr, tm_num-1, name, 0);
    }
    for(int hm_num = 1; hm_num <= 7; ++hm_num)
    {
        char name[5] = {0};
        snprintf(name, sizeof(name), "HM%02d", hm_num);
        check_item_at_index(tmhm_pocket_ptr, hm_num-1+50, name, 0);
    }

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        tmhm_pocket_ptr,
        "TM01"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Potion", "Bicycle", "Master Ball"};
    test_item_list_invalid_items(tmhm_pocket_ptr, wrong_pocket_item_names, 3);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"TM51", "HM08"};
    test_item_list_invalid_items(tmhm_pocket_ptr, wrong_generation_item_names, 2);

    // Start adding and removing items, and make sure the numbers are accurate.
    for(int tm_num = 1; tm_num <= 50; ++tm_num)
    {
        char name[5] = "";
        snprintf(name, sizeof(name), "TM%02d", tm_num);

        error = pkmn_item_list_add(
                    tmhm_pocket_ptr,
                    name,
                    50
                );
        check_num_items(tmhm_pocket_ptr, tm_num);
        check_item_at_index(tmhm_pocket_ptr, tm_num-1, name, 50);
    }
    for(int tm_num = 50; tm_num >= 1; --tm_num)
    {
        char name[5] = "";
        snprintf(name, sizeof(name), "TM%02d", tm_num);

        error = pkmn_item_list_remove(
                    tmhm_pocket_ptr,
                    name,
                    50
                );
        check_num_items(tmhm_pocket_ptr, tm_num-1);
        check_item_at_index(tmhm_pocket_ptr, tm_num-1, name, 0);
    }

    for(int hm_num = 1; hm_num <= 7; ++hm_num)
    {
        char name[5] = "";
        snprintf(name, sizeof(name), "HM%02d", hm_num);

        error = pkmn_item_list_add(
                    tmhm_pocket_ptr,
                    name,
                    50
                );
        check_num_items(tmhm_pocket_ptr, hm_num);
        check_item_at_index(tmhm_pocket_ptr, hm_num-1+50, name, 50);
    }
    for(int hm_num = 7; hm_num >= 1; --hm_num)
    {
        char name[5] = "";
        snprintf(name, sizeof(name), "HM%02d", hm_num);

        error = pkmn_item_list_remove(
                    tmhm_pocket_ptr,
                    name,
                    50
                );
        check_num_items(tmhm_pocket_ptr, hm_num-1);
        check_item_at_index(tmhm_pocket_ptr, hm_num-1+50, name, 0);
    }

    struct pkmn_string_list valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                tmhm_pocket_ptr,
                &valid_items
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(57, valid_items.length);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gen2_item_pc_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    pkmn_item_list_t item_pc =
    {
        .name = NULL,
        .game = NULL,
        .capacity = 0,
        ._internal = NULL
    };

    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_item_list_init(
                "PC",
                game,
                &item_pc
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL_STRING("PC", item_pc.name);
    TEST_ASSERT_EQUAL_STRING(game, item_pc.game);
    TEST_ASSERT_EQUAL(50, item_pc.capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(&item_pc);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        &item_pc,
        "Potion"
    );

    // Make sure we can't add items from later generations.
    test_item_list_invalid_items(
        &item_pc,
        WRONG_GENERATION_ALL_POCKET_ITEM_NAMES,
        4
    );

    // Test setting items by index.
    test_item_list_set_item(
        &item_pc,
        ALL_POCKET_ITEM_NAMES,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        &item_pc,
        ALL_POCKET_ITEM_NAMES,
        8
    );

    struct pkmn_string_list valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                &item_pc,
                &valid_items
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    struct pkmn_string_list full_item_list =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_database_item_list(
                item_pc.game,
                &full_item_list
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(full_item_list.strings);
    TEST_ASSERT_TRUE(full_item_list.length > 0);

    TEST_ASSERT_EQUAL(full_item_list.length, valid_items.length);

    error = pkmn_string_list_free(&valid_items);
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_string_list_free(&full_item_list);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_list_free(&item_pc);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(item_pc._internal);
}

static void get_bag_pockets(
    pkmn_item_bag_t* item_bag_ptr,
    pkmn_item_list_t* item_pocket_ptr_out,
    pkmn_item_list_t* key_item_pocket_ptr_out,
    pkmn_item_list_t* ball_pocket_ptr_out,
    pkmn_item_list_t* tmhm_pocket_ptr_out
)
{
    TEST_ASSERT_NOT_NULL(item_bag_ptr);
    TEST_ASSERT_NOT_NULL(item_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(tmhm_pocket_ptr_out);

    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                "Items",
                item_pocket_ptr_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                "KeyItems",
                key_item_pocket_ptr_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                "Balls",
                ball_pocket_ptr_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                "TM/HM",
                tmhm_pocket_ptr_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
}

static void gen2_item_bag_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_item_bag_t item_bag =
    {
        .game = NULL,
        .pocket_names =
        {
            .strings = NULL,
            .length = 0
        },
        ._internal = NULL
    };

    error = pkmn_item_bag_init(
                game,
                &item_bag
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(item_bag._internal);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_item_bag_strerror(&item_bag)
    );

    TEST_ASSERT_NOT_NULL(item_bag.pocket_names.strings);
    TEST_ASSERT_EQUAL(4, item_bag.pocket_names.length);

    test_item_bag_pocket_names(&item_bag);

    pkmn_item_list_t item_pocket;
    pkmn_item_list_t key_item_pocket;
    pkmn_item_list_t ball_pocket;
    pkmn_item_list_t tmhm_pocket;

    get_bag_pockets(
        &item_bag,
        &item_pocket,
        &key_item_pocket,
        &ball_pocket,
        &tmhm_pocket
    );
    TEST_ASSERT_NOT_NULL(item_pocket._internal);
    TEST_ASSERT_NOT_NULL(key_item_pocket._internal);
    TEST_ASSERT_NOT_NULL(ball_pocket._internal);
    TEST_ASSERT_NOT_NULL(tmhm_pocket._internal);

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
            ALL_POCKET_ITEM_NAMES[item_index],
            5
        );
        PKMN_TEST_ASSERT_SUCCESS(error);
    }

    check_item_at_index(&item_pocket, 0, "Potion", 5);
    check_item_at_index(&item_pocket, 1, "Berry", 5);
    check_item_at_index(&item_pocket, 2, "None", 0);

    check_item_at_index(&key_item_pocket, 0, "Bicycle", 5);
    check_item_at_index(&key_item_pocket, 1, "SquirtBottle", 5);
    check_item_at_index(&key_item_pocket, 2, "None", 0);

    check_item_at_index(&ball_pocket, 0, "Great Ball", 5);
    check_item_at_index(&ball_pocket, 1, "Friend Ball", 5);
    check_item_at_index(&ball_pocket, 2, "None", 0);

    check_item_at_index(&tmhm_pocket, 0, "TM01", 0);
    check_item_at_index(&tmhm_pocket, 1, "TM02", 0);
    check_item_at_index(&tmhm_pocket, 27, "TM28", 5);
    check_item_at_index(&tmhm_pocket, 50, "HM01", 5);

    // Make sure removing items through the bag removes from the proper pockets.
    for(size_t item_index = 0; item_index < 8; ++item_index)
    {
        pkmn_item_bag_remove(
            &item_bag,
            ALL_POCKET_ITEM_NAMES[item_index],
            5
        );
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    check_num_items(&item_pocket, 0);
    check_num_items(&key_item_pocket, 0);
    check_num_items(&ball_pocket, 0);
    check_num_items(&tmhm_pocket, 0);

    check_item_at_index(&item_pocket, 0, "None", 0);
    check_item_at_index(&item_pocket, 1, "None", 0);
    check_item_at_index(&item_pocket, 2, "None", 0);

    check_item_at_index(&key_item_pocket, 0, "None", 0);
    check_item_at_index(&key_item_pocket, 1, "None", 0);
    check_item_at_index(&key_item_pocket, 2, "None", 0);

    check_item_at_index(&ball_pocket, 0, "None", 0);
    check_item_at_index(&ball_pocket, 1, "None", 0);
    check_item_at_index(&ball_pocket, 2, "None", 0);

    check_item_at_index(&tmhm_pocket, 0, "TM01", 0);
    check_item_at_index(&tmhm_pocket, 1, "TM02", 0);
    check_item_at_index(&tmhm_pocket, 27, "TM28", 0);
    check_item_at_index(&tmhm_pocket, 50, "HM01", 0);

    // Make sure Crystal-exclusive items can only be added in Crystal.
    if(!strcmp(game, "Crystal"))
    {
        for(size_t item_index = 0; item_index < 4; ++item_index)
        {
            error = pkmn_item_bag_add(
                        &item_bag,
                        CRYSTAL_ITEM_NAMES[item_index],
                        1
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        for(size_t item_index = 0; item_index < 4; ++item_index)
        {
            check_item_at_index(&key_item_pocket, item_index, CRYSTAL_ITEM_NAMES[item_index], 1);
        }
        for(size_t item_index = 0; item_index < 4; ++item_index)
        {
            error = pkmn_item_bag_remove(
                        &item_bag,
                        CRYSTAL_ITEM_NAMES[item_index],
                        1
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        for(size_t item_index = 0; item_index < 4; ++item_index)
        {
            check_item_at_index(&key_item_pocket, item_index, "None", 0);
        }
    }
    else
    {
        test_item_bag_invalid_items(
            &item_bag,
            CRYSTAL_ITEM_NAMES,
            4
        );
    }

    // Make sure we can't add items from later generations.
    test_item_bag_invalid_items(
        &item_bag,
        WRONG_GENERATION_ALL_POCKET_ITEM_NAMES,
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

#define GEN2_ITEM_TESTS(test_game) \
void test_gen2_item_pocket_ ## test_game () \
{ \
    pkmn_error_t error = PKMN_ERROR_NONE; \
 \
    pkmn_item_list_t item_pocket = \
    { \
        .name = NULL, \
        .game = NULL, \
        .capacity = 0, \
        ._internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "Items", \
                #test_game, \
                &item_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(item_pocket._internal); \
 \
    gen2_item_pocket_test( \
        &item_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&item_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(item_pocket._internal); \
} \
void test_gen2_key_item_pocket_ ## test_game () \
{ \
    pkmn_error_t error = PKMN_ERROR_NONE; \
 \
    pkmn_item_list_t key_item_pocket = \
    { \
        .name = NULL, \
        .game = NULL, \
        .capacity = 0, \
        ._internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "KeyItems", \
                #test_game, \
                &key_item_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(key_item_pocket._internal); \
 \
    gen2_key_item_pocket_test( \
        &key_item_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&key_item_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(key_item_pocket._internal); \
} \
void test_gen2_ball_pocket_ ## test_game () \
{ \
    pkmn_error_t error = PKMN_ERROR_NONE; \
 \
    pkmn_item_list_t ball_pocket = \
    { \
        .name = NULL, \
        .game = NULL, \
        .capacity = 0, \
        ._internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "Balls", \
                #test_game, \
                &ball_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(ball_pocket._internal); \
 \
    gen2_ball_pocket_test( \
        &ball_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&ball_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(ball_pocket._internal); \
} \
void test_gen2_tmhm_pocket_ ## test_game () \
{ \
    pkmn_error_t error = PKMN_ERROR_NONE; \
 \
    pkmn_item_list_t tmhm_pocket = \
    { \
        .name = NULL, \
        .game = NULL, \
        .capacity = 0, \
        ._internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "TM/HM", \
                #test_game, \
                &tmhm_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(tmhm_pocket._internal); \
 \
    gen2_tmhm_pocket_test( \
        &tmhm_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&tmhm_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(tmhm_pocket._internal); \
} \
void test_gen2_item_pc_ ## test_game () \
{ \
    gen2_item_pc_test(#test_game); \
} \
void test_gen2_item_bag_ ## test_game () \
{ \
    gen2_item_bag_test(#test_game); \
}

GEN2_ITEM_TESTS(Gold)
GEN2_ITEM_TESTS(Silver)
GEN2_ITEM_TESTS(Crystal)
