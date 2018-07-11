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

static inline bool is_game_colosseum(enum pkmn_game game)
{
    return (game == PKMN_GAME_COLOSSEUM);
}

static void gcn_item_pocket_test(
    struct pkmn_item_list* p_item_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_item_pocket);
    TEST_ASSERT_NOT_NULL(p_item_pocket->p_internal);

    enum pkmn_error error = PKMN_ERROR_NONE;

    size_t capacity = is_game_colosseum(game) ? 20 : 30;

    TEST_ASSERT_EQUAL_STRING("Items", p_item_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_item_pocket->game);
    TEST_ASSERT_EQUAL(capacity, p_item_pocket->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_item_pocket,
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
        p_item_pocket,
        is_game_colosseum(game) ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_game_colosseum(game) ? 5 : 6
    );

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"};
    test_item_list_invalid_items(p_item_pocket, wrong_generation_item_names, 4);

    const char* item_names[] =
    {
        "Potion", "Orange Mail", "Lava Cookie", "Stardust",
        "Shadow Mail", "Pink Scarf", "Antidote", "Green Shard"
    };

    // Test setting items by index.
    test_item_list_set_item(
        p_item_pocket,
        item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_item_pocket,
        item_names,
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

static void gcn_key_item_pocket_test(
    struct pkmn_item_list* p_key_item_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_key_item_pocket);
    TEST_ASSERT_NOT_NULL(p_key_item_pocket->p_internal);

    enum pkmn_error error = PKMN_ERROR_NONE;

    const char* gcn_item = is_game_colosseum(game) ? "Ein File S" : "Miror Radar";

    TEST_ASSERT_EQUAL_STRING("Key Items", p_key_item_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_key_item_pocket->game);
    TEST_ASSERT_EQUAL(43, p_key_item_pocket->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_key_item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(p_key_item_pocket, gcn_item);

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
        p_key_item_pocket,
        is_game_colosseum(game) ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_game_colosseum(game) ? 5 : 6
    );

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"};
    test_item_list_invalid_items(p_key_item_pocket, wrong_generation_item_names, 4);

    // Make sure we can't add items from incompatible Generation III games.
    const char* frlg_items[] = {"Helix Fossil", "Tea", "Ruby"};;
    const char* emerald_items[] = {"Magma Emblem", "Old Sea Map"};

    test_item_list_invalid_items(p_key_item_pocket, frlg_items, 3);
    test_item_list_invalid_items(p_key_item_pocket, emerald_items, 2);

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
        p_key_item_pocket,
        is_game_colosseum(game) ? colosseum_item_names : xd_item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_key_item_pocket,
        is_game_colosseum(game) ? colosseum_item_names : xd_item_names,
        8
    );

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

