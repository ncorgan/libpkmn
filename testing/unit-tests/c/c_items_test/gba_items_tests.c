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

static const char* ALL_POCKET_ITEM_NAMES[] =
{
    "Potion", "Mach Bike", "Great Ball", "TM01",
    "Aspear Berry", "Wailmer Pail", "Master Ball", "HM04",
};
static const char* WRONG_GAME_ALL_POCKET_ITEM_NAMES[] =
{
    "Pink Bow", "Black Sludge",
    "Ein File S", "Gonzap's Key",
    "GS Ball", "Poffin Items",
    "TM51",
    "Berry", "Occa Berry",
};

static void gba_item_pocket_test(
    struct pkmn_item_list* item_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(item_pocket_ptr->p_internal);
    TEST_ASSERT_NOT_NULL(game);

    size_t expected_capacity = 0;
    if(!strcmp(game, "Ruby") || !strcmp(game, "Sapphire"))
    {
        expected_capacity = 20;
    }
    else if(!strcmp(game, "Emerald"))
    {
        expected_capacity = 30;
    }
    else
    {
        expected_capacity = 42;
    }

    TEST_ASSERT_EQUAL_STRING("Items", item_pocket_ptr->name);
    TEST_ASSERT_EQUAL_STRING(game, item_pocket_ptr->game);
    TEST_ASSERT_EQUAL(expected_capacity, item_pocket_ptr->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(item_pocket_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        item_pocket_ptr,
        "Potion"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Bicycle", "Master Ball", "HM01", "Razz Berry"};
    test_item_list_invalid_items(item_pocket_ptr, wrong_pocket_item_names, 4);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"};
    test_item_list_invalid_items(item_pocket_ptr, wrong_generation_item_names, 4);

    // Make sure we can't add items from Gamecube games.
    const char* gamecube_item_names[] = {"Time Flute", "Poké Snack"};
    test_item_list_invalid_items(item_pocket_ptr, gamecube_item_names, 2);

    const char* item_names[] =
    {
        "Potion", "Orange Mail", "Lava Cookie", "Stardust",
        "Shadow Mail", "Pink Scarf", "Antidote", "Green Shard"
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
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = pkmn_item_list_get_valid_items(
                             item_pocket_ptr,
                             &valid_items
                         );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.pp_strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.pp_strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gba_key_item_pocket_test(
    struct pkmn_item_list* key_item_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr->p_internal);
    TEST_ASSERT_NOT_NULL(game);

    size_t expected_capacity = 0;
    if(!strcmp(game, "Ruby") || !strcmp(game, "Sapphire"))
    {
        expected_capacity = 20;
    }
    else
    {
        expected_capacity = 30;
    }

    TEST_ASSERT_EQUAL_STRING("Key Items", key_item_pocket_ptr->name);
    TEST_ASSERT_EQUAL_STRING(game, key_item_pocket_ptr->game);
    TEST_ASSERT_EQUAL(expected_capacity, key_item_pocket_ptr->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(key_item_pocket_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        key_item_pocket_ptr,
        "Basement Key"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Potion", "Master Ball", "HM01", "Razz Berry"};
    test_item_list_invalid_items(key_item_pocket_ptr, wrong_pocket_item_names, 4);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"};
    test_item_list_invalid_items(key_item_pocket_ptr, wrong_generation_item_names, 4);

    // Make sure we can't add items from incompatible Generation III games.
    const char* gcn_items[] = {"Ein File S", "Powerup Part", "Gonzap's Key", "Krane Memo 1"};
    const char* frlg_items[] = {"Helix Fossil", "Tea", "Ruby"};;
    const char* emerald_items[] = {"Magma Emblem", "Old Sea Map"};

    test_item_list_invalid_items(key_item_pocket_ptr, gcn_items, 4);
    if(!strcmp(game, "Ruby") || !strcmp(game, "Sapphire"))
    {
        test_item_list_invalid_items(key_item_pocket_ptr, frlg_items, 3);
    }
    if(strcmp(game, "Emerald"))
    {
        test_item_list_invalid_items(key_item_pocket_ptr, emerald_items, 2);
    }

    const char* item_names[] =
    {
        "Wailmer Pail", "Basement Key", "Meteorite", "Old Rod",
        "Red Orb", "Root Fossil", "Contest Pass", "Eon Ticket"
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
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = pkmn_item_list_get_valid_items(
                             key_item_pocket_ptr,
                             &valid_items
                         );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.pp_strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.pp_strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gba_ball_pocket_test(
    struct pkmn_item_list* ball_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr);
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr->p_internal);
    TEST_ASSERT_NOT_NULL(game);

    size_t expected_capacity = 0;
    if(!strcmp(game, "FireRed") || !strcmp(game, "LeafGreen"))
    {
        expected_capacity = 13;
    }
    else
    {
        expected_capacity = 16;
    }

    TEST_ASSERT_EQUAL_STRING("Poké Balls", ball_pocket_ptr->name);
    TEST_ASSERT_EQUAL_STRING(game, ball_pocket_ptr->game);
    TEST_ASSERT_EQUAL(expected_capacity, ball_pocket_ptr->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(ball_pocket_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        ball_pocket_ptr,
        "Master Ball"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Bicycle", "Potion", "HM01", "Razz Berry"};
    test_item_list_invalid_items(ball_pocket_ptr, wrong_pocket_item_names, 4);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"Moon Ball", "Heal Ball", "Dream Ball"};
    test_item_list_invalid_items(ball_pocket_ptr, wrong_generation_item_names, 3);

    const char* item_names[] =
    {
        "Master Ball", "Ultra Ball", "Great Ball", "Poké Ball",
        "Safari Ball", "Net Ball", "Dive Ball", "Nest Ball"
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
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = pkmn_item_list_get_valid_items(
                             ball_pocket_ptr,
                             &valid_items
                         );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.pp_strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.pp_strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gba_tmhm_pocket_test(
    struct pkmn_item_list* tmhm_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(tmhm_pocket_ptr);
    TEST_ASSERT_NOT_NULL(tmhm_pocket_ptr->p_internal);
    TEST_ASSERT_NOT_NULL(game);

    const char* expected_pocket_name = NULL;
    size_t expected_capacity = 0;
    if(!strcmp(game, "FireRed") || !strcmp(game, "LeafGreen"))
    {
        expected_pocket_name = "TM Case";
        expected_capacity = 58;
    }
    else
    {
        expected_pocket_name = "TMs & HMs";
        expected_capacity = 64;
    }

    TEST_ASSERT_EQUAL_STRING(expected_pocket_name, tmhm_pocket_ptr->name);
    TEST_ASSERT_EQUAL_STRING(game, tmhm_pocket_ptr->game);
    TEST_ASSERT_EQUAL(expected_capacity, tmhm_pocket_ptr->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(tmhm_pocket_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        tmhm_pocket_ptr,
        "TM01"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Bicycle", "Potion", "Great Ball", "Razz Berry"};
    test_item_list_invalid_items(tmhm_pocket_ptr, wrong_pocket_item_names, 4);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"TM51"};
    test_item_list_invalid_items(tmhm_pocket_ptr, wrong_generation_item_names, 1);

    const char* item_names[] =
    {
        "TM01", "HM01", "TM02", "HM02",
        "TM03", "HM03", "TM04", "HM04"
    };

    // Test setting items by index.
    test_item_list_set_item(
        tmhm_pocket_ptr,
        item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        tmhm_pocket_ptr,
        item_names,
        8
    );

    struct pkmn_string_list valid_items =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = pkmn_item_list_get_valid_items(
                             tmhm_pocket_ptr,
                             &valid_items
                         );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.pp_strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.pp_strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gba_berry_pocket_test(
    struct pkmn_item_list* berry_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(berry_pocket_ptr);
    TEST_ASSERT_NOT_NULL(berry_pocket_ptr->p_internal);
    TEST_ASSERT_NOT_NULL(game);

    const char* expected_pocket_name = NULL;
    size_t expected_capacity = 0;
    if(!strcmp(game, "FireRed") || !strcmp(game, "LeafGreen"))
    {
        expected_pocket_name = "Berry Pouch";
        expected_capacity = 43;
    }
    else
    {
        expected_pocket_name = "Berries";
        expected_capacity = 46;
    }

    TEST_ASSERT_EQUAL_STRING(expected_pocket_name, berry_pocket_ptr->name);
    TEST_ASSERT_EQUAL_STRING(game, berry_pocket_ptr->game);
    TEST_ASSERT_EQUAL(expected_capacity, berry_pocket_ptr->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(berry_pocket_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        berry_pocket_ptr,
        "Oran Berry"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Bicycle", "Potion", "Great Ball", "TM01"};
    test_item_list_invalid_items(berry_pocket_ptr, wrong_pocket_item_names, 4);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"Berry", "Occa Berry", "Roseli Berry"};
    test_item_list_invalid_items(berry_pocket_ptr, wrong_generation_item_names, 3);

    const char* item_names[] =
    {
        "Cheri Berry", "Razz Berry", "Lum Berry", "Pinap Berry",
        "Aspear Berry", "Iapapa Berry", "Wiki Berry", "Apicot Berry"
    };

    // Test setting items by index.
    test_item_list_set_item(
        berry_pocket_ptr,
        item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        berry_pocket_ptr,
        item_names,
        8
    );

    struct pkmn_string_list valid_items =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = pkmn_item_list_get_valid_items(
                             berry_pocket_ptr,
                             &valid_items
                         );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.pp_strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.pp_strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gba_item_pc_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    struct pkmn_item_list item_pc =
    {
        .name = NULL,
        .game = NULL,
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

    TEST_ASSERT_EQUAL_STRING("PC", item_pc.name);
    TEST_ASSERT_EQUAL_STRING(game, item_pc.game);
    TEST_ASSERT_EQUAL(50, item_pc.capacity);
    TEST_ASSERT_NOT_NULL(item_pc.p_internal);

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
        WRONG_GAME_ALL_POCKET_ITEM_NAMES,
        9
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
        .pp_strings = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                &item_pc,
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
                item_pc.game,
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

    error = pkmn_item_list_free(&item_pc);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(item_pc.p_internal);
}

static void get_bag_pockets(
    struct pkmn_item_bag* item_bag_ptr,
    struct pkmn_item_list* item_pocket_ptr_out,
    struct pkmn_item_list* key_item_pocket_ptr_out,
    struct pkmn_item_list* ball_pocket_ptr_out,
    struct pkmn_item_list* tmhm_pocket_ptr_out,
    const char* tmhm_pocket_name,
    struct pkmn_item_list* berry_pocket_ptr_out,
    const char* berry_pocket_name
)
{
    TEST_ASSERT_NOT_NULL(item_bag_ptr);
    TEST_ASSERT_NOT_NULL(item_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(tmhm_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(tmhm_pocket_name);
    TEST_ASSERT_NOT_NULL(berry_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(berry_pocket_name);

    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                "Items",
                item_pocket_ptr_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                "Key Items",
                key_item_pocket_ptr_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                "Poké Balls",
                ball_pocket_ptr_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                tmhm_pocket_name,
                tmhm_pocket_ptr_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                berry_pocket_name,
                berry_pocket_ptr_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
}

static void gba_item_bag_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_item_bag item_bag =
    {
        .game = NULL,
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
    TEST_ASSERT_EQUAL(5, item_bag.pocket_names.length);

    test_item_bag_pocket_names(&item_bag);

    const char* tmhm_pocket_name = NULL;
    const char* berry_pocket_name = NULL;
    if(!strcmp(game, "FireRed") || !strcmp(game, "LeafGreen"))
    {
        tmhm_pocket_name = "TM Case";
        berry_pocket_name = "Berry Pouch";
    }
    else
    {
        tmhm_pocket_name = "TMs & HMs";
        berry_pocket_name = "Berries";
    }

    struct pkmn_item_list item_pocket;
    struct pkmn_item_list key_item_pocket;
    struct pkmn_item_list ball_pocket;
    struct pkmn_item_list tmhm_pocket;
    struct pkmn_item_list berry_pocket;

    get_bag_pockets(
        &item_bag,
        &item_pocket,
        &key_item_pocket,
        &ball_pocket,
        &tmhm_pocket,
        tmhm_pocket_name,
        &berry_pocket,
        berry_pocket_name
    );
    TEST_ASSERT_NOT_NULL(item_pocket.p_internal);
    TEST_ASSERT_NOT_NULL(key_item_pocket.p_internal);
    TEST_ASSERT_NOT_NULL(ball_pocket.p_internal);
    TEST_ASSERT_NOT_NULL(tmhm_pocket.p_internal);
    TEST_ASSERT_NOT_NULL(berry_pocket.p_internal);

    gba_item_pocket_test(&item_pocket, game);
    gba_key_item_pocket_test(&key_item_pocket, game);
    gba_ball_pocket_test(&ball_pocket, game);
    gba_tmhm_pocket_test(&tmhm_pocket, game);
    gba_berry_pocket_test(&berry_pocket, game);

    // Make sure adding items through the bag adds to the proper pockets.
    check_num_items(&item_pocket, 0);
    check_num_items(&key_item_pocket, 0);
    check_num_items(&ball_pocket, 0);
    check_num_items(&tmhm_pocket, 0);
    check_num_items(&berry_pocket, 0);
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
    check_item_at_index(&item_pocket, 1, "None", 0);

    check_item_at_index(&key_item_pocket, 0, "Mach Bike", 5);
    check_item_at_index(&key_item_pocket, 1, "Wailmer Pail", 5);
    check_item_at_index(&key_item_pocket, 2, "None", 0);

    check_item_at_index(&ball_pocket, 0, "Great Ball", 5);
    check_item_at_index(&ball_pocket, 1, "Master Ball", 5);
    check_item_at_index(&ball_pocket, 2, "None", 0);

    check_item_at_index(&tmhm_pocket, 0, "TM01", 5);
    check_item_at_index(&tmhm_pocket, 1, "HM04", 5);
    check_item_at_index(&tmhm_pocket, 2, "None", 0);

    check_item_at_index(&berry_pocket, 0, "Aspear Berry", 5);
    check_item_at_index(&berry_pocket, 1, "None", 0);

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
    check_num_items(&berry_pocket, 0);

    check_item_at_index(&item_pocket, 0, "None", 0);
    check_item_at_index(&item_pocket, 1, "None", 0);

    check_item_at_index(&key_item_pocket, 0, "None", 0);
    check_item_at_index(&key_item_pocket, 1, "None", 0);
    check_item_at_index(&key_item_pocket, 2, "None", 0);

    check_item_at_index(&ball_pocket, 0, "None", 0);
    check_item_at_index(&ball_pocket, 1, "None", 0);
    check_item_at_index(&ball_pocket, 2, "None", 0);

    check_item_at_index(&tmhm_pocket, 0, "None", 0);
    check_item_at_index(&tmhm_pocket, 1, "None", 0);
    check_item_at_index(&tmhm_pocket, 2, "None", 0);

    check_item_at_index(&berry_pocket, 0, "None", 0);
    check_item_at_index(&berry_pocket, 1, "None", 0);

    // Make sure we can't add items from later generations.
    test_item_bag_invalid_items(
        &item_bag,
        WRONG_GAME_ALL_POCKET_ITEM_NAMES,
        9
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

    error = pkmn_item_list_free(&berry_pocket);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_free(&item_bag);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

#define GBA_ITEM_TESTS(test_game) \
void test_gba_item_pocket_ ## test_game () \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
 \
    struct pkmn_item_list item_pocket = \
    { \
        .name = NULL, \
        .game = NULL, \
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
    gba_item_pocket_test( \
        &item_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&item_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(item_pocket.p_internal); \
} \
void test_gba_key_item_pocket_ ## test_game () \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
 \
    struct pkmn_item_list key_item_pocket = \
    { \
        .name = NULL, \
        .game = NULL, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "Key Items", \
                #test_game, \
                &key_item_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(key_item_pocket.p_internal); \
 \
    gba_key_item_pocket_test( \
        &key_item_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&key_item_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(key_item_pocket.p_internal); \
} \
void test_gba_ball_pocket_ ## test_game () \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
 \
    struct pkmn_item_list ball_pocket = \
    { \
        .name = NULL, \
        .game = NULL, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "Poké Balls", \
                #test_game, \
                &ball_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(ball_pocket.p_internal); \
 \
    gba_ball_pocket_test( \
        &ball_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&ball_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(ball_pocket.p_internal); \
} \
void test_gba_tmhm_pocket_ ## test_game () \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
 \
    struct pkmn_item_list tmhm_pocket = \
    { \
        .name = NULL, \
        .game = NULL, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    const char* pocket_name = NULL; \
    if(!strcmp(#test_game, "FireRed") || !strcmp(#test_game, "LeafGreen")) \
    { \
        pocket_name = "TM Case"; \
    } \
    else \
    { \
        pocket_name = "TMs & HMs"; \
    } \
 \
    error = pkmn_item_list_init( \
                pocket_name, \
                #test_game, \
                &tmhm_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(tmhm_pocket.p_internal); \
 \
    gba_tmhm_pocket_test( \
        &tmhm_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&tmhm_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(tmhm_pocket.p_internal); \
} \
void test_gba_berry_pocket_ ## test_game () \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
 \
    struct pkmn_item_list berry_pocket = \
    { \
        .name = NULL, \
        .game = NULL, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    const char* pocket_name = NULL; \
    if(!strcmp(#test_game, "FireRed") || !strcmp(#test_game, "LeafGreen")) \
    { \
        pocket_name = "Berry Pouch"; \
    } \
    else \
    { \
        pocket_name = "Berries"; \
    } \
 \
    error = pkmn_item_list_init( \
                pocket_name, \
                #test_game, \
                &berry_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(berry_pocket.p_internal); \
 \
    gba_berry_pocket_test( \
        &berry_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&berry_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(berry_pocket.p_internal); \
} \
void test_gba_item_pc_ ## test_game () \
{ \
    gba_item_pc_test(#test_game); \
} \
void test_gba_item_bag_ ## test_game () \
{ \
    gba_item_bag_test(#test_game); \
}

GBA_ITEM_TESTS(Ruby)
GBA_ITEM_TESTS(Sapphire)
GBA_ITEM_TESTS(Emerald)
GBA_ITEM_TESTS(FireRed)
GBA_ITEM_TESTS(LeafGreen)
