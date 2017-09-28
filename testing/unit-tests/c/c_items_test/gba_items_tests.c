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
    pkmn_item_list_t* item_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(item_pocket);
    TEST_ASSERT_NOT_NULL(item_pocket->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(game);

    size_t capacity = 0;
    if(!strcmp(game, "Ruby") || !strcmp(game, "Sapphire"))
    {
        capacity = 20;
    }
    else if(!strcmp(game, "Emerald"))
    {
        capacity = 30;
    }
    else
    {
        capacity = 42;
    }

    TEST_ASSERT_EQUAL_STRING("Items", item_pocket->name);
    TEST_ASSERT_EQUAL_STRING(game, item_pocket->game);
    TEST_ASSERT_EQUAL(0, item_pocket->num_items);
    TEST_ASSERT_EQUAL(capacity, item_pocket->capacity);
    TEST_ASSERT_EQUAL(capacity, item_pocket->item_slots.length);

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        item_pocket,
        "Potion"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Bicycle", "Master Ball", "HM01", "Razz Berry"};
    test_item_list_invalid_items(item_pocket, wrong_pocket_item_names, 4);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"};
    test_item_list_invalid_items(item_pocket, wrong_generation_item_names, 4);

    // Make sure we can't add items from Gamecube games.
    const char* gamecube_item_names[] = {"Time Flute", "Poké Snack"};
    test_item_list_invalid_items(item_pocket, gamecube_item_names, 2);

    const char* item_names[] =
    {
        "Potion", "Orange Mail", "Lava Cookie", "Stardust",
        "Shadow Mail", "Pink Scarf", "Antidote", "Green Shard"
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
    pkmn_error_t error = pkmn_item_list_get_valid_items(
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

static void gba_key_item_pocket_test(
    pkmn_item_list_t* key_item_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(key_item_pocket);
    TEST_ASSERT_NOT_NULL(key_item_pocket->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(game);

    size_t capacity = 0;
    if(!strcmp(game, "Ruby") || !strcmp(game, "Sapphire"))
    {
        capacity = 20;
    }
    else
    {
        capacity = 30;
    }

    TEST_ASSERT_EQUAL_STRING("Key Items", key_item_pocket->name);
    TEST_ASSERT_EQUAL_STRING(game, key_item_pocket->game);
    TEST_ASSERT_EQUAL(0, key_item_pocket->num_items);
    TEST_ASSERT_EQUAL(capacity, key_item_pocket->capacity);
    TEST_ASSERT_EQUAL(capacity, key_item_pocket->item_slots.length);

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(key_item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        key_item_pocket,
        "Basement Key"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Potion", "Master Ball", "HM01", "Razz Berry"};
    test_item_list_invalid_items(key_item_pocket, wrong_pocket_item_names, 4);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"};
    test_item_list_invalid_items(key_item_pocket, wrong_generation_item_names, 4);

    // Make sure we can't add items from incompatible Generation III games.
    const char* gcn_items[] = {"Ein File S", "Powerup Part", "Gonzap's Key", "Krane Memo 1"};
    const char* frlg_items[] = {"Helix Fossil", "Tea", "Ruby"};;
    const char* emerald_items[] = {"Magma Emblem", "Old Sea Map"};
    
    test_item_list_invalid_items(key_item_pocket, gcn_items, 4);
    if(!strcmp(game, "Ruby") || !strcmp(game, "Sapphire"))
    {
        test_item_list_invalid_items(key_item_pocket, frlg_items, 3);
    }
    if(strcmp(game, "Emerald"))
    {
        test_item_list_invalid_items(key_item_pocket, emerald_items, 2);
    }

    const char* item_names[] =
    {
        "Wailmer Pail", "Basement Key", "Meteorite", "Old Rod",
        "Red Orb", "Root Fossil", "Contest Pass", "Eon Ticket"
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
    pkmn_error_t error = pkmn_item_list_get_valid_items(
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

static void gba_ball_pocket_test(
    pkmn_item_list_t* ball_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(ball_pocket);
    TEST_ASSERT_NOT_NULL(ball_pocket->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(game);

    size_t capacity = 0;
    if(!strcmp(game, "FireRed") || !strcmp(game, "LeafGreen"))
    {
        capacity = 13;
    }
    else
    {
        capacity = 16;
    }

    TEST_ASSERT_EQUAL_STRING("Poké Balls", ball_pocket->name);
    TEST_ASSERT_EQUAL_STRING(game, ball_pocket->game);
    TEST_ASSERT_EQUAL(0, ball_pocket->num_items);
    TEST_ASSERT_EQUAL(capacity, ball_pocket->capacity);
    TEST_ASSERT_EQUAL(capacity, ball_pocket->item_slots.length);

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(ball_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        ball_pocket,
        "Master Ball"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Bicycle", "Potion", "HM01", "Razz Berry"};
    test_item_list_invalid_items(ball_pocket, wrong_pocket_item_names, 4);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"Moon Ball", "Heal Ball", "Dream Ball"};
    test_item_list_invalid_items(ball_pocket, wrong_generation_item_names, 3);

    const char* item_names[] =
    {
        "Master Ball", "Ultra Ball", "Great Ball", "Poké Ball",
        "Safari Ball", "Net Ball", "Dive Ball", "Nest Ball"
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
    pkmn_error_t error = pkmn_item_list_get_valid_items(
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

static void gba_tmhm_pocket_test(
    pkmn_item_list_t* tmhm_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(tmhm_pocket);
    TEST_ASSERT_NOT_NULL(tmhm_pocket->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(game);

    const char* pocket_name = NULL;
    size_t capacity = 0;
    if(!strcmp(game, "FireRed") || !strcmp(game, "LeafGreen"))
    {
        pocket_name = "TM Case";
        capacity = 58;
    }
    else
    {
        pocket_name = "TMs & HMs";
        capacity = 64;
    }

    TEST_ASSERT_EQUAL_STRING(pocket_name, tmhm_pocket->name);
    TEST_ASSERT_EQUAL_STRING(game, tmhm_pocket->game);
    TEST_ASSERT_EQUAL(0, tmhm_pocket->num_items);
    TEST_ASSERT_EQUAL(capacity, tmhm_pocket->capacity);
    TEST_ASSERT_EQUAL(capacity, tmhm_pocket->item_slots.length);

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(tmhm_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        tmhm_pocket,
        "TM01"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Bicycle", "Potion", "Great Ball", "Razz Berry"};
    test_item_list_invalid_items(tmhm_pocket, wrong_pocket_item_names, 4);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"TM51"};
    test_item_list_invalid_items(tmhm_pocket, wrong_generation_item_names, 1);

    const char* item_names[] =
    {
        "TM01", "HM01", "TM02", "HM02",
        "TM03", "HM03", "TM04", "HM04"
    };

    // Test setting items by index.
    test_item_list_set_item(
        tmhm_pocket,
        item_names,
        3
    );
    TEST_ASSERT_EQUAL(0, tmhm_pocket->num_items);

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        tmhm_pocket,
        item_names,
        8
    );

    pkmn_string_list_t valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = pkmn_item_list_get_valid_items(
                             tmhm_pocket,
                             &valid_items
                         );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gba_berry_pocket_test(
    pkmn_item_list_t* berry_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(berry_pocket);
    TEST_ASSERT_NOT_NULL(berry_pocket->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(game);

    const char* pocket_name = NULL;
    size_t capacity = 0;
    if(!strcmp(game, "FireRed") || !strcmp(game, "LeafGreen"))
    {
        pocket_name = "Berry Pouch";
        capacity = 43;
    }
    else
    {
        pocket_name = "Berries";
        capacity = 46;
    }

    TEST_ASSERT_EQUAL_STRING(pocket_name, berry_pocket->name);
    TEST_ASSERT_EQUAL_STRING(game, berry_pocket->game);
    TEST_ASSERT_EQUAL(0, berry_pocket->num_items);
    TEST_ASSERT_EQUAL(capacity, berry_pocket->capacity);
    TEST_ASSERT_EQUAL(capacity, berry_pocket->item_slots.length);

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(berry_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        berry_pocket,
        "Oran Berry"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Bicycle", "Potion", "Great Ball", "TM01"};
    test_item_list_invalid_items(berry_pocket, wrong_pocket_item_names, 4);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"Berry", "Occa Berry", "Roseli Berry"};
    test_item_list_invalid_items(berry_pocket, wrong_generation_item_names, 3);

    const char* item_names[] =
    {
        "Cheri Berry", "Razz Berry", "Lum Berry", "Pinap Berry",
        "Aspear Berry", "Iapapa Berry", "Wiki Berry", "Apicot Berry"
    };

    // Test setting items by index.
    test_item_list_set_item(
        berry_pocket,
        item_names,
        3
    );
    TEST_ASSERT_EQUAL(0, berry_pocket->num_items);

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        berry_pocket,
        item_names,
        8
    );

    pkmn_string_list_t valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = pkmn_item_list_get_valid_items(
                             berry_pocket,
                             &valid_items
                         );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gba_item_pc_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

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

    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_item_list_init(
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
        WRONG_GAME_ALL_POCKET_ITEM_NAMES,
        9
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
    error = pkmn_item_list_get_valid_items(
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

    error = pkmn_item_list_free(&item_pc);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(item_pc._internal);
}

static void get_bag_pockets(
    pkmn_item_bag_t* item_bag_ptr,
    pkmn_item_list_t** item_pocket_ptr_out,
    pkmn_item_list_t** key_item_pocket_ptr_out,
    pkmn_item_list_t** ball_pocket_ptr_out,
    pkmn_item_list_t** tmhm_pocket_ptr_out,
    const char* tmhm_pocket_name,
    pkmn_item_list_t** berry_pocket_ptr_out,
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

    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                "Items",
                item_pocket_ptr_out
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                "Key Items",
                key_item_pocket_ptr_out
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                "Poké Balls",
                ball_pocket_ptr_out
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                tmhm_pocket_name,
                tmhm_pocket_ptr_out
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                berry_pocket_name,
                berry_pocket_ptr_out
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void gba_item_bag_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_item_bag_t item_bag =
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

    error = pkmn_item_bag_init(
                game,
                &item_bag
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_bag._internal);

    TEST_ASSERT_EQUAL(5, item_bag.pockets.num_pockets);
    TEST_ASSERT_EQUAL(5, item_bag.pockets.pocket_names.length);

    // Make sure pockets are stored and accessible as expected.
    for(size_t i = 0; i < 5; ++i)
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

    const char* COMMON_POCKET_NAMES[] = {"Items", "Key Items", "Poké Balls"};
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

    for(size_t i = 0; i < 3; ++i)
    {
        pkmn_item_list_t* item_list_ptr = NULL;
        error = pkmn_item_bag_get_pocket(
                    &item_bag,
                    COMMON_POCKET_NAMES[i],
                    &item_list_ptr
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(item_list_ptr);
        TEST_ASSERT_EQUAL_STRING(
            item_list_ptr->name,
            COMMON_POCKET_NAMES[i]
        );
    }

    pkmn_item_list_t* item_pocket_ptr = NULL;
    pkmn_item_list_t* key_item_pocket_ptr = NULL;
    pkmn_item_list_t* ball_pocket_ptr = NULL;
    pkmn_item_list_t* tmhm_pocket_ptr = NULL;
    pkmn_item_list_t* berry_pocket_ptr = NULL;

    error = pkmn_item_bag_get_pocket(
                &item_bag,
                tmhm_pocket_name,
                &tmhm_pocket_ptr
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(tmhm_pocket_ptr);
    TEST_ASSERT_EQUAL_STRING(
        tmhm_pocket_ptr->name,
        tmhm_pocket_name
    );

    error = pkmn_item_bag_get_pocket(
                &item_bag,
                berry_pocket_name,
                &berry_pocket_ptr
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(berry_pocket_ptr);
    TEST_ASSERT_EQUAL_STRING(
        berry_pocket_ptr->name,
        berry_pocket_name
    );

    get_bag_pockets(
        &item_bag,
        &item_pocket_ptr,
        &key_item_pocket_ptr,
        &ball_pocket_ptr,
        &tmhm_pocket_ptr,
        tmhm_pocket_name,
        &berry_pocket_ptr,
        berry_pocket_name
    );
    TEST_ASSERT_NOT_NULL(item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr);
    TEST_ASSERT_NOT_NULL(tmhm_pocket_ptr);
    TEST_ASSERT_NOT_NULL(berry_pocket_ptr);

    gba_item_pocket_test(item_pocket_ptr, game);
    gba_key_item_pocket_test(key_item_pocket_ptr, game);
    gba_ball_pocket_test(ball_pocket_ptr, game);
    gba_tmhm_pocket_test(tmhm_pocket_ptr, game);
    gba_berry_pocket_test(berry_pocket_ptr, game);

    error = pkmn_item_bag_free(&item_bag);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(item_bag._internal);

    error = pkmn_item_bag_init(
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
        &tmhm_pocket_ptr,
        tmhm_pocket_name,
        &berry_pocket_ptr,
        berry_pocket_name
    );
    TEST_ASSERT_NOT_NULL(item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr);
    TEST_ASSERT_NOT_NULL(tmhm_pocket_ptr);
    TEST_ASSERT_NOT_NULL(berry_pocket_ptr);

    // Make sure adding items through the bag adds to the proper pockets.
    TEST_ASSERT_EQUAL(0, item_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, key_item_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, ball_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, tmhm_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, berry_pocket_ptr->num_items);
    for(size_t i = 0; i < 8; ++i)
    {
        pkmn_item_bag_add(
            &item_bag,
            ALL_POCKET_ITEM_NAMES[i],
            5
        );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    TEST_ASSERT_EQUAL_STRING("Potion", item_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, item_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", item_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, item_pocket_ptr->item_slots.item_slots[1].amount);

    TEST_ASSERT_EQUAL_STRING("Mach Bike", key_item_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, key_item_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("Wailmer Pail", key_item_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(5, key_item_pocket_ptr->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", key_item_pocket_ptr->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, key_item_pocket_ptr->item_slots.item_slots[2].amount);

    TEST_ASSERT_EQUAL_STRING("Great Ball", ball_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, ball_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("Master Ball", ball_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(5, ball_pocket_ptr->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", ball_pocket_ptr->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, ball_pocket_ptr->item_slots.item_slots[2].amount);

    TEST_ASSERT_EQUAL_STRING("TM01", tmhm_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, tmhm_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("HM04", tmhm_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(5, tmhm_pocket_ptr->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", tmhm_pocket_ptr->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, tmhm_pocket_ptr->item_slots.item_slots[2].amount);

    TEST_ASSERT_EQUAL_STRING("Aspear Berry", berry_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, berry_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", berry_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, berry_pocket_ptr->item_slots.item_slots[1].amount);

    // Make sure removing items through the bag removes from the proper pockets.
    for(size_t i = 0; i < 8; ++i)
    {
        pkmn_item_bag_remove(
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
    TEST_ASSERT_EQUAL(0, berry_pocket_ptr->num_items);

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

    TEST_ASSERT_EQUAL_STRING("None", tmhm_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(0, tmhm_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", tmhm_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, tmhm_pocket_ptr->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", tmhm_pocket_ptr->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, tmhm_pocket_ptr->item_slots.item_slots[2].amount);

    TEST_ASSERT_EQUAL_STRING("None", berry_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(0, berry_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", berry_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, berry_pocket_ptr->item_slots.item_slots[1].amount);

    // Make sure we can't add items from later generations.
    test_item_bag_invalid_items(
        &item_bag,
        WRONG_GAME_ALL_POCKET_ITEM_NAMES,
        9
    );
}

#define GBA_ITEM_TESTS(test_game) \
void test_gba_item_pocket_ ## test_game () \
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
    gba_item_pocket_test( \
        &item_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&item_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NULL(item_pocket._internal); \
} \
void test_gba_key_item_pocket_ ## test_game () \
{ \
    pkmn_error_t error = PKMN_ERROR_NONE; \
 \
    pkmn_item_list_t key_item_pocket = \
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
                "Key Items", \
                #test_game, \
                &key_item_pocket \
            ); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NOT_NULL(key_item_pocket._internal); \
 \
    gba_key_item_pocket_test( \
        &key_item_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&key_item_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NULL(key_item_pocket._internal); \
} \
void test_gba_ball_pocket_ ## test_game () \
{ \
    pkmn_error_t error = PKMN_ERROR_NONE; \
 \
    pkmn_item_list_t ball_pocket = \
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
                "Poké Balls", \
                #test_game, \
                &ball_pocket \
            ); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NOT_NULL(ball_pocket._internal); \
 \
    gba_ball_pocket_test( \
        &ball_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&ball_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NULL(ball_pocket._internal); \
} \
void test_gba_tmhm_pocket_ ## test_game () \
{ \
    pkmn_error_t error = PKMN_ERROR_NONE; \
 \
    pkmn_item_list_t tmhm_pocket = \
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
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NOT_NULL(tmhm_pocket._internal); \
 \
    gba_tmhm_pocket_test( \
        &tmhm_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&tmhm_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NULL(tmhm_pocket._internal); \
} \
void test_gba_berry_pocket_ ## test_game () \
{ \
    pkmn_error_t error = PKMN_ERROR_NONE; \
 \
    pkmn_item_list_t berry_pocket = \
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
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NOT_NULL(berry_pocket._internal); \
 \
    gba_berry_pocket_test( \
        &berry_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&berry_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NULL(berry_pocket._internal); \
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
