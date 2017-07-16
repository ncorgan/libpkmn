/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_items_test.h"

#include <pkmn-c/database/lists.h>

#include <unity.h>

#include <string.h>

#define STRBUFFER_LEN 1024

static const char* colosseum_all_pocket_items[] =
{
    "Potion", "Ein File S", "Great Ball", "TM01",
    "TM02", "Aspear Berry", "Joy Scent", "Excite Scent"
};

static const char* xd_all_pocket_items[] =
{
    "Potion", "Gonzap's Key", "Great Ball", "TM01",
    "TM02", "Aspear Berry", "Joy Scent", "Battle CD 01"
};

static const char* colosseum_wrong_game_all_pocket_items[] =
{
    "Pink Bow", "Black Sludge",
    "GS Ball", "Gonzap's Key", "Poffin Items",
    "TM51", "HM01",
    "Berry", "Occa Berry"
};

static const char* xd_wrong_game_all_pocket_items[] =
{
    "Pink Bow", "Black Sludge",
    "GS Ball", "Ein File S", "Poffin Items",
    "TM51", "HM01",
    "Berry", "Occa Berry"
};

void gcn_item_pocket_test(
    pkmn_item_list_handle_t item_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(item_pocket);
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    bool colosseum = !strcmp(game, "Colosseum");

    error = pkmn_item_list_get_name(
                item_pocket,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Items", strbuffer);

    error = pkmn_item_list_get_game(
                item_pocket,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    int capacity = 0;
    error = pkmn_item_list_get_capacity(
                item_pocket,
                &capacity
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL((colosseum ? 20 : 30), capacity);

    int num_items = 0;
    error = pkmn_item_list_get_num_items(
                item_pocket,
                &num_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_items);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        item_pocket,
        "Potion"
    );

    // Make sure we can't add items from other pockets.
    static const char* colosseum_wrong_pocket_items[] = {
        "Ein File S", "Great Ball", "TM01", "Oran Berry", "Joy Scent"
    };
    static const char* xd_wrong_pocket_items[] = {
        "Miror Radar", "Great Ball", "TM01", "Oran Berry", "Joy Scent", "Battle CD 01"
    };
    test_item_list_invalid_items(
        item_pocket,
        (colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items),
        (colosseum ? 5 : 6)
    );

    // Make sure we can't add items from other generations.
    static const char* wrong_generation_items[] = {
        "Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"
    };
    test_item_list_invalid_items(
        item_pocket,
        wrong_generation_items,
        4
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const char* item_names[] = {
        "Potion", "Orange Mail", "Lava Cookie", "Stardust",
        "Shadow Mail", "Pink Scarf", "Antidote", "Green Shard"
    };
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
    error = pkmn_item_list_get_valid_items(
                item_pocket,
                &valid_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

void gcn_key_item_pocket_test(
    pkmn_item_list_handle_t key_item_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(key_item_pocket);
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    bool colosseum = !strcmp(game, "Colosseum");
    const char* gcn_item = colosseum ? "Ein File S" : "Miror Radar";

    error = pkmn_item_list_get_name(
                key_item_pocket,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Key Items", strbuffer);

    error = pkmn_item_list_get_game(
                key_item_pocket,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    int capacity = 0;
    error = pkmn_item_list_get_capacity(
                key_item_pocket,
                &capacity
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(43, capacity);

    int num_items = 0;
    error = pkmn_item_list_get_num_items(
                key_item_pocket,
                &num_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_items);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(key_item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        key_item_pocket,
        gcn_item
    );

    // Make sure we can't add items from other pockets.
    static const char* colosseum_wrong_pocket_items[] = {
        "Potion", "Great Ball", "TM01", "Oran Berry", "Joy Scent"
    };
    static const char* xd_wrong_pocket_items[] = {
        "Potion", "Great Ball", "TM01", "Oran Berry", "Joy Scent", "Battle CD 01"
    };
    test_item_list_invalid_items(
        key_item_pocket,
        (colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items),
        (colosseum ? 5 : 6)
    );

    // Make sure we can't add items from other generations.
    static const char* wrong_generation_items[] = {
        "GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"
    };
    test_item_list_invalid_items(
        key_item_pocket,
        wrong_generation_items,
        4
    );

    // Make sure we can't add items from incompatible Generation III games.
    static const char* frlg_items[] = {
        "Helix Fossil", "Tea", "Ruby"
    };
    static const char* emerald_items[] = {
        "Magma Emblem", "Old Sea Map"
    };
    test_item_list_invalid_items(
        key_item_pocket,
        frlg_items,
        3
    );
    test_item_list_invalid_items(
        key_item_pocket,
        emerald_items,
        2
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const char* colosseum_item_names[] = {
        "Jail Key", "Elevator Key", "Small Tablet", "F-Disk",
        "R-Disk", "L-Disk", "D-Disk", "U-Disk"
    };
    static const char* xd_item_names[] = {
        "Krane Memo 1", "Krane Memo 2", "Krane Memo 3", "Krane Memo 4",
        "Krane Memo 5", "Voice Case 1", "Voice Case 2", "Voice Case 3"
    };
    test_item_list_add_remove(
        key_item_pocket,
        (colosseum ? colosseum_item_names : xd_item_names),
        8
    );

    pkmn_string_list_t valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                key_item_pocket,
                &valid_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

void gcn_ball_pocket_test(
    pkmn_item_list_handle_t ball_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(ball_pocket);
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    bool colosseum = !strcmp(game, "Colosseum");

    error = pkmn_item_list_get_name(
                ball_pocket,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Poké Balls", strbuffer);

    error = pkmn_item_list_get_game(
                ball_pocket,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    int capacity = 0;
    error = pkmn_item_list_get_capacity(
                ball_pocket,
                &capacity
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(16, capacity);

    int num_items = 0;
    error = pkmn_item_list_get_num_items(
                ball_pocket,
                &num_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_items);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(ball_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        ball_pocket,
        "Poké Ball"
    );

    // Make sure we can't add items from other pockets.
    static const char* colosseum_wrong_pocket_items[] = {
        "Ein File S", "Potion", "TM01", "Oran Berry", "Joy Scent"
    };
    static const char* xd_wrong_pocket_items[] = {
        "Miror Radar", "Potion", "TM01", "Oran Berry", "Joy Scent", "Battle CD 01"
    };
    test_item_list_invalid_items(
        ball_pocket,
        (colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items),
        (colosseum ? 5 : 6)
    );

    // Make sure we can't add items from other generations.
    static const char* wrong_generation_items[] = {
        "Moon Ball", "Heal Ball", "Dream Ball"
    };
    test_item_list_invalid_items(
        ball_pocket,
        wrong_generation_items,
        3
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const char* item_names[] = {
        "Master Ball", "Ultra Ball", "Great Ball", "Poké Ball",
        "Safari Ball", "Net Ball", "Dive Ball", "Nest Ball"
    };
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
    error = pkmn_item_list_get_valid_items(
                ball_pocket,
                &valid_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

void gcn_tm_pocket_test(
    pkmn_item_list_handle_t tm_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(tm_pocket);
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    bool colosseum = !strcmp(game, "Colosseum");

    error = pkmn_item_list_get_name(
                tm_pocket,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("TMs", strbuffer);

    error = pkmn_item_list_get_game(
                tm_pocket,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    int capacity = 0;
    error = pkmn_item_list_get_capacity(
                tm_pocket,
                &capacity
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(64, capacity);

    int num_items = 0;
    error = pkmn_item_list_get_num_items(
                tm_pocket,
                &num_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_items);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(tm_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        tm_pocket,
        "Potion"
    );

    // Make sure we can't add items from other pockets.
    static const char* colosseum_wrong_pocket_items[] = {
        "Ein File S", "Great Ball", "Potion", "Oran Berry", "Joy Scent"
    };
    static const char* xd_wrong_pocket_items[] = {
        "Miror Radar", "Great Ball", "Potion", "Oran Berry", "Joy Scent", "Battle CD 01"
    };
    test_item_list_invalid_items(
        tm_pocket,
        (colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items),
        (colosseum ? 5 : 6)
    );

    // Make sure we can't add items from other generations.
    static const char* wrong_generation_items[] = {
        "Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"
    };
    test_item_list_invalid_items(
        tm_pocket,
        wrong_generation_items,
        4
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const char* item_names[] = {
        "TM01", "TM02", "TM03", "TM04",
        "TM05", "TM06", "TM07", "TM08"
    };
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
    error = pkmn_item_list_get_valid_items(
                tm_pocket,
                &valid_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(50, valid_items.length);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

void gcn_berry_pocket_test(
    pkmn_item_list_handle_t berry_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(berry_pocket);
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    bool colosseum = !strcmp(game, "Colosseum");

    error = pkmn_item_list_get_name(
                berry_pocket,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Berries", strbuffer);

    error = pkmn_item_list_get_game(
                berry_pocket,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    int capacity = 0;
    error = pkmn_item_list_get_capacity(
                berry_pocket,
                &capacity
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(46, capacity);

    int num_items = 0;
    error = pkmn_item_list_get_num_items(
                berry_pocket,
                &num_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_items);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(berry_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        berry_pocket,
        "Oran Berry"
    );

    // Make sure we can't add items from other pockets.
    static const char* colosseum_wrong_pocket_items[] = {
        "Ein File S", "Potion", "TM01", "Potion", "Joy Scent"
    };
    static const char* xd_wrong_pocket_items[] = {
        "Miror Radar", "Potion", "TM01", "Potion", "Joy Scent", "Battle CD 01"
    };
    test_item_list_invalid_items(
        berry_pocket,
        (colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items),
        (colosseum ? 5 : 6)
    );

    // Make sure we can't add items from other generations.
    static const char* wrong_generation_items[] = {
        "Berry", "Occa Berry", "Roseli Berry"
    };
    test_item_list_invalid_items(
        berry_pocket,
        wrong_generation_items,
        3
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const char* item_names[] = {
        "Cheri Berry", "Razz Berry", "Lum Berry", "Pinap Berry",
        "Aspear Berry", "Iapapa Berry", "Wiki Berry", "Apicot Berry"
    };
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
    error = pkmn_item_list_get_valid_items(
                berry_pocket,
                &valid_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

void gcn_cologne_pocket_test(
    pkmn_item_list_handle_t cologne_pocket,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(cologne_pocket);
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    bool colosseum = !strcmp(game, "Colosseum");

    error = pkmn_item_list_get_name(
                cologne_pocket,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Colognes", strbuffer);

    error = pkmn_item_list_get_game(
                cologne_pocket,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    int capacity = 0;
    error = pkmn_item_list_get_capacity(
                cologne_pocket,
                &capacity
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(3, capacity);

    int num_items = 0;
    error = pkmn_item_list_get_num_items(
                cologne_pocket,
                &num_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_items);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(cologne_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        cologne_pocket,
        "Joy Scent"
    );

    // Make sure we can't add items from other pockets.
    static const char* colosseum_wrong_pocket_items[] = {
        "Ein File S", "Potion", "TM01", "Potion", "Oran Berry"
    };
    static const char* xd_wrong_pocket_items[] = {
        "Miror Radar", "Potion", "TM01", "Potion", "Oran Berry", "Battle CD 01"
    };
    test_item_list_invalid_items(
        cologne_pocket,
        (colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items),
        (colosseum ? 5 : 6)
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

    pkmn_item_slots_t item_slots =
    {
        .item_slots = NULL,
        .length = 0
    };
    error = pkmn_item_list_as_array(
                cologne_pocket,
                &item_slots
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL(3, item_slots.length);

    TEST_ASSERT_EQUAL_STRING("Joy Scent", item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(3, item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("Vivid Scent", item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(2, item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, item_slots.item_slots[2].amount);

    error = pkmn_item_slots_free(&item_slots);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL(0, item_slots.length);

    error = pkmn_item_list_remove(
                cologne_pocket,
                "Vivid Scent",
                2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_remove(
                cologne_pocket,
                "Joy Scent",
                3
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

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
    TEST_ASSERT_EQUAL(3, valid_items.length);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

void xd_battle_cd_pocket_test(
    pkmn_item_list_handle_t battle_cd_pocket
)
{
    TEST_ASSERT_NOT_NULL(battle_cd_pocket);

    pkmn_error_t error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    error = pkmn_item_list_get_name(
                battle_cd_pocket,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Battle CDs", strbuffer);

    error = pkmn_item_list_get_game(
                battle_cd_pocket,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("XD", strbuffer);

    int capacity = 0;
    error = pkmn_item_list_get_capacity(
                battle_cd_pocket,
                &capacity
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(60, capacity);

    int num_items = 0;
    error = pkmn_item_list_get_num_items(
                battle_cd_pocket,
                &num_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_items);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(battle_cd_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        battle_cd_pocket,
        "Battle CD 01"
    );

    // Make sure we can't add items from other pockets.
    static const char* wrong_pocket_items[] = {
        "Miror Radar", "Great Ball", "TM01", "Oran Berry", "Joy Scent", "Potion"
    };
    test_item_list_invalid_items(
        battle_cd_pocket,
        wrong_pocket_items,
        6
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const char* item_names[] = {
        "Battle CD 01", "Battle CD 02", "Battle CD 03", "Battle CD 04",
        "Battle CD 05", "Battle CD 06", "Battle CD 07", "Battle CD 08"
    };
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
    error = pkmn_item_list_get_valid_items(
                battle_cd_pocket,
                &valid_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(60, valid_items.length);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

void gcn_item_pc_test(
    pkmn_item_list_handle_t item_pc,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(item_pc);
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    bool colosseum = !strcmp(game, "Colosseum");

    error = pkmn_item_list_get_name(
                item_pc,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("PC", strbuffer);

    error = pkmn_item_list_get_game(
                item_pc,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    int capacity = 0;
    error = pkmn_item_list_get_capacity(
                item_pc,
                &capacity
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(235, capacity);

    int num_items = 0;
    error = pkmn_item_list_get_num_items(
                item_pc,
                &num_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_items);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(item_pc);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        item_pc,
        "Potion"
    );

    // Make sure we can't add items from other generations or incompatible Generation III games.
    test_item_list_invalid_items(
        item_pc,
        (colosseum ? colosseum_wrong_game_all_pocket_items : xd_wrong_game_all_pocket_items),
        (colosseum ? 5 : 6)
    );

    // Make sure we can't add items from other generations.
    static const char* wrong_generation_items[] = {
        "Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"
    };
    test_item_list_invalid_items(
        item_pc,
        wrong_generation_items,
        4
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    test_item_list_add_remove(
        item_pc,
        (colosseum ? colosseum_all_pocket_items : xd_all_pocket_items),
        8
    );

    pkmn_string_list_t valid_items =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                item_pc,
                &valid_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(valid_items.strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

void gcn_item_bag_test(
    pkmn_item_bag_handle_t bag,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(bag);
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    bool colosseum = !strcmp(game, "Colosseum");

    pkmn_string_list_t pocket_names = {
        .strings = NULL,
        .length = 0
    };

    // Check unchanging and initial values.
    error = pkmn_item_bag_get_game(
                bag,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    error = pkmn_item_bag_get_pocket_names(
                bag,
                &pocket_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL((colosseum ? 6 : 7), pocket_names.length);

    pkmn_string_list_free(&pocket_names);
    TEST_ASSERT_NULL(pocket_names.strings);
    TEST_ASSERT_EQUAL(0, pocket_names.length);

    // Test pockets.
    pkmn_item_list_handle_t item_list_handle = NULL;
    pkmn_item_list_handle_t key_item_list_handle = NULL;
    pkmn_item_list_handle_t ball_list_handle = NULL;
    pkmn_item_list_handle_t tm_list_handle = NULL;
    pkmn_item_list_handle_t berry_list_handle = NULL;
    pkmn_item_list_handle_t cologne_list_handle = NULL;
    pkmn_item_list_handle_t battle_cd_list_handle = NULL; // XD only
    int num_items = 0;

    error = pkmn_item_bag_get_pocket(
                bag,
                "Items",
                &item_list_handle
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_list_handle);
    error = pkmn_item_list_get_num_items(
                item_list_handle,
                &num_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_items);

    error = pkmn_item_bag_get_pocket(
                bag,
                "Key Items",
                &key_item_list_handle
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(key_item_list_handle);
    error = pkmn_item_list_get_num_items(
                key_item_list_handle,
                &num_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_items);

    error = pkmn_item_bag_get_pocket(
                bag,
                "Poké Balls",
                &ball_list_handle
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(ball_list_handle);
    error = pkmn_item_list_get_num_items(
                ball_list_handle,
                &num_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_items);

    error = pkmn_item_bag_get_pocket(
                bag,
                "TMs",
                &tm_list_handle
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(tm_list_handle);
    error = pkmn_item_list_get_num_items(
                tm_list_handle,
                &num_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_items);

    error = pkmn_item_bag_get_pocket(
                bag,
                "Berries",
                &berry_list_handle
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(berry_list_handle);
    error = pkmn_item_list_get_num_items(
                berry_list_handle,
                &num_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_items);

    error = pkmn_item_bag_get_pocket(
                bag,
                "Colognes",
                &cologne_list_handle
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(cologne_list_handle);
    error = pkmn_item_list_get_num_items(
                cologne_list_handle,
                &num_items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_items);

    if(!colosseum)
    {
        error = pkmn_item_bag_get_pocket(
                    bag,
                    "Battle CDs",
                    &battle_cd_list_handle
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(battle_cd_list_handle);
        error = pkmn_item_list_get_num_items(
                    battle_cd_list_handle,
                    &num_items
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0, num_items);
    }

    // Make sure we can't add items from later generations.
    test_item_bag_invalid_items(
        bag,
        colosseum ? colosseum_wrong_game_all_pocket_items
                  : xd_wrong_game_all_pocket_items,
        9
    );

    gcn_item_pocket_test(
        item_list_handle,
        game
    );
    gcn_key_item_pocket_test(
        key_item_list_handle,
        game
    );
    gcn_ball_pocket_test(
        ball_list_handle,
        game
    );
    gcn_tm_pocket_test(
        tm_list_handle,
        game
    );
    gcn_berry_pocket_test(
        berry_list_handle,
        game
    );
    gcn_cologne_pocket_test(
        cologne_list_handle,
        game
    );
    if(!colosseum)
    {
        xd_battle_cd_pocket_test(
            battle_cd_list_handle
        );
    }

    pkmn_item_slots_t item_slots =
    {
        .item_slots = NULL,
        .length = 0
    };

    // Test adding items through the bag.
    for(size_t i = 0; i < 8; ++i)
    {
        error = pkmn_item_bag_add(
                    bag,
                    colosseum ? colosseum_all_pocket_items[i]
                              : xd_all_pocket_items[i],
                    5
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    error = pkmn_item_list_as_array(
                item_list_handle,
                &item_slots
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL_STRING("Potion", item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, item_slots.item_slots[1].amount);
    error = pkmn_item_slots_free(&item_slots);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(item_slots.item_slots);

    const char* gcn_item = colosseum ? "Ein File S" : "Gonzap's Key";
    error = pkmn_item_list_as_array(
                key_item_list_handle,
                &item_slots
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL_STRING(gcn_item, item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, item_slots.item_slots[1].amount);
    error = pkmn_item_slots_free(&item_slots);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(item_slots.item_slots);

    error = pkmn_item_list_as_array(
                ball_list_handle,
                &item_slots
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL_STRING("Great Ball", item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, item_slots.item_slots[1].amount);
    error = pkmn_item_slots_free(&item_slots);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(item_slots.item_slots);

    error = pkmn_item_list_as_array(
                tm_list_handle,
                &item_slots
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL_STRING("TM01", item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("TM02", item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(5, item_slots.item_slots[1].amount);
    TEST_ASSERT_EQUAL_STRING("None", item_slots.item_slots[2].item);
    TEST_ASSERT_EQUAL(0, item_slots.item_slots[2].amount);
    error = pkmn_item_slots_free(&item_slots);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(item_slots.item_slots);

    error = pkmn_item_list_as_array(
                berry_list_handle,
                &item_slots
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL_STRING("Aspear Berry", item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, item_slots.item_slots[0].amount);
    TEST_ASSERT_EQUAL_STRING("None", item_slots.item_slots[1].item);
    TEST_ASSERT_EQUAL(0, item_slots.item_slots[1].amount);
    error = pkmn_item_slots_free(&item_slots);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(item_slots.item_slots);

    error = pkmn_item_list_as_array(
                cologne_list_handle,
                &item_slots
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL_STRING("Joy Scent", item_slots.item_slots[0].item);
    TEST_ASSERT_EQUAL(5, item_slots.item_slots[0].amount);
    if(colosseum)
    {
        TEST_ASSERT_EQUAL_STRING("Excite Scent", item_slots.item_slots[1].item);
        TEST_ASSERT_EQUAL(5, item_slots.item_slots[1].amount);
        TEST_ASSERT_EQUAL_STRING("None", item_slots.item_slots[2].item);
        TEST_ASSERT_EQUAL(0, item_slots.item_slots[2].amount);
        error = pkmn_item_slots_free(&item_slots);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(item_slots.item_slots);
    }
    else
    {
        TEST_ASSERT_EQUAL_STRING("None", item_slots.item_slots[1].item);
        TEST_ASSERT_EQUAL(0, item_slots.item_slots[1].amount);
        error = pkmn_item_slots_free(&item_slots);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(item_slots.item_slots);

        error = pkmn_item_list_as_array(
                    battle_cd_list_handle,
                    &item_slots
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(item_slots.item_slots);
        TEST_ASSERT_EQUAL_STRING("Battle CD 01", item_slots.item_slots[0].item);
        TEST_ASSERT_EQUAL(5, item_slots.item_slots[0].amount);
        TEST_ASSERT_EQUAL_STRING("None", item_slots.item_slots[1].item);
        TEST_ASSERT_EQUAL(0, item_slots.item_slots[1].amount);
        error = pkmn_item_slots_free(&item_slots);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(item_slots.item_slots);
    }

    if(!colosseum)
    {
        error = pkmn_item_list_free(&battle_cd_list_handle);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(battle_cd_list_handle);
    }

    error = pkmn_item_list_free(&cologne_list_handle);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(cologne_list_handle);

    error = pkmn_item_list_free(&berry_list_handle);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(berry_list_handle);

    error = pkmn_item_list_free(&tm_list_handle);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(tm_list_handle);

    error = pkmn_item_list_free(&ball_list_handle);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(ball_list_handle);

    error = pkmn_item_list_free(&key_item_list_handle);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(key_item_list_handle);

    error = pkmn_item_list_free(&item_list_handle);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(item_list_handle);
}
