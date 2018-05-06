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
    struct pkmn_item_list* item_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(item_pocket_ptr->p_internal);
    TEST_ASSERT_NOT_NULL(game);

    bool is_colosseum = !strcmp(game, "Colosseum");

    size_t capacity = is_colosseum ? 20 : 30;

    TEST_ASSERT_EQUAL_STRING("Items", item_pocket_ptr->p_name);
    TEST_ASSERT_EQUAL_STRING(game, item_pocket_ptr->p_game);
    TEST_ASSERT_EQUAL(capacity, item_pocket_ptr->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(item_pocket_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        item_pocket_ptr,
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
        item_pocket_ptr,
        is_colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_colosseum ? 5 : 6
    );

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"};
    test_item_list_invalid_items(item_pocket_ptr, wrong_generation_item_names, 4);

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

static void gcn_key_item_pocket_test(
    struct pkmn_item_list* key_item_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr);
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr->p_internal);
    TEST_ASSERT_NOT_NULL(game);

    bool is_colosseum = !strcmp(game, "Colosseum");
    const char* gcn_item = is_colosseum ? "Ein File S" : "Miror Radar";

    TEST_ASSERT_EQUAL_STRING("Key Items", key_item_pocket_ptr->p_name);
    TEST_ASSERT_EQUAL_STRING(game, key_item_pocket_ptr->p_game);
    TEST_ASSERT_EQUAL(43, key_item_pocket_ptr->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(key_item_pocket_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(key_item_pocket_ptr, gcn_item);

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
        key_item_pocket_ptr,
        is_colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_colosseum ? 5 : 6
    );

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"};
    test_item_list_invalid_items(key_item_pocket_ptr, wrong_generation_item_names, 4);

    // Make sure we can't add items from incompatible Generation III games.
    const char* frlg_items[] = {"Helix Fossil", "Tea", "Ruby"};;
    const char* emerald_items[] = {"Magma Emblem", "Old Sea Map"};

    test_item_list_invalid_items(key_item_pocket_ptr, frlg_items, 3);
    test_item_list_invalid_items(key_item_pocket_ptr, emerald_items, 2);

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
        key_item_pocket_ptr,
        is_colosseum ? colosseum_item_names : xd_item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        key_item_pocket_ptr,
        is_colosseum ? colosseum_item_names : xd_item_names,
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

static void gcn_ball_pocket_test(
    struct pkmn_item_list* ball_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr);
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr->p_internal);
    TEST_ASSERT_NOT_NULL(game);

    bool is_colosseum = !strcmp(game, "Colosseum");

    TEST_ASSERT_EQUAL_STRING("Poké Balls", ball_pocket_ptr->p_name);
    TEST_ASSERT_EQUAL_STRING(game, ball_pocket_ptr->p_game);
    TEST_ASSERT_EQUAL(16, ball_pocket_ptr->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(ball_pocket_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        ball_pocket_ptr,
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
        ball_pocket_ptr,
        is_colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_colosseum ? 5 : 6
    );

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

static void gcn_tm_pocket_test(
    struct pkmn_item_list* tm_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(tm_pocket_ptr);
    TEST_ASSERT_NOT_NULL(tm_pocket_ptr->p_internal);
    TEST_ASSERT_NOT_NULL(game);

    bool is_colosseum = !strcmp(game, "Colosseum");

    TEST_ASSERT_EQUAL_STRING("TMs", tm_pocket_ptr->p_name);
    TEST_ASSERT_EQUAL_STRING(game, tm_pocket_ptr->p_game);
    TEST_ASSERT_EQUAL(64, tm_pocket_ptr->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(tm_pocket_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        tm_pocket_ptr,
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
        tm_pocket_ptr,
        is_colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_colosseum ? 5 : 6
    );

    // Make sure we can't add items from later games.
    const char* wrong_generation_item_names[] = {"TM51", "HM01"};
    test_item_list_invalid_items(tm_pocket_ptr, wrong_generation_item_names, 2);

    const char* item_names[] =
    {
        "TM01", "TM02", "TM03", "TM04",
        "TM05", "TM06", "TM07", "TM08"
    };

    // Test setting items by index.
    test_item_list_set_item(
        tm_pocket_ptr,
        item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        tm_pocket_ptr,
        item_names,
        8
    );

    struct pkmn_string_list valid_items =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = pkmn_item_list_get_valid_items(
                             tm_pocket_ptr,
                             &valid_items
                         );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.pp_strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.pp_strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gcn_berry_pocket_test(
    struct pkmn_item_list* berry_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(berry_pocket_ptr);
    TEST_ASSERT_NOT_NULL(berry_pocket_ptr->p_internal);
    TEST_ASSERT_NOT_NULL(game);

    bool is_colosseum = !strcmp(game, "Colosseum");

    TEST_ASSERT_EQUAL_STRING("Berries", berry_pocket_ptr->p_name);
    TEST_ASSERT_EQUAL_STRING(game, berry_pocket_ptr->p_game);
    TEST_ASSERT_EQUAL(46, berry_pocket_ptr->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(berry_pocket_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        berry_pocket_ptr,
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
        berry_pocket_ptr,
        is_colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_colosseum ? 5 : 6
    );

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

static void gcn_cologne_pocket_test(
    struct pkmn_item_list* cologne_pocket_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(cologne_pocket_ptr);
    TEST_ASSERT_NOT_NULL(cologne_pocket_ptr->p_internal);
    TEST_ASSERT_NOT_NULL(game);

    bool is_colosseum = !strcmp(game, "Colosseum");

    TEST_ASSERT_EQUAL_STRING("Colognes", cologne_pocket_ptr->p_name);
    TEST_ASSERT_EQUAL_STRING(game, cologne_pocket_ptr->p_game);
    TEST_ASSERT_EQUAL(3, cologne_pocket_ptr->capacity);

    enum pkmn_error error = PKMN_ERROR_NONE;

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(cologne_pocket_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        cologne_pocket_ptr,
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
        cologne_pocket_ptr,
        is_colosseum ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_colosseum ? 5 : 6
    );

    // Since this pocket can only have 3 items, we can't use our typical function, which requires 8.

    error = pkmn_item_list_add(
                cologne_pocket_ptr,
                "Joy Scent",
                3
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_add(
                cologne_pocket_ptr,
                "Excite Scent",
                3
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_add(
                cologne_pocket_ptr,
                "Vivid Scent",
                3
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_list_remove(
                cologne_pocket_ptr,
                "Excite Scent",
                3
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_remove(
                cologne_pocket_ptr,
                "Vivid Scent",
                1
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(cologne_pocket_ptr, 0, "Joy Scent", 3);
    check_item_at_index(cologne_pocket_ptr, 1, "Vivid Scent", 2);
    check_item_at_index(cologne_pocket_ptr, 2, "None", 0);

    error = pkmn_item_list_remove(
                cologne_pocket_ptr,
                "Joy Scent",
                3
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_remove(
                cologne_pocket_ptr,
                "Vivid Scent",
                2
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_num_items(cologne_pocket_ptr, 0);

    struct pkmn_string_list valid_items =
    {
        .pp_strings = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                cologne_pocket_ptr,
                &valid_items
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.pp_strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.pp_strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gcn_battle_cd_pocket_test(
    struct pkmn_item_list* battle_cd_pocket_ptr
)
{
    TEST_ASSERT_NOT_NULL(battle_cd_pocket_ptr);
    TEST_ASSERT_NOT_NULL(battle_cd_pocket_ptr->p_internal);

    TEST_ASSERT_EQUAL_STRING("Battle CDs", battle_cd_pocket_ptr->p_name);
    TEST_ASSERT_EQUAL_STRING("XD", battle_cd_pocket_ptr->p_game);
    TEST_ASSERT_EQUAL(60, battle_cd_pocket_ptr->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(battle_cd_pocket_ptr);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        battle_cd_pocket_ptr,
        "Battle CD 01"
    );

    // Make sure we can't add items from other pockets.
    static const char* wrong_pocket_items[] =
    {
        "Miror Radar", "Potion", "Great Ball", "TM01", "Oran Berry", "Joy Scent"
    };
    test_item_list_invalid_items(
        battle_cd_pocket_ptr,
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
        battle_cd_pocket_ptr,
        item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        battle_cd_pocket_ptr,
        item_names,
        8
    );

    struct pkmn_string_list valid_items =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = pkmn_item_list_get_valid_items(
                             battle_cd_pocket_ptr,
                             &valid_items
                         );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(valid_items.pp_strings);
    TEST_ASSERT_TRUE(valid_items.length > 0);

    error = pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.pp_strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

static void gcn_item_pc_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    struct pkmn_item_list item_pc =
    {
        .p_name = NULL,
        .p_game = NULL,
        .capacity = 0,
        .p_internal = NULL
    };

    bool is_colosseum = !strcmp(game, "Colosseum");

    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_item_list_init(
                "PC",
                game,
                &item_pc
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_NOT_NULL(item_pc.p_internal);

    TEST_ASSERT_EQUAL_STRING("PC", item_pc.p_name);
    TEST_ASSERT_EQUAL_STRING(game, item_pc.p_game);
    TEST_ASSERT_EQUAL(235, item_pc.capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(&item_pc);

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

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        &item_pc,
        is_colosseum ? COLOSSEUM_ALL_POCKET_ITEM_NAMES
                     : XD_ALL_POCKET_ITEM_NAMES,
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
                item_pc.p_game,
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

static void get_common_bag_pockets(
    struct pkmn_item_bag* item_bag_ptr,
    struct pkmn_item_list* item_pocket_ptr_out,
    struct pkmn_item_list* key_item_pocket_ptr_out,
    struct pkmn_item_list* ball_pocket_ptr_out,
    struct pkmn_item_list* tm_pocket_ptr_out,
    struct pkmn_item_list* berry_pocket_ptr_out,
    struct pkmn_item_list* cologne_pocket_ptr_out
)
{
    TEST_ASSERT_NOT_NULL(item_bag_ptr);
    TEST_ASSERT_NOT_NULL(item_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(key_item_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(ball_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(tm_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(berry_pocket_ptr_out);
    TEST_ASSERT_NOT_NULL(cologne_pocket_ptr_out);

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
                "TMs",
                tm_pocket_ptr_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                "Berries",
                berry_pocket_ptr_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                item_bag_ptr,
                "Colognes",
                cologne_pocket_ptr_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
}

static void gcn_item_bag_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    enum pkmn_error error = PKMN_ERROR_NONE;

    bool is_colosseum = !strcmp(game, "Colosseum");
    size_t num_pockets = is_colosseum ? 6 : 7;

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
    TEST_ASSERT_NOT_NULL(item_bag.p_internal);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_item_bag_strerror(&item_bag)
    );

    TEST_ASSERT_EQUAL(num_pockets, item_bag.pocket_names.length);

    test_item_bag_pocket_names(&item_bag);

    struct pkmn_item_list item_pocket;
    struct pkmn_item_list key_item_pocket;
    struct pkmn_item_list ball_pocket;
    struct pkmn_item_list tm_pocket;
    struct pkmn_item_list berry_pocket;
    struct pkmn_item_list cologne_pocket;
    struct pkmn_item_list battle_cd_pocket; // XD only

    get_common_bag_pockets(
        &item_bag,
        &item_pocket,
        &key_item_pocket,
        &ball_pocket,
        &tm_pocket,
        &berry_pocket,
        &cologne_pocket
    );
    TEST_ASSERT_NOT_NULL(item_pocket.p_internal);
    TEST_ASSERT_NOT_NULL(key_item_pocket.p_internal);
    TEST_ASSERT_NOT_NULL(ball_pocket.p_internal);
    TEST_ASSERT_NOT_NULL(tm_pocket.p_internal);
    TEST_ASSERT_NOT_NULL(berry_pocket.p_internal);
    TEST_ASSERT_NOT_NULL(cologne_pocket.p_internal);

    gcn_item_pocket_test(&item_pocket, game);
    gcn_key_item_pocket_test(&key_item_pocket, game);
    gcn_ball_pocket_test(&ball_pocket, game);
    gcn_tm_pocket_test(&tm_pocket, game);
    gcn_berry_pocket_test(&berry_pocket, game);
    gcn_cologne_pocket_test(&cologne_pocket, game);

    if(!is_colosseum)
    {
        error = pkmn_item_bag_get_pocket(
                    &item_bag,
                    "Battle CDs",
                    &battle_cd_pocket
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(battle_cd_pocket.p_internal);

        gcn_battle_cd_pocket_test(&battle_cd_pocket);
    }

    // Make sure adding items through the bag adds to the proper pockets.
    check_num_items(&item_pocket, 0);
    check_num_items(&key_item_pocket, 0);
    check_num_items(&ball_pocket, 0);
    check_num_items(&tm_pocket, 0);
    check_num_items(&berry_pocket, 0);

    const char* gcn_key_item = is_colosseum ? "Ein File S" : "Gonzap's Key";
    const char** all_pocket_item_names = is_colosseum ? COLOSSEUM_ALL_POCKET_ITEM_NAMES
                                       : XD_ALL_POCKET_ITEM_NAMES;
    const char** wrong_game_all_pocket_item_names = is_colosseum ? COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEM_NAMES
                                                  : XD_WRONG_GAME_ALL_POCKET_ITEM_NAMES;

    for(size_t item_index = 0; item_index < 8; ++item_index)
    {
        pkmn_item_bag_add(
            &item_bag,
            all_pocket_item_names[item_index],
            5
        );
        PKMN_TEST_ASSERT_SUCCESS(error);
    }

    check_item_at_index(&item_pocket, 0, "Potion", 5);
    check_item_at_index(&item_pocket, 1, "None", 0);

    check_item_at_index(&key_item_pocket, 0, gcn_key_item, 5);
    check_item_at_index(&key_item_pocket, 1, "None", 0);

    check_item_at_index(&ball_pocket, 0, "Great Ball", 5);
    check_item_at_index(&ball_pocket, 1, "None", 0);

    check_item_at_index(&tm_pocket, 0, "TM01", 5);
    check_item_at_index(&tm_pocket, 1, "TM02", 5);
    check_item_at_index(&tm_pocket, 2, "None", 0);

    check_item_at_index(&berry_pocket, 0, "Aspear Berry", 5);
    check_item_at_index(&berry_pocket, 1, "None", 0);

    check_item_at_index(&cologne_pocket, 0, "Joy Scent", 5);

    if(is_colosseum)
    {
        check_item_at_index(&cologne_pocket, 1, "Excite Scent", 5);
    }
    else
    {
        check_item_at_index(&cologne_pocket, 1, "None", 0);

        check_item_at_index(&battle_cd_pocket, 0, "Battle CD 01", 5);
        check_item_at_index(&battle_cd_pocket, 1, "None", 0);
    }

    // Make sure removing items through the bag removes from the proper pockets.
    for(size_t item_index = 0; item_index < 8; ++item_index)
    {
        pkmn_item_bag_remove(
            &item_bag,
            all_pocket_item_names[item_index],
            5
        );
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    check_num_items(&item_pocket, 0);
    check_num_items(&key_item_pocket, 0);
    check_num_items(&ball_pocket, 0);
    check_num_items(&tm_pocket, 0);
    check_num_items(&berry_pocket, 0);
    check_num_items(&cologne_pocket, 0);
    if(!is_colosseum)
    {
        check_num_items(&battle_cd_pocket, 0);
    }

    check_item_at_index(&item_pocket, 0, "None", 0);
    check_item_at_index(&item_pocket, 1, "None", 0);

    check_item_at_index(&key_item_pocket, 0, "None", 0);
    check_item_at_index(&key_item_pocket, 1, "None", 0);

    check_item_at_index(&ball_pocket, 0, "None", 0);
    check_item_at_index(&ball_pocket, 1, "None", 0);

    check_item_at_index(&tm_pocket, 0, "None", 0);
    check_item_at_index(&tm_pocket, 1, "None", 0);
    check_item_at_index(&tm_pocket, 2, "None", 0);

    check_item_at_index(&berry_pocket, 0, "None", 0);
    check_item_at_index(&berry_pocket, 1, "None", 0);

    check_item_at_index(&cologne_pocket, 0, "None", 0);

    if(is_colosseum)
    {
        check_item_at_index(&cologne_pocket, 1, "None", 0);
    }
    else
    {
        check_item_at_index(&cologne_pocket, 1, "None", 0);

        check_item_at_index(&battle_cd_pocket, 0, "None", 0);
        check_item_at_index(&battle_cd_pocket, 1, "None", 0);
    }

    // Make sure we can't add items from later generations.
    test_item_bag_invalid_items(
        &item_bag,
        wrong_game_all_pocket_item_names,
        9
    );

    // Free pockets and bag.

    error = pkmn_item_list_free(&item_pocket);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_list_free(&key_item_pocket);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_list_free(&ball_pocket);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_list_free(&tm_pocket);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_list_free(&berry_pocket);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_list_free(&cologne_pocket);
    PKMN_TEST_ASSERT_SUCCESS(error);

    if(!is_colosseum)
    {
        error = pkmn_item_list_free(&battle_cd_pocket);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }

    error = pkmn_item_bag_free(&item_bag);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

#define GCN_ITEM_TESTS(test_game) \
void test_gcn_item_pocket_ ## test_game () \
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
    gcn_item_pocket_test( \
        &item_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&item_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(item_pocket.p_internal); \
} \
void test_gcn_key_item_pocket_ ## test_game () \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
 \
    struct pkmn_item_list key_item_pocket = \
    { \
        .p_name = NULL, \
        .p_game = NULL, \
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
    gcn_key_item_pocket_test( \
        &key_item_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&key_item_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(key_item_pocket.p_internal); \
} \
void test_gcn_ball_pocket_ ## test_game () \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
 \
    struct pkmn_item_list ball_pocket = \
    { \
        .p_name = NULL, \
        .p_game = NULL, \
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
    gcn_ball_pocket_test( \
        &ball_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&ball_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(ball_pocket.p_internal); \
} \
void test_gcn_tm_pocket_ ## test_game () \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
 \
    struct pkmn_item_list tm_pocket = \
    { \
        .p_name = NULL, \
        .p_game = NULL, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "TMs", \
                #test_game, \
                &tm_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(tm_pocket.p_internal); \
 \
    gcn_tm_pocket_test( \
        &tm_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&tm_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(tm_pocket.p_internal); \
} \
void test_gcn_berry_pocket_ ## test_game () \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
 \
    struct pkmn_item_list berry_pocket = \
    { \
        .p_name = NULL, \
        .p_game = NULL, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "Berries", \
                #test_game, \
                &berry_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(berry_pocket.p_internal); \
 \
    gcn_berry_pocket_test( \
        &berry_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&berry_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(berry_pocket.p_internal); \
} \
void test_gcn_cologne_pocket_ ## test_game () \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
 \
    struct pkmn_item_list cologne_pocket = \
    { \
        .p_name = NULL, \
        .p_game = NULL, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "Colognes", \
                #test_game, \
                &cologne_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(cologne_pocket.p_internal); \
 \
    gcn_cologne_pocket_test( \
        &cologne_pocket, \
        #test_game \
    ); \
 \
    error = pkmn_item_list_free(&cologne_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(cologne_pocket.p_internal); \
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
    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_item_list battle_cd_pocket =
    {
        .p_name = NULL,
        .p_game = NULL,
        .capacity = 0,
        .p_internal = NULL
    };

    error = pkmn_item_list_init(
                "Battle CDs",
                "XD",
                &battle_cd_pocket
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(battle_cd_pocket.p_internal);

    gcn_battle_cd_pocket_test(
        &battle_cd_pocket
    );

    error = pkmn_item_list_free(&battle_cd_pocket);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(battle_cd_pocket.p_internal);
}
