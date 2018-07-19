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

static const enum pkmn_item ALL_POCKET_ITEMS[] =
{
    PKMN_ITEM_POTION,
    PKMN_ITEM_MACH_BIKE,
    PKMN_ITEM_GREAT_BALL,
    PKMN_ITEM_TM01,
    PKMN_ITEM_ASPEAR_BERRY,
    PKMN_ITEM_WAILMER_PAIL,
    PKMN_ITEM_MASTER_BALL,
    PKMN_ITEM_HM04
};
static const enum pkmn_item WRONG_GAME_ALL_POCKET_ITEMS[] =
{
    PKMN_ITEM_PINK_BOW, PKMN_ITEM_BLACK_SLUDGE,
    PKMN_ITEM_EIN_FILE_S, PKMN_ITEM_GONZAPS_KEY,
    PKMN_ITEM_GS_BALL, PKMN_ITEM_POFFIN_CASE,
    PKMN_ITEM_TM51,
    PKMN_ITEM_BERRY, PKMN_ITEM_OCCA_BERRY
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
        PKMN_ITEM_POTION
    );

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item wrong_pocket_items[] =
    {
        PKMN_ITEM_BICYCLE,
        PKMN_ITEM_MASTER_BALL,
        PKMN_ITEM_HM01,
        PKMN_ITEM_RAZZ_BERRY
    };
    test_item_list_invalid_items(p_item_pocket, wrong_pocket_items, 4);

    // Make sure we can't add items from later generations.
    static const enum pkmn_item wrong_generation_items[] =
    {
        PKMN_ITEM_PINK_BOW,
        PKMN_ITEM_BLACK_SLUDGE,
        PKMN_ITEM_BINDING_BAND,
        PKMN_ITEM_BEEDRILLITE
    };
    test_item_list_invalid_items(p_item_pocket, wrong_generation_items, 4);

    // Make sure we can't add items from Gamecube games.
    static const enum pkmn_item gamecube_items[] =
    {
        PKMN_ITEM_TIME_FLUTE,
        PKMN_ITEM_POKE_SNACK
    };
    test_item_list_invalid_items(p_item_pocket, gamecube_items, 2);

    static const enum pkmn_item items[] =
    {
        PKMN_ITEM_POTION,
        PKMN_ITEM_ORANGE_MAIL,
        PKMN_ITEM_LAVA_COOKIE,
        PKMN_ITEM_STARDUST,
        PKMN_ITEM_SHADOW_MAIL,
        PKMN_ITEM_PINK_SCARF,
        PKMN_ITEM_ANTIDOTE,
        PKMN_ITEM_GREEN_SHARD
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
        PKMN_ITEM_BASEMENT_KEY
    );

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item wrong_pocket_items[] =
    {
        PKMN_ITEM_POTION,
        PKMN_ITEM_MASTER_BALL,
        PKMN_ITEM_HM01,
        PKMN_ITEM_RAZZ_BERRY
    };
    test_item_list_invalid_items(p_key_item_pocket, wrong_pocket_items, 4);

    // Make sure we can't add items from later generations.
    static const enum pkmn_item wrong_generation_items[] =
    {
        PKMN_ITEM_GS_BALL,
        PKMN_ITEM_POFFIN_CASE,
        PKMN_ITEM_DNA_SPLICERS,
        PKMN_ITEM_AQUA_SUIT
    };
    test_item_list_invalid_items(p_key_item_pocket, wrong_generation_items, 4);

    // Make sure we can't add items from incompatible Generation III games.
    static const enum pkmn_item gcn_items[] =
    {
        PKMN_ITEM_EIN_FILE_S,
        PKMN_ITEM_POWERUP_PART,
        PKMN_ITEM_GONZAPS_KEY,
        PKMN_ITEM_KRANE_MEMO_1
    };
    static const enum pkmn_item frlg_items[] =
    {
        PKMN_ITEM_HELIX_FOSSIL,
        PKMN_ITEM_TEA,
        PKMN_ITEM_RUBY
    };
    static const enum pkmn_item emerald_items[] =
    {
        PKMN_ITEM_MAGMA_EMBLEM,
        PKMN_ITEM_OLD_SEA_MAP
    };
    test_item_list_invalid_items(p_key_item_pocket, gcn_items, 4);
    if(is_game_rs(game))
    {
        test_item_list_invalid_items(p_key_item_pocket, frlg_items, 3);
    }
    if(game != PKMN_GAME_EMERALD)
    {
        test_item_list_invalid_items(p_key_item_pocket, emerald_items, 2);
    }

    static const enum pkmn_item items[] =
    {
        PKMN_ITEM_WAILMER_PAIL,
        PKMN_ITEM_BASEMENT_KEY,
        PKMN_ITEM_METEORITE,
        PKMN_ITEM_OLD_ROD,
        PKMN_ITEM_RED_ORB,
        PKMN_ITEM_ROOT_FOSSIL,
        PKMN_ITEM_CONTEST_PASS,
        PKMN_ITEM_EON_TICKET
    };

    // Test setting items by index.
    test_item_list_set_item(
        p_key_item_pocket,
        items,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_key_item_pocket,
        items,
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
        PKMN_ITEM_MASTER_BALL
    );

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item wrong_pocket_items[] =
    {
        PKMN_ITEM_BICYCLE,
        PKMN_ITEM_POTION,
        PKMN_ITEM_HM01,
        PKMN_ITEM_RAZZ_BERRY
    };
    test_item_list_invalid_items(p_ball_pocket, wrong_pocket_items, 4);

    // Make sure we can't add items from later generations.
    static const enum pkmn_item wrong_generation_items[] =
    {
        PKMN_ITEM_MOON_BALL,
        PKMN_ITEM_HEAL_BALL,
        PKMN_ITEM_DREAM_BALL
    };
    test_item_list_invalid_items(p_ball_pocket, wrong_generation_items, 3);

    static const enum pkmn_item items[] =
    {
        PKMN_ITEM_MASTER_BALL,
        PKMN_ITEM_ULTRA_BALL,
        PKMN_ITEM_GREAT_BALL,
        PKMN_ITEM_POKE_BALL,
        PKMN_ITEM_SAFARI_BALL,
        PKMN_ITEM_NET_BALL,
        PKMN_ITEM_DIVE_BALL,
        PKMN_ITEM_NEST_BALL
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
        PKMN_ITEM_TM01
    );

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item wrong_pocket_items[] =
    {
        PKMN_ITEM_BICYCLE,
        PKMN_ITEM_POTION,
        PKMN_ITEM_GREAT_BALL,
        PKMN_ITEM_RAZZ_BERRY
    };
    test_item_list_invalid_items(p_tmhm_pocket, wrong_pocket_items, 4);

    // Make sure we can't add items from later generations.
    static const enum pkmn_item wrong_generation_items[] =
    {
        PKMN_ITEM_TM51
    };
    test_item_list_invalid_items(p_tmhm_pocket, wrong_generation_items, 1);

    static const enum pkmn_item items[] =
    {
        PKMN_ITEM_TM01,
        PKMN_ITEM_HM01,
        PKMN_ITEM_TM02,
        PKMN_ITEM_HM02,
        PKMN_ITEM_TM03,
        PKMN_ITEM_HM03,
        PKMN_ITEM_TM04,
        PKMN_ITEM_HM04
    };

    // Test setting items by index.
    test_item_list_set_item(
        p_tmhm_pocket,
        items,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_tmhm_pocket,
        items,
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
        PKMN_ITEM_ORAN_BERRY
    );

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item wrong_pocket_items[] =
    {
        PKMN_ITEM_BICYCLE,
        PKMN_ITEM_POTION,
        PKMN_ITEM_GREAT_BALL,
        PKMN_ITEM_TM01
    };
    test_item_list_invalid_items(p_berry_pocket, wrong_pocket_items, 4);

    // Make sure we can't add items from later generations.
    static const enum pkmn_item wrong_generation_items[] =
    {
        PKMN_ITEM_BERRY,
        PKMN_ITEM_OCCA_BERRY,
        PKMN_ITEM_ROSELI_BERRY
    };
    test_item_list_invalid_items(p_berry_pocket, wrong_generation_items, 3);

    static const enum pkmn_item items[] =
    {
        PKMN_ITEM_CHERI_BERRY,
        PKMN_ITEM_RAZZ_BERRY,
        PKMN_ITEM_LUM_BERRY,
        PKMN_ITEM_PINAP_BERRY,
        PKMN_ITEM_ASPEAR_BERRY,
        PKMN_ITEM_IAPAPA_BERRY,
        PKMN_ITEM_WIKI_BERRY,
        PKMN_ITEM_APICOT_BERRY
    };

    // Test setting items by index.
    test_item_list_set_item(
        p_berry_pocket,
        items,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_berry_pocket,
        items,
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
        PKMN_ITEM_POTION
    );

    // Make sure we can't add items from later generations.
    test_item_list_invalid_items(
        &item_pc,
        WRONG_GAME_ALL_POCKET_ITEMS,
        9
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
            ALL_POCKET_ITEMS[item_index],
            5
        );
        PKMN_TEST_ASSERT_SUCCESS(error);
    }

    check_item_at_index(&item_pocket, 0, PKMN_ITEM_POTION, 5);
    check_item_at_index(&item_pocket, 1, PKMN_ITEM_NONE, 0);

    check_item_at_index(&key_item_pocket, 0, PKMN_ITEM_MACH_BIKE, 5);
    check_item_at_index(&key_item_pocket, 1, PKMN_ITEM_WAILMER_PAIL, 5);
    check_item_at_index(&key_item_pocket, 2, PKMN_ITEM_NONE, 0);

    check_item_at_index(&ball_pocket, 0, PKMN_ITEM_GREAT_BALL, 5);
    check_item_at_index(&ball_pocket, 1, PKMN_ITEM_MASTER_BALL, 5);
    check_item_at_index(&ball_pocket, 2, PKMN_ITEM_NONE, 0);

    check_item_at_index(&tmhm_pocket, 0, PKMN_ITEM_TM01, 5);
    check_item_at_index(&tmhm_pocket, 1, PKMN_ITEM_HM04, 5);
    check_item_at_index(&tmhm_pocket, 2, PKMN_ITEM_NONE, 0);

    check_item_at_index(&berry_pocket, 0, PKMN_ITEM_ASPEAR_BERRY, 5);
    check_item_at_index(&berry_pocket, 1, PKMN_ITEM_NONE, 0);

    // Make sure removing items through the bag removes from the proper pockets.
    for(size_t item_index = 0; item_index < 8; ++item_index)
    {
        pkmn_item_bag_remove(
            &item_bag,
            ALL_POCKET_ITEMS[item_index],
            5
        );
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    check_num_items(&item_pocket, 0);
    check_num_items(&key_item_pocket, 0);
    check_num_items(&ball_pocket, 0);
    check_num_items(&tmhm_pocket, 0);
    check_num_items(&berry_pocket, 0);

    check_item_at_index(&item_pocket, 0, PKMN_ITEM_NONE, 0);
    check_item_at_index(&item_pocket, 1, PKMN_ITEM_NONE, 0);

    check_item_at_index(&key_item_pocket, 0, PKMN_ITEM_NONE, 0);
    check_item_at_index(&key_item_pocket, 1, PKMN_ITEM_NONE, 0);
    check_item_at_index(&key_item_pocket, 2, PKMN_ITEM_NONE, 0);

    check_item_at_index(&ball_pocket, 0, PKMN_ITEM_NONE, 0);
    check_item_at_index(&ball_pocket, 1, PKMN_ITEM_NONE, 0);
    check_item_at_index(&ball_pocket, 2, PKMN_ITEM_NONE, 0);

    check_item_at_index(&tmhm_pocket, 0, PKMN_ITEM_NONE, 0);
    check_item_at_index(&tmhm_pocket, 1, PKMN_ITEM_NONE, 0);
    check_item_at_index(&tmhm_pocket, 2, PKMN_ITEM_NONE, 0);

    check_item_at_index(&berry_pocket, 0, PKMN_ITEM_NONE, 0);
    check_item_at_index(&berry_pocket, 1, PKMN_ITEM_NONE, 0);

    // Make sure we can't add items from later generations.
    test_item_bag_invalid_items(
        &item_bag,
        WRONG_GAME_ALL_POCKET_ITEMS,
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
