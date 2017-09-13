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
    pkmn_item_list2_t* item_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(item_pocket);
    TEST_ASSERT_NOT_NULL(item_pocket->item_slots.item_slots);

    TEST_ASSERT_EQUAL_STRING("Items", item_pocket->name);
    TEST_ASSERT_EQUAL_STRING(game, item_pocket->game);
    TEST_ASSERT_EQUAL(0, item_pocket->num_items);
    TEST_ASSERT_EQUAL(20, item_pocket->capacity);
    TEST_ASSERT_EQUAL(20, item_pocket->item_slots.length);

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        item_pocket,
        "Potion"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Bicycle", "Master Ball", "HM01"};
    test_item_list_invalid_items(item_pocket, wrong_pocket_item_names, 3);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] =
    {
        "Black Flute", "Black Sludge", "Binding Band", "Beedrillite"
    };
    test_item_list_invalid_items(item_pocket, wrong_generation_item_names, 4);

    const char* item_names[] =
    {
        "Potion", "HP Up", "Wht Apricorn", "Lucky Egg",
        "Flower Mail", "Burn Heal", "PSNCureBerry", "Stick"
    };

    // Test setting items by index.
    test_item_list_set_item(
        item_pocket,
        item_names,
        3
    );
    TEST_ASSERT_EQUAL(0, item_pocket->num_items);

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        item_pocket,
        item_names,
        8
    );

    pkmn_string_list_t valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = pkmn_item_list2_get_valid_items(
                             item_pocket,
                             &valid_items
                         );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gen2_key_item_pocket_test(
    pkmn_item_list2_t* key_item_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(key_item_pocket);
    TEST_ASSERT_NOT_NULL(key_item_pocket->item_slots.item_slots);

    pkmn_error_t error = PKMN_ERROR_NONE;

    TEST_ASSERT_EQUAL_STRING("KeyItems", key_item_pocket->name);
    TEST_ASSERT_EQUAL_STRING(game, key_item_pocket->game);
    TEST_ASSERT_EQUAL(0, key_item_pocket->num_items);
    TEST_ASSERT_EQUAL(26, key_item_pocket->capacity);
    TEST_ASSERT_EQUAL(26, key_item_pocket->item_slots.length);

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(key_item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        key_item_pocket,
        "Bicycle"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Potion", "Master Ball", "HM01"};
    test_item_list_invalid_items(key_item_pocket, wrong_pocket_item_names, 3);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] =
    {
        "Mach Bike", "Jade Orb", "Light Stone", "Aqua Suit"
    };
    test_item_list_invalid_items(key_item_pocket, wrong_generation_item_names, 4);

    // Crystal-specific items.
    bool is_crystal = (bool)(!strcmp(key_item_pocket->game, "Crystal"));
    for(size_t i = 0; i < sizeof(CRYSTAL_ITEM_NAMES)/sizeof(CRYSTAL_ITEM_NAMES[0]); ++i)
    {
        error = pkmn_item_list2_add(
                    key_item_pocket,
                    CRYSTAL_ITEM_NAMES[i],
                    1
                );
        TEST_ASSERT_EQUAL(
            is_crystal ? PKMN_ERROR_NONE : PKMN_ERROR_INVALID_ARGUMENT,
            error
        );

        error = pkmn_item_list2_remove(
                    key_item_pocket,
                    CRYSTAL_ITEM_NAMES[i],
                    1
                );
        TEST_ASSERT_EQUAL(
            is_crystal ? PKMN_ERROR_NONE : PKMN_ERROR_INVALID_ARGUMENT,
            error
        );

        TEST_ASSERT_EQUAL(0, key_item_pocket->num_items);
    }

    const char* item_names[] =
    {
        "Bicycle", "Basement Key", "SecretPotion", "Mystery Egg",
        "Silver Wing", "Lost Item", "SquirtBottle", "Rainbow Wing"
    };

    // Test setting items by index.
    test_item_list_set_item(
        key_item_pocket,
        item_names,
        3
    );
    TEST_ASSERT_EQUAL(0, key_item_pocket->num_items);

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        key_item_pocket,
        item_names,
        8
    );

    pkmn_string_list_t valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_item_list2_get_valid_items(
                             key_item_pocket,
                             &valid_items
                         );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gen2_ball_pocket_test(
    pkmn_item_list2_t* ball_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(ball_pocket);
    TEST_ASSERT_NOT_NULL(ball_pocket->item_slots.item_slots);

    TEST_ASSERT_EQUAL_STRING("Balls", ball_pocket->name);
    TEST_ASSERT_EQUAL_STRING(game, ball_pocket->game);
    TEST_ASSERT_EQUAL(0, ball_pocket->num_items);
    TEST_ASSERT_EQUAL(12, ball_pocket->capacity);
    TEST_ASSERT_EQUAL(12, ball_pocket->item_slots.length);

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(ball_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        ball_pocket,
        "Master Ball"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Potion", "Bicycle", "HM01"};
    test_item_list_invalid_items(ball_pocket, wrong_pocket_item_names, 3);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"Premier Ball", "Heal Ball", "Dream Ball"};
    test_item_list_invalid_items(ball_pocket, wrong_generation_item_names, 3);

    const char* item_names[] =
    {
        "Great Ball", "Poké Ball", "Park Ball", "Fast Ball",
        "Master Ball", "Friend Ball", "Love Ball", "Level Ball"
    };

    // Test setting items by index.
    test_item_list_set_item(
        ball_pocket,
        item_names,
        3
    );
    TEST_ASSERT_EQUAL(0, ball_pocket->num_items);

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        ball_pocket,
        item_names,
        8
    );

    pkmn_string_list_t valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = pkmn_item_list2_get_valid_items(
                             ball_pocket,
                             &valid_items
                         );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gen2_tmhm_pocket_test(
    pkmn_item_list2_t* tmhm_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(tmhm_pocket);
    TEST_ASSERT_NOT_NULL(tmhm_pocket->item_slots.item_slots);

    TEST_ASSERT_EQUAL_STRING("TM/HM", tmhm_pocket->name);
    TEST_ASSERT_EQUAL_STRING(game, tmhm_pocket->game);
    TEST_ASSERT_EQUAL(0, tmhm_pocket->num_items);
    TEST_ASSERT_EQUAL(57, tmhm_pocket->capacity);
    TEST_ASSERT_EQUAL(57, tmhm_pocket->item_slots.length);

    pkmn_error_t error = PKMN_ERROR_NONE;

    // Make sure item slots start as correctly empty.
    for(int i = 1; i <= 50; ++i)
    {
        char name[5] = {0};
        snprintf(name, sizeof(name), "TM%02d", i);
        TEST_ASSERT_EQUAL_STRING(name, tmhm_pocket->item_slots.item_slots[i-1].item);
        TEST_ASSERT_EQUAL(0, tmhm_pocket->item_slots.item_slots[i-1].amount);
    }
    for(int i = 1; i <= 7; ++i)
    {
        char name[5] = {0};
        snprintf(name, sizeof(name), "HM%02d", i);
        TEST_ASSERT_EQUAL_STRING(name, tmhm_pocket->item_slots.item_slots[i-1+50].item);
        TEST_ASSERT_EQUAL(0, tmhm_pocket->item_slots.item_slots[i-1+50].amount);
    }

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        tmhm_pocket,
        "TM01"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Potion", "Bicycle", "Master Ball"};
    test_item_list_invalid_items(tmhm_pocket, wrong_pocket_item_names, 3);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"TM51", "HM08"};
    test_item_list_invalid_items(tmhm_pocket, wrong_generation_item_names, 2);

    // Start adding and removing items, and make sure the numbers are accurate.
    for(int i = 1; i <= 50; ++i) {
        char name[5] = "";
        snprintf(name, sizeof(name), "TM%02d", i);

        error = pkmn_item_list2_add(
                    tmhm_pocket,
                    name,
                    50
                );
        TEST_ASSERT_EQUAL(i, tmhm_pocket->num_items);
        TEST_ASSERT_EQUAL_STRING(name, tmhm_pocket->item_slots.item_slots[i-1].item);
        TEST_ASSERT_EQUAL(50, tmhm_pocket->item_slots.item_slots[i-1].amount);
    }
    for(int i = 50; i >= 1; --i) {
        char name[5] = "";
        snprintf(name, sizeof(name), "TM%02d", i);

        error = pkmn_item_list2_remove(
                    tmhm_pocket,
                    name,
                    50
                );
        TEST_ASSERT_EQUAL(i-1, tmhm_pocket->num_items);
        TEST_ASSERT_EQUAL_STRING(name, tmhm_pocket->item_slots.item_slots[i-1].item);
        TEST_ASSERT_EQUAL(0, tmhm_pocket->item_slots.item_slots[i-1].amount);
    }

    for(int i = 1; i <= 7; ++i) {
        char name[5] = "";
        snprintf(name, sizeof(name), "HM%02d", i);

        error = pkmn_item_list2_add(
                    tmhm_pocket,
                    name,
                    50
                );
        TEST_ASSERT_EQUAL(i, tmhm_pocket->num_items);
        TEST_ASSERT_EQUAL_STRING(name, tmhm_pocket->item_slots.item_slots[i-1+50].item);
        TEST_ASSERT_EQUAL(50, tmhm_pocket->item_slots.item_slots[i-1+50].amount);
    }
    for(int i = 7; i >= 1; --i) {
        char name[5] = "";
        snprintf(name, sizeof(name), "HM%02d", i);

        error = pkmn_item_list2_remove(
                    tmhm_pocket,
                    name,
                    50
                );
        TEST_ASSERT_EQUAL(i-1, tmhm_pocket->num_items);
        TEST_ASSERT_EQUAL_STRING(name, tmhm_pocket->item_slots.item_slots[i-1+50].item);
        TEST_ASSERT_EQUAL(0, tmhm_pocket->item_slots.item_slots[i-1+50].amount);
    }

    pkmn_string_list_t valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_item_list2_get_valid_items(
                tmhm_pocket,
                &valid_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
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

    pkmn_item_list2_t item_pc =
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

    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_item_list2_init(
                "PC",
                game,
                &item_pc
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_NOT_NULL(item_pc.item_slots.item_slots);

    TEST_ASSERT_EQUAL_STRING("PC", item_pc.name);
    TEST_ASSERT_EQUAL_STRING(game, item_pc.game);
    TEST_ASSERT_EQUAL(0, item_pc.num_items);
    TEST_ASSERT_EQUAL(50, item_pc.capacity);
    TEST_ASSERT_EQUAL(50, item_pc.item_slots.length);

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(&item_pc);

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
    TEST_ASSERT_EQUAL(0, item_pc.num_items);

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        &item_pc,
        ALL_POCKET_ITEM_NAMES,
        8
    );

    pkmn_string_list_t valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_item_list2_get_valid_items(
                &item_pc,
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
                item_pc.game,
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

    error = pkmn_item_list2_free(&item_pc);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(item_pc._internal);
}

static void get_bag_pockets(
    pkmn_item_bag2_t* item_bag_ptr,
    pkmn_item_list2_t** item_pocket_ptr_out,
    pkmn_item_list2_t** key_item_pocket_ptr_out,
    pkmn_item_list2_t** ball_pocket_ptr_out,
    pkmn_item_list2_t** tmhm_pocket_ptr_out
)
{
    TEST_ASSERT_NOT_NULL(item_bag_ptr);
    TEST_ASSERT_NOT_NULL(item_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(tmhm_pocket_ptr_out);

    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_item_bag2_get_pocket(
                item_bag_ptr,
                "Items",
                item_pocket_ptr_out
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_item_bag2_get_pocket(
                item_bag_ptr,
                "KeyItems",
                key_item_pocket_ptr_out
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_item_bag2_get_pocket(
                item_bag_ptr,
                "Balls",
                ball_pocket_ptr_out
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_item_bag2_get_pocket(
                item_bag_ptr,
                "TM/HM",
                tmhm_pocket_ptr_out
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void gen2_item_bag_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_item_bag2_t item_bag =
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
            .num_pockets = 0
        },
        ._internal = NULL
    };

    error = pkmn_item_bag2_init(
                game,
                &item_bag
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_bag._internal);

    TEST_ASSERT_EQUAL(4, item_bag.pockets.num_pockets);
    TEST_ASSERT_EQUAL(4, item_bag.pockets.pocket_names.length);

    // Make sure pockets are stored and accessible as expected.
    for(size_t i = 0; i < 4; ++i)
    {
        TEST_ASSERT_EQUAL_STRING(
            item_bag.pockets.pockets[i].name,
            item_bag.pockets.pocket_names.strings[i]
        );
        TEST_ASSERT_EQUAL_STRING(
            item_bag.pockets.pockets[i].game,
            item_bag.game
        );
    }

    const char* POCKET_NAMES[] = {"Items", "KeyItems", "Balls", "TM/HM"};
    for(size_t i = 0; i < 4; ++i)
    {
        pkmn_item_list2_t* item_list_ptr = NULL;
        error = pkmn_item_bag2_get_pocket(
                    &item_bag,
                    POCKET_NAMES[i],
                    &item_list_ptr
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(item_list_ptr);
        TEST_ASSERT_EQUAL_STRING(
            item_list_ptr->name,
            POCKET_NAMES[i]
        );
    }

    pkmn_item_list2_t* item_pocket_ptr = NULL;
    pkmn_item_list2_t* key_item_pocket_ptr = NULL;
    pkmn_item_list2_t* ball_pocket_ptr = NULL;
    pkmn_item_list2_t* tmhm_pocket_ptr = NULL;

    get_bag_pockets(
        &item_bag,
        &item_pocket_ptr,
        &key_item_pocket_ptr,
        &ball_pocket_ptr,
        &tmhm_pocket_ptr
    );
    TEST_ASSERT_NOT_NULL(item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr);
    TEST_ASSERT_NOT_NULL(tmhm_pocket_ptr);

    gen2_item_pocket_test(item_pocket_ptr, game);
    gen2_key_item_pocket_test(key_item_pocket_ptr, game);
    gen2_ball_pocket_test(ball_pocket_ptr, game);
    gen2_tmhm_pocket_test(tmhm_pocket_ptr, game);

    error = pkmn_item_bag2_free(&item_bag);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(item_bag._internal);

    error = pkmn_item_bag2_init(
                game,
                &item_bag
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_bag._internal);

    get_bag_pockets(
        &item_bag,
        &item_pocket_ptr,
        &key_item_pocket_ptr,
        &ball_pocket_ptr,
        &tmhm_pocket_ptr
    );
    TEST_ASSERT_NOT_NULL(item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr);
    TEST_ASSERT_NOT_NULL(tmhm_pocket_ptr);

    // Make sure adding items through the bag adds to the proper pockets.
    TEST_ASSERT_EQUAL(0, item_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, key_item_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, ball_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, tmhm_pocket_ptr->num_items);
    for(size_t i = 0; i < 8; ++i)
    {
        pkmn_item_bag2_add(
            &item_bag,
            ALL_POCKET_ITEM_NAMES[i],
            5
        );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    TEST_ASSERT_EQUAL_STRING("Potion", item_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, item_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("Berry", item_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(5, item_pocket_ptr->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", item_pocket_ptr->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, item_pocket_ptr->item_slots.item_slots[2].amount);

    TEST_ASSERT_EQUAL_STRING("Bicycle", key_item_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, key_item_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("SquirtBottle", key_item_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(5, key_item_pocket_ptr->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", key_item_pocket_ptr->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, key_item_pocket_ptr->item_slots.item_slots[2].amount);

    TEST_ASSERT_EQUAL_STRING("Great Ball", ball_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, ball_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("Friend Ball", ball_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(5, ball_pocket_ptr->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", ball_pocket_ptr->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, ball_pocket_ptr->item_slots.item_slots[2].amount);

    TEST_ASSERT_EQUAL_STRING("TM01", tmhm_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(0, tmhm_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("TM02", tmhm_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, tmhm_pocket_ptr->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("TM28", tmhm_pocket_ptr->item_slots.item_slots[27].item);
    TEST_ASSERT_EQUAL(5, tmhm_pocket_ptr->item_slots.item_slots[27].amount);
    TEST_ASSERT_EQUAL_STRING("HM01", tmhm_pocket_ptr->item_slots.item_slots[50].item);
    TEST_ASSERT_EQUAL(5, tmhm_pocket_ptr->item_slots.item_slots[50].amount);

    // Make sure removing items through the bag removes from the proper pockets.
    for(size_t i = 0; i < 8; ++i)
    {
        pkmn_item_bag2_remove(
            &item_bag,
            ALL_POCKET_ITEM_NAMES[i],
            5
        );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }
    TEST_ASSERT_EQUAL(0, item_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, key_item_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, ball_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, tmhm_pocket_ptr->num_items);

    TEST_ASSERT_EQUAL_STRING("None", item_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(0, item_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", item_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, item_pocket_ptr->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", item_pocket_ptr->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, item_pocket_ptr->item_slots.item_slots[2].amount);

    TEST_ASSERT_EQUAL_STRING("None", key_item_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(0, key_item_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", key_item_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, key_item_pocket_ptr->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", key_item_pocket_ptr->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, key_item_pocket_ptr->item_slots.item_slots[2].amount);

    TEST_ASSERT_EQUAL_STRING("None", ball_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(0, ball_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", ball_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, ball_pocket_ptr->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", ball_pocket_ptr->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, ball_pocket_ptr->item_slots.item_slots[2].amount);

    TEST_ASSERT_EQUAL_STRING("TM01", tmhm_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(0, tmhm_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("TM02", tmhm_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, tmhm_pocket_ptr->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("TM28", tmhm_pocket_ptr->item_slots.item_slots[27].item);
    TEST_ASSERT_EQUAL(0, tmhm_pocket_ptr->item_slots.item_slots[27].amount);
    TEST_ASSERT_EQUAL_STRING("HM01", tmhm_pocket_ptr->item_slots.item_slots[50].item);
    TEST_ASSERT_EQUAL(0, tmhm_pocket_ptr->item_slots.item_slots[50].amount);

    if(!strcmp(game, "Crystal"))
    {
        for(size_t i = 0; i < 4; ++i)
        {
            error = pkmn_item_bag2_add(
                        &item_bag,
                        CRYSTAL_ITEM_NAMES[i],
                        1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        }
        for(size_t i = 0; i < 4; ++i)
        {
            TEST_ASSERT_EQUAL_STRING(
                CRYSTAL_ITEM_NAMES[i],
                key_item_pocket_ptr->item_slots.item_slots[i].item
            );
            TEST_ASSERT_EQUAL(1, key_item_pocket_ptr->item_slots.item_slots[i].amount);
        }
        for(size_t i = 0; i < 4; ++i)
        {
            error = pkmn_item_bag2_remove(
                        &item_bag,
                        CRYSTAL_ITEM_NAMES[i],
                        1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        }
        for(size_t i = 0; i < 4; ++i)
        {
            TEST_ASSERT_EQUAL_STRING("None", key_item_pocket_ptr->item_slots.item_slots[i].item);
            TEST_ASSERT_EQUAL(0, key_item_pocket_ptr->item_slots.item_slots[i].amount);
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
}

#define GEN2_ITEM_TESTS(test_game) \
void test_gen2_item_pocket_ ## test_game () \
{ \
    pkmn_error_t error = PKMN_ERROR_NONE; \
 \
    pkmn_item_list2_t item_pocket = \
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
    error = pkmn_item_list2_init( \
                "Items", \
                #test_game, \
                &item_pocket \
            ); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NOT_NULL(item_pocket._internal); \
 \
    gen2_item_pocket_test( \
        &item_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list2_free(&item_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NULL(item_pocket._internal); \
} \
void test_gen2_key_item_pocket_ ## test_game () \
{ \
    pkmn_error_t error = PKMN_ERROR_NONE; \
 \
    pkmn_item_list2_t key_item_pocket = \
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
    error = pkmn_item_list2_init( \
                "KeyItems", \
                #test_game, \
                &key_item_pocket \
            ); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NOT_NULL(key_item_pocket._internal); \
 \
    gen2_key_item_pocket_test( \
        &key_item_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list2_free(&key_item_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NULL(key_item_pocket._internal); \
} \
void test_gen2_ball_pocket_ ## test_game () \
{ \
    pkmn_error_t error = PKMN_ERROR_NONE; \
 \
    pkmn_item_list2_t ball_pocket = \
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
    error = pkmn_item_list2_init( \
                "Balls", \
                #test_game, \
                &ball_pocket \
            ); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NOT_NULL(ball_pocket._internal); \
 \
    gen2_ball_pocket_test( \
        &ball_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list2_free(&ball_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NULL(ball_pocket._internal); \
} \
void test_gen2_tmhm_pocket_ ## test_game () \
{ \
    pkmn_error_t error = PKMN_ERROR_NONE; \
 \
    pkmn_item_list2_t tmhm_pocket = \
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
    error = pkmn_item_list2_init( \
                "TM/HM", \
                #test_game, \
                &tmhm_pocket \
            ); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NOT_NULL(tmhm_pocket._internal); \
 \
    gen2_tmhm_pocket_test( \
        &tmhm_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list2_free(&tmhm_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
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