static void gcn_ball_pocket_test(
    struct pkmn_item_list* p_ball_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_ball_pocket);
    TEST_ASSERT_NOT_NULL(p_ball_pocket->p_internal);

    enum pkmn_error error = PKMN_ERROR_NONE;

    TEST_ASSERT_EQUAL_STRING("Poké Balls", p_ball_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_ball_pocket->game);
    TEST_ASSERT_EQUAL(16, p_ball_pocket->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_ball_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_ball_pocket,
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
        p_ball_pocket,
        is_game_colosseum(game) ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_game_colosseum(game) ? 5 : 6
    );

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"Moon Ball", "Heal Ball", "Dream Ball"};
    test_item_list_invalid_items(p_ball_pocket, wrong_generation_item_names, 3);

    const char* item_names[] =
    {
        "Master Ball", "Ultra Ball", "Great Ball", "Poké Ball",
        "Safari Ball", "Net Ball", "Dive Ball", "Nest Ball"
    };

    // Test setting items by index.
    test_item_list_set_item(
        p_ball_pocket,
        item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_ball_pocket,
        item_names,
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

static void gcn_tm_pocket_test(
    struct pkmn_item_list* p_tm_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_tm_pocket);
    TEST_ASSERT_NOT_NULL(p_tm_pocket->p_internal);

    enum pkmn_error error = PKMN_ERROR_NONE;

    TEST_ASSERT_EQUAL_STRING("TMs", p_tm_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_tm_pocket->game);
    TEST_ASSERT_EQUAL(64, p_tm_pocket->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_tm_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_tm_pocket,
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
        p_tm_pocket,
        is_game_colosseum(game) ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_game_colosseum(game) ? 5 : 6
    );

    // Make sure we can't add items from later games.
    const char* wrong_generation_item_names[] = {"TM51", "HM01"};
    test_item_list_invalid_items(p_tm_pocket, wrong_generation_item_names, 2);

    const char* item_names[] =
    {
        "TM01", "TM02", "TM03", "TM04",
        "TM05", "TM06", "TM07", "TM08"
    };

    // Test setting items by index.
    test_item_list_set_item(
        p_tm_pocket,
        item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_tm_pocket,
        item_names,
        8
    );

    struct pkmn_item_enum_list valid_items =
    {
        .p_enums = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                p_tm_pocket,
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
                p_tm_pocket,
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

static void gcn_berry_pocket_test(
    struct pkmn_item_list* p_berry_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_berry_pocket);
    TEST_ASSERT_NOT_NULL(p_berry_pocket->p_internal);

    enum pkmn_error error = PKMN_ERROR_NONE;

    TEST_ASSERT_EQUAL_STRING("Berries", p_berry_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_berry_pocket->game);
    TEST_ASSERT_EQUAL(46, p_berry_pocket->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_berry_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_berry_pocket,
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
        p_berry_pocket,
        is_game_colosseum(game) ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_game_colosseum(game) ? 5 : 6
    );

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"Berry", "Occa Berry", "Roseli Berry"};
    test_item_list_invalid_items(p_berry_pocket, wrong_generation_item_names, 3);

    const char* item_names[] =
    {
        "Cheri Berry", "Razz Berry", "Lum Berry", "Pinap Berry",
        "Aspear Berry", "Iapapa Berry", "Wiki Berry", "Apicot Berry"
    };

    // Test setting items by index.
    test_item_list_set_item(
        p_berry_pocket,
        item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_berry_pocket,
        item_names,
        8
    );

    struct pkmn_item_enum_list valid_items =
    {
        .p_enums = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                p_berry_pocket,
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
                p_berry_pocket,
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

static void gcn_cologne_pocket_test(
    struct pkmn_item_list* p_cologne_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_cologne_pocket);
    TEST_ASSERT_NOT_NULL(p_cologne_pocket->p_internal);

    TEST_ASSERT_EQUAL_STRING("Colognes", p_cologne_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_cologne_pocket->game);
    TEST_ASSERT_EQUAL(3, p_cologne_pocket->capacity);

    enum pkmn_error error = PKMN_ERROR_NONE;

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_cologne_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_cologne_pocket,
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
        p_cologne_pocket,
        is_game_colosseum(game) ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_game_colosseum(game) ? 5 : 6
    );

    // Since this pocket can only have 3 items, we can't use our typical function, which requires 8.

    error = pkmn_item_list_add(
                p_cologne_pocket,
                "Joy Scent",
                3
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_add(
                p_cologne_pocket,
                "Excite Scent",
                3
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_add(
                p_cologne_pocket,
                "Vivid Scent",
                3
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_list_remove(
                p_cologne_pocket,
                "Excite Scent",
                3
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_remove(
                p_cologne_pocket,
                "Vivid Scent",
                1
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(p_cologne_pocket, 0, "Joy Scent", 3);
    check_item_at_index(p_cologne_pocket, 1, "Vivid Scent", 2);
    check_item_at_index(p_cologne_pocket, 2, "None", 0);

    error = pkmn_item_list_remove(
                p_cologne_pocket,
                "Joy Scent",
                3
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_remove(
                p_cologne_pocket,
                "Vivid Scent",
                2
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_num_items(p_cologne_pocket, 0);

    struct pkmn_item_enum_list valid_items =
    {
        .p_enums = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                p_cologne_pocket,
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
                p_cologne_pocket,
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

static void gcn_battle_cd_pocket_test(
    struct pkmn_item_list* p_battle_cd_pocket
)
{
    TEST_ASSERT_NOT_NULL(p_battle_cd_pocket);
    TEST_ASSERT_NOT_NULL(p_battle_cd_pocket->p_internal);

    enum pkmn_error error = PKMN_ERROR_NONE;

    TEST_ASSERT_EQUAL_STRING("Battle CDs", p_battle_cd_pocket->p_name);
    TEST_ASSERT_EQUAL(PKMN_GAME_XD, p_battle_cd_pocket->game);
    TEST_ASSERT_EQUAL(60, p_battle_cd_pocket->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_battle_cd_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_battle_cd_pocket,
        "Battle CD 01"
    );

    // Make sure we can't add items from other pockets.
    static const char* wrong_pocket_items[] =
    {
        "Miror Radar", "Potion", "Great Ball", "TM01", "Oran Berry", "Joy Scent"
    };
    test_item_list_invalid_items(
        p_battle_cd_pocket,
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
        p_battle_cd_pocket,
        item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_battle_cd_pocket,
        item_names,
        8
    );

    struct pkmn_item_enum_list valid_items =
    {
        .p_enums = NULL,
        .length = 0
    };
    error = pkmn_item_list_get_valid_items(
                p_battle_cd_pocket,
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
                p_battle_cd_pocket,
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

static void gcn_item_pc_test(
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

    TEST_ASSERT_NOT_NULL(item_pc.p_internal);

    TEST_ASSERT_EQUAL_STRING("PC", item_pc.p_name);
    TEST_ASSERT_EQUAL(game, item_pc.game);
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
        is_game_colosseum(game) ? COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEM_NAMES
                     : XD_WRONG_GAME_ALL_POCKET_ITEM_NAMES,
        9
    );

    // Test setting items by index.
    test_item_list_set_item(
        &item_pc,
        is_game_colosseum(game) ? COLOSSEUM_ALL_POCKET_ITEM_NAMES
                     : XD_ALL_POCKET_ITEM_NAMES,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        &item_pc,
        is_game_colosseum(game) ? COLOSSEUM_ALL_POCKET_ITEM_NAMES
                     : XD_ALL_POCKET_ITEM_NAMES,
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

static void get_common_bag_pockets(
    struct pkmn_item_bag* p_item_bag,
    struct pkmn_item_list* p_item_pocket_out,
    struct pkmn_item_list* p_key_item_pocket_out,
    struct pkmn_item_list* p_ball_pocket_out,
    struct pkmn_item_list* p_tm_pocket_out,
    struct pkmn_item_list* p_berry_pocket_out,
    struct pkmn_item_list* p_cologne_pocket_out
)
{
    TEST_ASSERT_NOT_NULL(p_item_bag);
    TEST_ASSERT_NOT_NULL(p_item_pocket_out);
    TEST_ASSERT_NOT_NULL(p_key_item_pocket_out);
    TEST_ASSERT_NOT_NULL(p_ball_pocket_out);
    TEST_ASSERT_NOT_NULL(p_tm_pocket_out);
    TEST_ASSERT_NOT_NULL(p_berry_pocket_out);
    TEST_ASSERT_NOT_NULL(p_cologne_pocket_out);

    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_item_bag_get_pocket(
                p_item_bag,
                "Items",
                p_item_pocket_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                p_item_bag,
                "Key Items",
                p_key_item_pocket_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                p_item_bag,
                "Poké Balls",
                p_ball_pocket_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                p_item_bag,
                "TMs",
                p_tm_pocket_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                p_item_bag,
                "Berries",
                p_berry_pocket_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                p_item_bag,
                "Colognes",
                p_cologne_pocket_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
}

static void gcn_item_bag_test(enum pkmn_game game)
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

    size_t num_pockets = is_game_colosseum(game) ? 6 : 7;
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

    if(!is_game_colosseum(game))
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

    const char* gcn_key_item = is_game_colosseum(game) ? "Ein File S" : "Gonzap's Key";
    const char** all_pocket_item_names = is_game_colosseum(game) ? COLOSSEUM_ALL_POCKET_ITEM_NAMES
                                       : XD_ALL_POCKET_ITEM_NAMES;
    const char** wrong_game_all_pocket_item_names = is_game_colosseum(game) ? COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEM_NAMES
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

    if(is_game_colosseum(game))
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
    if(!is_game_colosseum(game))
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

    if(is_game_colosseum(game))
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

    if(!is_game_colosseum(game))
    {
        error = pkmn_item_list_free(&battle_cd_pocket);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }

    error = pkmn_item_bag_free(&item_bag);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

#define GCN_ITEM_TESTS(test_game_enum, test_game) \
void test_gcn_item_pocket_ ## test_game () \
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
    gcn_item_pocket_test( \
        &item_pocket, \
        test_game_enum \
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
        .game = PKMN_GAME_NONE, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "Key Items", \
                test_game_enum, \
                &key_item_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(key_item_pocket.p_internal); \
 \
    gcn_key_item_pocket_test( \
        &key_item_pocket, \
        test_game_enum \
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
        .game = PKMN_GAME_NONE, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "Poké Balls", \
                test_game_enum, \
                &ball_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(ball_pocket.p_internal); \
 \
    gcn_ball_pocket_test( \
        &ball_pocket, \
        test_game_enum \
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
        .game = PKMN_GAME_NONE, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "TMs", \
                test_game_enum, \
                &tm_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(tm_pocket.p_internal); \
 \
    gcn_tm_pocket_test( \
        &tm_pocket, \
        test_game_enum \
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
        .game = PKMN_GAME_NONE, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "Berries", \
                test_game_enum, \
                &berry_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(berry_pocket.p_internal); \
 \
    gcn_berry_pocket_test( \
        &berry_pocket, \
        test_game_enum \
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
        .game = PKMN_GAME_NONE, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    error = pkmn_item_list_init( \
                "Colognes", \
                test_game_enum, \
                &cologne_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(cologne_pocket.p_internal); \
 \
    gcn_cologne_pocket_test( \
        &cologne_pocket, \
        test_game_enum \
    ); \
 \
    error = pkmn_item_list_free(&cologne_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(cologne_pocket.p_internal); \
} \
void test_gcn_item_pc_ ## test_game () \
{ \
    gcn_item_pc_test(test_game_enum); \
} \
void test_gcn_item_bag_ ## test_game () \
{ \
    gcn_item_bag_test(test_game_enum); \
}

GCN_ITEM_TESTS(PKMN_GAME_COLOSSEUM, Colosseum)
GCN_ITEM_TESTS(PKMN_GAME_XD, XD)

void test_gcn_battle_cd_pocket_XD()
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_item_list battle_cd_pocket =
    {
        .p_name = NULL,
        .game = PKMN_GAME_NONE,
        .capacity = 0,
        .p_internal = NULL
    };

    error = pkmn_item_list_init(
                "Battle CDs",
                PKMN_GAME_XD,
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
