/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_items_test.h"
#include "pkmntest-c/util.h"

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

static inline bool is_game_rs(enum pkmn_game game)
{
    return (game == PKMN_GAME_RUBY) || (game == PKMN_GAME_SAPPHIRE);
}

static inline bool is_game_frlg(enum pkmn_game game)
{
    return (game == PKMN_GAME_FIRERED) || (game == PKMN_GAME_LEAFGREEN);
}

static void gba_item_pocket_test(
    struct pkmn_item_list* p_item_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_item_pocket);
    TEST_ASSERT_NOT_NULL(p_item_pocket->p_internal);

    size_t expected_capacity = 0;
    if(is_game_rs(game))
    {
        expected_capacity = 20;
    }
    else if(game == PKMN_GAME_EMERALD)
    {
        expected_capacity = 30;
    }
    else
    {
        expected_capacity = 42;
    }

    enum pkmn_error error = PKMN_ERROR_NONE;

    TEST_ASSERT_EQUAL_STRING("Items", p_item_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_item_pocket->game);
    TEST_ASSERT_EQUAL(expected_capacity, p_item_pocket->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_item_pocket,
        "Potion"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Bicycle", "Master Ball", "HM01", "Razz Berry"};
    test_item_list_invalid_items(p_item_pocket, wrong_pocket_item_names, 4);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"};
    test_item_list_invalid_items(p_item_pocket, wrong_generation_item_names, 4);

    // Make sure we can't add items from Gamecube games.
    const char* gamecube_item_names[] = {"Time Flute", "Poké Snack"};
    test_item_list_invalid_items(p_item_pocket, gamecube_item_names, 2);

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

static void gba_key_item_pocket_test(
    struct pkmn_item_list* p_key_item_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_key_item_pocket);
    TEST_ASSERT_NOT_NULL(p_key_item_pocket->p_internal);

    enum pkmn_error error = PKMN_ERROR_NONE;

    size_t expected_capacity = 0;
    if(is_game_rs(game))
    {
        expected_capacity = 20;
    }
    else
    {
        expected_capacity = 30;
    }

    TEST_ASSERT_EQUAL_STRING("Key Items", p_key_item_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_key_item_pocket->game);
    TEST_ASSERT_EQUAL(expected_capacity, p_key_item_pocket->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_key_item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_key_item_pocket,
        "Basement Key"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Potion", "Master Ball", "HM01", "Razz Berry"};
    test_item_list_invalid_items(p_key_item_pocket, wrong_pocket_item_names, 4);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"};
    test_item_list_invalid_items(p_key_item_pocket, wrong_generation_item_names, 4);

    // Make sure we can't add items from incompatible Generation III games.
    const char* gcn_items[] = {"Ein File S", "Powerup Part", "Gonzap's Key", "Krane Memo 1"};
    const char* frlg_items[] = {"Helix Fossil", "Tea", "Ruby"};;
    const char* emerald_items[] = {"Magma Emblem", "Old Sea Map"};

    test_item_list_invalid_items(p_key_item_pocket, gcn_items, 4);
    if(is_game_rs(game))
    {
        test_item_list_invalid_items(p_key_item_pocket, frlg_items, 3);
    }
    if(game != PKMN_GAME_EMERALD)
    {
        test_item_list_invalid_items(p_key_item_pocket, emerald_items, 2);
    }

    const char* item_names[] =
    {
        "Wailmer Pail", "Basement Key", "Meteorite", "Old Rod",
        "Red Orb", "Root Fossil", "Contest Pass", "Eon Ticket"
    };

    // Test setting items by index.
    test_item_list_set_item(
        p_key_item_pocket,
        item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_key_item_pocket,
        item_names,
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
}

static void gba_ball_pocket_test(
    struct pkmn_item_list* p_ball_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_ball_pocket);
    TEST_ASSERT_NOT_NULL(p_ball_pocket->p_internal);

    size_t expected_capacity = 0;
    if(is_game_frlg(game))
    {
        expected_capacity = 13;
    }
    else
    {
        expected_capacity = 16;
    }

    TEST_ASSERT_EQUAL_STRING("Poké Balls", p_ball_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_ball_pocket->game);
    TEST_ASSERT_EQUAL(expected_capacity, p_ball_pocket->capacity);

    enum pkmn_error error = PKMN_ERROR_NONE;

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_ball_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_ball_pocket,
        "Master Ball"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Bicycle", "Potion", "HM01", "Razz Berry"};
    test_item_list_invalid_items(p_ball_pocket, wrong_pocket_item_names, 4);

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

static void gba_tmhm_pocket_test(
    struct pkmn_item_list* p_tmhm_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_tmhm_pocket);
    TEST_ASSERT_NOT_NULL(p_tmhm_pocket->p_internal);

    enum pkmn_error error = PKMN_ERROR_NONE;

    const char* expected_pocket_name = NULL;
    size_t expected_capacity = 0;
    if(is_game_frlg(game))
    {
        expected_pocket_name = "TM Case";
        expected_capacity = 58;
    }
    else
    {
        expected_pocket_name = "TMs & HMs";
        expected_capacity = 64;
    }

    TEST_ASSERT_EQUAL_STRING(expected_pocket_name, p_tmhm_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_tmhm_pocket->game);
    TEST_ASSERT_EQUAL(expected_capacity, p_tmhm_pocket->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_tmhm_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_tmhm_pocket,
        "TM01"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Bicycle", "Potion", "Great Ball", "Razz Berry"};
    test_item_list_invalid_items(p_tmhm_pocket, wrong_pocket_item_names, 4);

    // Make sure we can't add items from later generations.
    const char* wrong_generation_item_names[] = {"TM51"};
    test_item_list_invalid_items(p_tmhm_pocket, wrong_generation_item_names, 1);

    const char* item_names[] =
    {
        "TM01", "HM01", "TM02", "HM02",
        "TM03", "HM03", "TM04", "HM04"
    };

    // Test setting items by index.
    test_item_list_set_item(
        p_tmhm_pocket,
        item_names,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_tmhm_pocket,
        item_names,
        8
    );

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
    TEST_ASSERT_TRUE(valid_items.length > 0);

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

static void gba_berry_pocket_test(
    struct pkmn_item_list* p_berry_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_berry_pocket);
    TEST_ASSERT_NOT_NULL(p_berry_pocket->p_internal);

    enum pkmn_error error = PKMN_ERROR_NONE;

    const char* expected_pocket_name = NULL;
    size_t expected_capacity = 0;
    if(is_game_frlg(game))
    {
        expected_pocket_name = "Berry Pouch";
        expected_capacity = 43;
    }
    else
    {
        expected_pocket_name = "Berries";
        expected_capacity = 46;
    }

    TEST_ASSERT_EQUAL_STRING(expected_pocket_name, p_berry_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_berry_pocket->game);
    TEST_ASSERT_EQUAL(expected_capacity, p_berry_pocket->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_berry_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        p_berry_pocket,
        "Oran Berry"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_item_names[] = {"Bicycle", "Potion", "Great Ball", "TM01"};
    test_item_list_invalid_items(p_berry_pocket, wrong_pocket_item_names, 4);

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

static void gba_item_pc_test(enum pkmn_game game)
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

    if(is_game_frlg(game))
    {
        TEST_ASSERT_EQUAL(full_item_list.length-2, valid_items.length);
        TEST_ASSERT_EQUAL(full_item_name_list.length-2, valid_item_names.length);

        TEST_ASSERT_FALSE(string_list_contains(&valid_item_names, "Berry Pouch"));
        TEST_ASSERT_FALSE(string_list_contains(&valid_item_names, "TM Case"));
    }
    else
    {
        TEST_ASSERT_EQUAL(full_item_list.length, valid_items.length);
        TEST_ASSERT_EQUAL(full_item_name_list.length, valid_item_names.length);
    }

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
    struct pkmn_item_list* p_tmhm_pocket_out,
    const char* tmhm_pocket_name,
    struct pkmn_item_list* p_berry_pocket_out,
    const char* berry_pocket_name
)
{
    TEST_ASSERT_NOT_NULL(p_item_bag);
    TEST_ASSERT_NOT_NULL(p_item_pocket_out);
    TEST_ASSERT_NOT_NULL(p_key_item_pocket_out);
    TEST_ASSERT_NOT_NULL(p_ball_pocket_out);
    TEST_ASSERT_NOT_NULL(p_tmhm_pocket_out);
    TEST_ASSERT_NOT_NULL(tmhm_pocket_name);
    TEST_ASSERT_NOT_NULL(p_berry_pocket_out);
    TEST_ASSERT_NOT_NULL(berry_pocket_name);

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
                tmhm_pocket_name,
                p_tmhm_pocket_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_bag_get_pocket(
                p_item_bag,
                berry_pocket_name,
                p_berry_pocket_out
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
}

static void gba_item_bag_test(enum pkmn_game game)
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
    TEST_ASSERT_EQUAL(5, item_bag.pocket_names.length);

    test_item_bag_pocket_names(&item_bag);

    const char* tmhm_pocket_name = NULL;
    const char* berry_pocket_name = NULL;
    if(is_game_frlg(game))
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

#define GBA_ITEM_TESTS(test_game_enum, test_game) \
void test_gba_item_pocket_ ## test_game () \
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
    gba_item_pocket_test( \
        &item_pocket, \
        test_game_enum \
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
    gba_key_item_pocket_test( \
        &key_item_pocket, \
        test_game_enum \
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
    gba_ball_pocket_test( \
        &ball_pocket, \
        test_game_enum \
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
        .p_name = NULL, \
        .game = PKMN_GAME_NONE, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    const char* pocket_name = NULL; \
    if(is_game_frlg(test_game_enum)) \
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
                test_game_enum, \
                &tmhm_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(tmhm_pocket.p_internal); \
 \
    gba_tmhm_pocket_test( \
        &tmhm_pocket, \
        test_game_enum \
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
        .p_name = NULL, \
        .game = PKMN_GAME_NONE, \
        .capacity = 0, \
        .p_internal = NULL \
    }; \
 \
    const char* pocket_name = NULL; \
    if(is_game_frlg(test_game_enum)) \
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
                test_game_enum, \
                &berry_pocket \
            ); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NOT_NULL(berry_pocket.p_internal); \
 \
    gba_berry_pocket_test( \
        &berry_pocket, \
        test_game_enum \
    ); \
 \
    error = pkmn_item_list_free(&berry_pocket); \
    PKMN_TEST_ASSERT_SUCCESS(error); \
    TEST_ASSERT_NULL(berry_pocket.p_internal); \
} \
void test_gba_item_pc_ ## test_game () \
{ \
    gba_item_pc_test(test_game_enum); \
} \
void test_gba_item_bag_ ## test_game () \
{ \
    gba_item_bag_test(test_game_enum); \
}

GBA_ITEM_TESTS(PKMN_GAME_RUBY, Ruby)
GBA_ITEM_TESTS(PKMN_GAME_SAPPHIRE, Sapphire)
GBA_ITEM_TESTS(PKMN_GAME_EMERALD, Emerald)
GBA_ITEM_TESTS(PKMN_GAME_FIRERED, FireRed)
GBA_ITEM_TESTS(PKMN_GAME_LEAFGREEN, LeafGreen)
