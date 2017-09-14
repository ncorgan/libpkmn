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

static const char* COLOSSEUM_ALL_POCKET_ITEM_NAMES[] =
{
    "Potion", "Ein File S", "Great Ball", "TM01",
    "TM02", "Aspear Berry", "Joy Scent", "Excite Scent"
};

static const char* XD_ALL_POCKET_ITEM_NAMES[] =
{
    "Potion", "Gonzap's Key", "Great Ball", "TM01",
    "TM02", "Aspear Berry", "Joy Scent", "Battle CD 01"
};

static const char* COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEM_NAMES[] =
{
    "Pink Bow", "Black Sludge",
    "GS Ball", "Gonzap's Key", "Poffin Items",
    "TM51", "HM01",
    "Berry", "Occa Berry"
};

static const char* XD_WRONG_GAME_ALL_POCKET_ITEM_NAMES[] =
{
    "Pink Bow", "Black Sludge",
    "GS Ball", "Ein File S", "Poffin Items",
    "TM51", "HM01",
    "Berry", "Occa Berry"
};

static void gcn_item_pocket_test(
    pkmn_item_list_t* item_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(item_pocket);
    TEST_ASSERT_NOT_NULL(item_pocket->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(game);

    bool is_colosseum = !strcmp(game, "Colosseum");

    size_t capacity = is_colosseum ? 20 : 30;

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
    static const char* colosseum_wrong_pocket_items[] =
    {
        "Ein File S", "Great Ball", "TM01", "Oran Berry", "Joy Scent"
    };
    static const char* xd_wrong_pocket_items[] =
    {
        "Miror Radar", "Great Ball", "TM01", "Oran Berry", "Joy Scent", "Battle CD 01"
    };
    test_item_list_invalid_items(
        item_pocket,
        is_colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_colosseum ? 5 : 6
    );

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"};
    test_item_list_invalid_items(item_pocket, wrong_generation_item_names, 4);

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

static void gcn_key_item_pocket_test(
    pkmn_item_list_t* key_item_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(key_item_pocket);
    TEST_ASSERT_NOT_NULL(key_item_pocket->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(game);

    bool is_colosseum = !strcmp(game, "Colosseum");
    const char* gcn_item = is_colosseum ? "Ein File S" : "Miror Radar";

    TEST_ASSERT_EQUAL_STRING("Key Items", key_item_pocket->name);
    TEST_ASSERT_EQUAL_STRING(game, key_item_pocket->game);
    TEST_ASSERT_EQUAL(0, key_item_pocket->num_items);
    TEST_ASSERT_EQUAL(43, key_item_pocket->capacity);
    TEST_ASSERT_EQUAL(43, key_item_pocket->item_slots.length);

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(key_item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(key_item_pocket, gcn_item);

    // Make sure we can't add items from other pockets.
    static const char* colosseum_wrong_pocket_items[] =
    {
        "Potion", "Great Ball", "TM01", "Oran Berry", "Joy Scent"
    };
    static const char* xd_wrong_pocket_items[] =
    {
        "Potion", "Great Ball", "TM01", "Oran Berry", "Joy Scent", "Battle CD 01"
    };
    test_item_list_invalid_items(
        key_item_pocket,
        is_colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_colosseum ? 5 : 6
    );

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"};
    test_item_list_invalid_items(key_item_pocket, wrong_generation_item_names, 4);

    // Make sure we can't add items from incompatible Generation III games.
    const char* frlg_items[] = {"Helix Fossil", "Tea", "Ruby"};;
    const char* emerald_items[] = {"Magma Emblem", "Old Sea Map"};
    
    test_item_list_invalid_items(key_item_pocket, frlg_items, 3);
    test_item_list_invalid_items(key_item_pocket, emerald_items, 2);

    static const char* colosseum_item_names[] =
    {
        "Jail Key", "Elevator Key", "Small Tablet", "F-Disk",
        "R-Disk", "L-Disk", "D-Disk", "U-Disk"
    };
    static const char* xd_item_names[] =
    {
        "Krane Memo 1", "Krane Memo 2", "Krane Memo 3", "Krane Memo 4",
        "Krane Memo 5", "Voice Case 1", "Voice Case 2", "Voice Case 3"
    };

    // Test setting items by index.
    test_item_list_set_item(
        key_item_pocket,
        is_colosseum ? colosseum_item_names : xd_item_names,
        3
    );
    TEST_ASSERT_EQUAL(0, key_item_pocket->num_items);

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        key_item_pocket,
        is_colosseum ? colosseum_item_names : xd_item_names,
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

static void gcn_ball_pocket_test(
    pkmn_item_list_t* ball_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(ball_pocket);
    TEST_ASSERT_NOT_NULL(ball_pocket->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(game);

    bool is_colosseum = !strcmp(game, "Colosseum");

    TEST_ASSERT_EQUAL_STRING("Poké Balls", ball_pocket->name);
    TEST_ASSERT_EQUAL_STRING(game, ball_pocket->game);
    TEST_ASSERT_EQUAL(0, ball_pocket->num_items);
    TEST_ASSERT_EQUAL(16, ball_pocket->capacity);
    TEST_ASSERT_EQUAL(16, ball_pocket->item_slots.length);

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(ball_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        ball_pocket,
        "Master Ball"
    );

    // Make sure we can't add items from other pockets.
    static const char* colosseum_wrong_pocket_items[] =
    {
        "Ein File S", "Potion", "TM01", "Oran Berry", "Joy Scent"
    };
    static const char* xd_wrong_pocket_items[] =
    {
        "Miror Radar", "Potion", "TM01", "Oran Berry", "Joy Scent", "Battle CD 01"
    };
    test_item_list_invalid_items(
        ball_pocket,
        is_colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_colosseum ? 5 : 6
    );

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

static void gcn_tm_pocket_test(
    pkmn_item_list_t* tm_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(tm_pocket);
    TEST_ASSERT_NOT_NULL(tm_pocket->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(game);

    bool is_colosseum = !strcmp(game, "Colosseum");

    TEST_ASSERT_EQUAL_STRING("TMs", tm_pocket->name);
    TEST_ASSERT_EQUAL_STRING(game, tm_pocket->game);
    TEST_ASSERT_EQUAL(0, tm_pocket->num_items);
    TEST_ASSERT_EQUAL(64, tm_pocket->capacity);
    TEST_ASSERT_EQUAL(64, tm_pocket->item_slots.length);

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(tm_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        tm_pocket,
        "TM01"
    );

    // Make sure we can't add items from other pockets.
    static const char* colosseum_wrong_pocket_items[] =
    {
        "Ein File S", "Potion", "Great Ball", "Oran Berry", "Joy Scent"
    };
    static const char* xd_wrong_pocket_items[] =
    {
        "Miror Radar", "Potion", "Great Ball", "Oran Berry", "Joy Scent", "Battle CD 01"
    };
    test_item_list_invalid_items(
        tm_pocket,
        is_colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_colosseum ? 5 : 6
    );

    // Make sure we can't add items from later games.
    const char* wrong_generation_item_names[] = {"TM51", "HM01"};
    test_item_list_invalid_items(tm_pocket, wrong_generation_item_names, 2);

    const char* item_names[] =
    {
        "TM01", "TM02", "TM03", "TM04",
        "TM05", "TM06", "TM07", "TM08"
    };

    // Test setting items by index.
    test_item_list_set_item(
        tm_pocket,
        item_names,
        3
    );
    TEST_ASSERT_EQUAL(0, tm_pocket->num_items);

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        tm_pocket,
        item_names,
        8
    );

    pkmn_string_list_t valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = pkmn_item_list_get_valid_items(
                             tm_pocket,
                             &valid_items
                         );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gcn_berry_pocket_test(
    pkmn_item_list_t* berry_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(berry_pocket);
    TEST_ASSERT_NOT_NULL(berry_pocket->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(game);

    bool is_colosseum = !strcmp(game, "Colosseum");

    TEST_ASSERT_EQUAL_STRING("Berries", berry_pocket->name);
    TEST_ASSERT_EQUAL_STRING(game, berry_pocket->game);
    TEST_ASSERT_EQUAL(0, berry_pocket->num_items);
    TEST_ASSERT_EQUAL(46, berry_pocket->capacity);
    TEST_ASSERT_EQUAL(46, berry_pocket->item_slots.length);

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(berry_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        berry_pocket,
        "Oran Berry"
    );

    // Make sure we can't add items from other pockets.
    static const char* colosseum_wrong_pocket_items[] =
    {
        "Ein File S", "Potion", "Great Ball", "TM01", "Joy Scent"
    };
    static const char* xd_wrong_pocket_items[] =
    {
        "Miror Radar", "Potion", "Great Ball", "TM01", "Joy Scent", "Battle CD 01"
    };
    test_item_list_invalid_items(
        berry_pocket,
        is_colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_colosseum ? 5 : 6
    );

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

static void gcn_cologne_pocket_test(
    pkmn_item_list_t* cologne_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(cologne_pocket);
    TEST_ASSERT_NOT_NULL(cologne_pocket->item_slots.item_slots);
    TEST_ASSERT_NOT_NULL(game);

    bool is_colosseum = !strcmp(game, "Colosseum");

    TEST_ASSERT_EQUAL_STRING("Colognes", cologne_pocket->name);
    TEST_ASSERT_EQUAL_STRING(game, cologne_pocket->game);
    TEST_ASSERT_EQUAL(0, cologne_pocket->num_items);
    TEST_ASSERT_EQUAL(3, cologne_pocket->capacity);
    TEST_ASSERT_EQUAL(3, cologne_pocket->item_slots.length);

    pkmn_error_t error = PKMN_ERROR_NONE;

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(cologne_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        cologne_pocket,
        "Joy Scent"
    );

    // Make sure we can't add items from other pockets.
    static const char* colosseum_wrong_pocket_items[] =
    {
        "Ein File S", "Potion", "Great Ball", "TM01", "Oran Berry"
    };
    static const char* xd_wrong_pocket_items[] =
    {
        "Miror Radar", "Potion", "Great Ball", "TM01", "Oran Berry", "Battle CD 01"
    };
    test_item_list_invalid_items(
        cologne_pocket,
        is_colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_colosseum ? 5 : 6
    );

    // Since this pocket can only have 3 items, we can't use our typical function, which requires 8.

    error = pkmn_item_list_add(
                cologne_pocket,
                "Joy Scent",
                3
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_add(
                cologne_pocket,
                "Excite Scent",
                3
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_add(
                cologne_pocket,
                "Vivid Scent",
                3
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_item_list_remove(
                cologne_pocket,
                "Excite Scent",
                3
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_remove(
                cologne_pocket,
                "Vivid Scent",
                1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_EQUAL_STRING("Joy Scent", cologne_pocket->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(3, cologne_pocket->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("Vivid Scent", cologne_pocket->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(2, cologne_pocket->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", cologne_pocket->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, cologne_pocket->item_slots.item_slots[2].amount);

    pkmn_string_list_t valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                cologne_pocket,
                &valid_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gcn_battle_cd_pocket_test(
    pkmn_item_list_t* battle_cd_pocket
)
{
    TEST_ASSERT_NOT_NULL(battle_cd_pocket);
    TEST_ASSERT_NOT_NULL(battle_cd_pocket->item_slots.item_slots);

    TEST_ASSERT_EQUAL_STRING("Battle CDs", battle_cd_pocket->name);
    TEST_ASSERT_EQUAL_STRING("XD", battle_cd_pocket->game);
    TEST_ASSERT_EQUAL(0, battle_cd_pocket->num_items);
    TEST_ASSERT_EQUAL(60, battle_cd_pocket->capacity);
    TEST_ASSERT_EQUAL(60, battle_cd_pocket->item_slots.length);

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(battle_cd_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        battle_cd_pocket,
        "Battle CD 01"
    );

    // Make sure we can't add items from other pockets.
    static const char* wrong_pocket_items[] =
    {
        "Miror Radar", "Potion", "Great Ball", "TM01", "Oran Berry", "Joy Scent"
    };
    test_item_list_invalid_items(
        battle_cd_pocket,
        wrong_pocket_items,
        6
    );

    const char* item_names[] =
    {
        "Battle CD 01", "Battle CD 02", "Battle CD 03", "Battle CD 04",
        "Battle CD 05", "Battle CD 06", "Battle CD 07", "Battle CD 08"
    };

    // Test setting items by index.
    test_item_list_set_item(
        battle_cd_pocket,
        item_names,
        3
    );
    TEST_ASSERT_EQUAL(0, battle_cd_pocket->num_items);

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        battle_cd_pocket,
        item_names,
        8
    );

    pkmn_string_list_t valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = pkmn_item_list_get_valid_items(
                             battle_cd_pocket,
                             &valid_items
                         );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gcn_item_pc_test(
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

    bool is_colosseum = !strcmp(game, "Colosseum");

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
    TEST_ASSERT_EQUAL(235, item_pc.capacity);
    TEST_ASSERT_EQUAL(235, item_pc.item_slots.length);

    // Make sure item slots start as completely empty.
    test_item_list_empty_slots(&item_pc);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        &item_pc,
        "Potion"
    );

    // Make sure we can't add items from incompatible games.
    test_item_list_invalid_items(
        &item_pc,
        is_colosseum ? COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEM_NAMES
                     : XD_WRONG_GAME_ALL_POCKET_ITEM_NAMES,
        9
    );

    // Test setting items by index.
    test_item_list_set_item(
        &item_pc,
        is_colosseum ? COLOSSEUM_ALL_POCKET_ITEM_NAMES
                     : XD_ALL_POCKET_ITEM_NAMES,
        3
    );
    TEST_ASSERT_EQUAL(0, item_pc.num_items);

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        &item_pc,
        is_colosseum ? COLOSSEUM_ALL_POCKET_ITEM_NAMES
                     : XD_ALL_POCKET_ITEM_NAMES,
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

static void get_common_bag_pockets(
    pkmn_item_bag_t* item_bag_ptr,
    pkmn_item_list_t** item_pocket_ptr_out,
    pkmn_item_list_t** key_item_pocket_ptr_out,
    pkmn_item_list_t** ball_pocket_ptr_out,
    pkmn_item_list_t** tm_pocket_ptr_out,
    pkmn_item_list_t** berry_pocket_ptr_out,
    pkmn_item_list_t** cologne_pocket_ptr_out
)
{
    TEST_ASSERT_NOT_NULL(item_bag_ptr);
    TEST_ASSERT_NOT_NULL(item_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(tm_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(berry_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(cologne_pocket_ptr_out);

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
                "TMs",
                tm_pocket_ptr_out
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                "Berries",
                berry_pocket_ptr_out
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                "Colognes",
                cologne_pocket_ptr_out
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void gcn_item_bag_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;

    bool is_colosseum = !strcmp(game, "Colosseum");
    size_t num_pockets = is_colosseum ? 6 : 7;

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

    TEST_ASSERT_EQUAL(num_pockets, item_bag.pockets.num_pockets);
    TEST_ASSERT_EQUAL(num_pockets, item_bag.pockets.pocket_names.length);

    // Make sure pockets are stored and accessible as expected.
    for(size_t i = 0; i < num_pockets; ++i)
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

    const char* COMMON_POCKET_NAMES[] =
    {
        "Items", "Key Items", "Poké Balls", "TMs", "Berries", "Colognes"
    };

    for(size_t i = 0; i < 6; ++i)
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
    if(!is_colosseum)
    {
        pkmn_item_list_t* item_list_ptr = NULL;
        error = pkmn_item_bag_get_pocket(
                    &item_bag,
                    "Battle CDs",
                    &item_list_ptr
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(item_list_ptr);
        TEST_ASSERT_EQUAL_STRING(
            item_list_ptr->name,
            "Battle CDs"
        );
    }

    pkmn_item_list_t* item_pocket_ptr = NULL;
    pkmn_item_list_t* key_item_pocket_ptr = NULL;
    pkmn_item_list_t* ball_pocket_ptr = NULL;
    pkmn_item_list_t* tm_pocket_ptr = NULL;
    pkmn_item_list_t* berry_pocket_ptr = NULL;
    pkmn_item_list_t* cologne_pocket_ptr = NULL;
    pkmn_item_list_t* battle_cd_pocket_ptr = NULL;

    get_common_bag_pockets(
        &item_bag,
        &item_pocket_ptr,
        &key_item_pocket_ptr,
        &ball_pocket_ptr,
        &tm_pocket_ptr,
        &berry_pocket_ptr,
        &cologne_pocket_ptr
    );
    TEST_ASSERT_NOT_NULL(item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr);
    TEST_ASSERT_NOT_NULL(tm_pocket_ptr);
    TEST_ASSERT_NOT_NULL(berry_pocket_ptr);
    TEST_ASSERT_NOT_NULL(cologne_pocket_ptr);

    gcn_item_pocket_test(item_pocket_ptr, game);
    gcn_key_item_pocket_test(key_item_pocket_ptr, game);
    gcn_ball_pocket_test(ball_pocket_ptr, game);
    gcn_tm_pocket_test(tm_pocket_ptr, game);
    gcn_berry_pocket_test(berry_pocket_ptr, game);
    gcn_cologne_pocket_test(cologne_pocket_ptr, game);

    if(!is_colosseum)
    {
        error = pkmn_item_bag_get_pocket(
                    &item_bag,
                    "Battle CDs",
                    &battle_cd_pocket_ptr
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(battle_cd_pocket_ptr);

        gcn_battle_cd_pocket_test(battle_cd_pocket_ptr);
    }

    error = pkmn_item_bag_free(&item_bag);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(item_bag._internal);

    error = pkmn_item_bag_init(
                game,
                &item_bag
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_bag._internal);

    get_common_bag_pockets(
        &item_bag,
        &item_pocket_ptr,
        &key_item_pocket_ptr,
        &ball_pocket_ptr,
        &tm_pocket_ptr,
        &berry_pocket_ptr,
        &cologne_pocket_ptr
    );
    TEST_ASSERT_NOT_NULL(item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr);
    TEST_ASSERT_NOT_NULL(tm_pocket_ptr);
    TEST_ASSERT_NOT_NULL(berry_pocket_ptr);
    TEST_ASSERT_NOT_NULL(cologne_pocket_ptr);

    // Make sure adding items through the bag adds to the proper pockets.
    TEST_ASSERT_EQUAL(0, item_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, key_item_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, ball_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, tm_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, berry_pocket_ptr->num_items);

    const char* gcn_key_item = is_colosseum ? "Ein File S" : "Gonzap's Key";
    const char** all_pocket_item_names = is_colosseum ? COLOSSEUM_ALL_POCKET_ITEM_NAMES
                                       : XD_ALL_POCKET_ITEM_NAMES;
    const char** wrong_game_all_pocket_item_names = is_colosseum ? COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEM_NAMES
                                                  : XD_WRONG_GAME_ALL_POCKET_ITEM_NAMES;

    for(size_t i = 0; i < 8; ++i)
    {
        pkmn_item_bag_add(
            &item_bag,
            all_pocket_item_names[i],
            5
        );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    TEST_ASSERT_EQUAL_STRING("Potion", item_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, item_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", item_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, item_pocket_ptr->item_slots.item_slots[1].amount);

    TEST_ASSERT_EQUAL_STRING(gcn_key_item, key_item_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, key_item_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", key_item_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, key_item_pocket_ptr->item_slots.item_slots[1].amount);

    TEST_ASSERT_EQUAL_STRING("Great Ball", ball_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, ball_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", ball_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, ball_pocket_ptr->item_slots.item_slots[1].amount);

    TEST_ASSERT_EQUAL_STRING("TM01", tm_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, tm_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("TM02", tm_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(5, tm_pocket_ptr->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", tm_pocket_ptr->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, tm_pocket_ptr->item_slots.item_slots[2].amount);

    TEST_ASSERT_EQUAL_STRING("Aspear Berry", berry_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, berry_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", berry_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, berry_pocket_ptr->item_slots.item_slots[1].amount);

    TEST_ASSERT_EQUAL_STRING("Joy Scent", cologne_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, cologne_pocket_ptr->item_slots.item_slots[0].amount);

    if(is_colosseum)
    {
        TEST_ASSERT_EQUAL_STRING("Excite Scent", cologne_pocket_ptr->item_slots.item_slots[1].item);
        TEST_ASSERT_EQUAL(5, cologne_pocket_ptr->item_slots.item_slots[1].amount);
    }
    else
    {
        TEST_ASSERT_EQUAL_STRING("None", cologne_pocket_ptr->item_slots.item_slots[1].item);
        TEST_ASSERT_EQUAL(0, cologne_pocket_ptr->item_slots.item_slots[1].amount);

        error = pkmn_item_bag_get_pocket(
                    &item_bag,
                    "Battle CDs",
                    &battle_cd_pocket_ptr
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(battle_cd_pocket_ptr);

        TEST_ASSERT_EQUAL_STRING("Battle CD 01", battle_cd_pocket_ptr->item_slots.item_slots[0].item);
        TEST_ASSERT_EQUAL(5, battle_cd_pocket_ptr->item_slots.item_slots[0].amount);
        TEST_ASSERT_EQUAL_STRING("None", battle_cd_pocket_ptr->item_slots.item_slots[1].item);
        TEST_ASSERT_EQUAL(0, battle_cd_pocket_ptr->item_slots.item_slots[1].amount);
    }

    // Make sure removing items through the bag removes from the proper pockets.
    for(size_t i = 0; i < 8; ++i)
    {
        pkmn_item_bag_remove(
            &item_bag,
            all_pocket_item_names[i],
            5
        );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }
    TEST_ASSERT_EQUAL(0, item_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, key_item_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, ball_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, tm_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, berry_pocket_ptr->num_items);
    TEST_ASSERT_EQUAL(0, cologne_pocket_ptr->num_items);
    if(!is_colosseum)
    {
        TEST_ASSERT_EQUAL(0, battle_cd_pocket_ptr->num_items);
    }

    TEST_ASSERT_EQUAL_STRING("None", item_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(0, item_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", item_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, item_pocket_ptr->item_slots.item_slots[1].amount);

    TEST_ASSERT_EQUAL_STRING("None", key_item_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(0, key_item_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", key_item_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, key_item_pocket_ptr->item_slots.item_slots[1].amount);

    TEST_ASSERT_EQUAL_STRING("None", ball_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(0, ball_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", ball_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, ball_pocket_ptr->item_slots.item_slots[1].amount);

    TEST_ASSERT_EQUAL_STRING("None", tm_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(0, tm_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", tm_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, tm_pocket_ptr->item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", tm_pocket_ptr->item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, tm_pocket_ptr->item_slots.item_slots[2].amount);

    TEST_ASSERT_EQUAL_STRING("None", berry_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(0, berry_pocket_ptr->item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", berry_pocket_ptr->item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, berry_pocket_ptr->item_slots.item_slots[1].amount);

    TEST_ASSERT_EQUAL_STRING("None", cologne_pocket_ptr->item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(0, cologne_pocket_ptr->item_slots.item_slots[0].amount);

    if(is_colosseum)
    {
        TEST_ASSERT_EQUAL_STRING("None", cologne_pocket_ptr->item_slots.item_slots[1].item);
        TEST_ASSERT_EQUAL(0, cologne_pocket_ptr->item_slots.item_slots[1].amount);
    }
    else
    {
        TEST_ASSERT_EQUAL_STRING("None", cologne_pocket_ptr->item_slots.item_slots[1].item);
        TEST_ASSERT_EQUAL(0, cologne_pocket_ptr->item_slots.item_slots[1].amount);

        TEST_ASSERT_EQUAL_STRING("None", battle_cd_pocket_ptr->item_slots.item_slots[0].item);
        TEST_ASSERT_EQUAL(0, battle_cd_pocket_ptr->item_slots.item_slots[0].amount);
        TEST_ASSERT_EQUAL_STRING("None", battle_cd_pocket_ptr->item_slots.item_slots[1].item);
        TEST_ASSERT_EQUAL(0, battle_cd_pocket_ptr->item_slots.item_slots[1].amount);
    }

    // Make sure we can't add items from later generations.
    test_item_bag_invalid_items(
        &item_bag,
        wrong_game_all_pocket_item_names,
        9
    );
}

#define GCN_ITEM_TESTS(test_game) \
void test_gcn_item_pocket_ ## test_game () \
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
    gcn_item_pocket_test( \
        &item_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&item_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NULL(item_pocket._internal); \
} \
void test_gcn_key_item_pocket_ ## test_game () \
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
    gcn_key_item_pocket_test( \
        &key_item_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&key_item_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NULL(key_item_pocket._internal); \
} \
void test_gcn_ball_pocket_ ## test_game () \
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
    gcn_ball_pocket_test( \
        &ball_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&ball_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NULL(ball_pocket._internal); \
} \
void test_gcn_tm_pocket_ ## test_game () \
{ \
    pkmn_error_t error = PKMN_ERROR_NONE; \
 \
    pkmn_item_list_t tm_pocket = \
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
                "TMs", \
                #test_game, \
                &tm_pocket \
            ); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NOT_NULL(tm_pocket._internal); \
 \
    gcn_tm_pocket_test( \
        &tm_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&tm_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NULL(tm_pocket._internal); \
} \
void test_gcn_berry_pocket_ ## test_game () \
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
    error = pkmn_item_list_init( \
                "Berries", \
                #test_game, \
                &berry_pocket \
            ); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NOT_NULL(berry_pocket._internal); \
 \
    gcn_berry_pocket_test( \
        &berry_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&berry_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NULL(berry_pocket._internal); \
} \
void test_gcn_cologne_pocket_ ## test_game () \
{ \
    pkmn_error_t error = PKMN_ERROR_NONE; \
 \
    pkmn_item_list_t cologne_pocket = \
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
                "Colognes", \
                #test_game, \
                &cologne_pocket \
            ); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NOT_NULL(cologne_pocket._internal); \
 \
    gcn_cologne_pocket_test( \
        &cologne_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&cologne_pocket); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error); \
    TEST_ASSERT_NULL(cologne_pocket._internal); \
} \
void test_gcn_item_pc_ ## test_game () \
{ \
    gcn_item_pc_test(#test_game); \
} \
void test_gcn_item_bag_ ## test_game () \
{ \
    gcn_item_bag_test(#test_game); \
}

GCN_ITEM_TESTS(Colosseum)
GCN_ITEM_TESTS(XD)

void test_gcn_battle_cd_pocket_XD()
{
    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_item_list_t battle_cd_pocket =
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
                "Battle CDs",
                "XD",
                &battle_cd_pocket
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(battle_cd_pocket._internal);

    gcn_battle_cd_pocket_test(
        &battle_cd_pocket
    );

    error = pkmn_item_list_free(&battle_cd_pocket);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(battle_cd_pocket._internal);
}
