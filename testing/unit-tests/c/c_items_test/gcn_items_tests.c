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

static const enum pkmn_item COLOSSEUM_ALL_POCKET_ITEMS[] =
{
    PKMN_ITEM_POTION,
    PKMN_ITEM_EIN_FILE_S,
    PKMN_ITEM_GREAT_BALL,
    PKMN_ITEM_TM01,
    PKMN_ITEM_TM02,
    PKMN_ITEM_ASPEAR_BERRY,
    PKMN_ITEM_JOY_SCENT,
    PKMN_ITEM_EXCITE_SCENT
};
static const enum pkmn_item XD_ALL_POCKET_ITEMS[] =
{
    PKMN_ITEM_POTION,
    PKMN_ITEM_GONZAPS_KEY,
    PKMN_ITEM_GREAT_BALL,
    PKMN_ITEM_TM01,
    PKMN_ITEM_TM02,
    PKMN_ITEM_ASPEAR_BERRY,
    PKMN_ITEM_JOY_SCENT,
    PKMN_ITEM_BATTLE_CD_01
};
static const enum pkmn_item COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEMS[] =
{
    PKMN_ITEM_PINK_BOW, PKMN_ITEM_BLACK_SLUDGE,
    PKMN_ITEM_GS_BALL, PKMN_ITEM_GONZAPS_KEY, PKMN_ITEM_POFFIN_CASE,
    PKMN_ITEM_TM51, PKMN_ITEM_HM01,
    PKMN_ITEM_BERRY, PKMN_ITEM_OCCA_BERRY
};
static const enum pkmn_item XD_WRONG_GAME_ALL_POCKET_ITEMS[] =
{
    PKMN_ITEM_PINK_BOW, PKMN_ITEM_BLACK_SLUDGE,
    PKMN_ITEM_GS_BALL, PKMN_ITEM_EIN_FILE_S, PKMN_ITEM_POFFIN_CASE,
    PKMN_ITEM_TM51, PKMN_ITEM_HM01,
    PKMN_ITEM_BERRY, PKMN_ITEM_OCCA_BERRY
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
        PKMN_ITEM_POTION
    );

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item colosseum_wrong_pocket_items[] =
    {
        PKMN_ITEM_EIN_FILE_S,
        PKMN_ITEM_GREAT_BALL,
        PKMN_ITEM_TM01,
        PKMN_ITEM_ORAN_BERRY,
        PKMN_ITEM_JOY_SCENT
    };
    static const enum pkmn_item xd_wrong_pocket_items[] =
    {
        PKMN_ITEM_MIROR_RADAR,
        PKMN_ITEM_GREAT_BALL,
        PKMN_ITEM_TM01,
        PKMN_ITEM_ORAN_BERRY,
        PKMN_ITEM_JOY_SCENT,
        PKMN_ITEM_BATTLE_CD_01
    };
    test_item_list_invalid_items(
        p_item_pocket,
        is_game_colosseum(game) ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_game_colosseum(game) ? 5 : 6
    );

    // Make sure we can't add items from later generations.
    static const enum pkmn_item wrong_generation_items[] =
    {
        PKMN_ITEM_PINK_BOW,
        PKMN_ITEM_BLACK_SLUDGE,
        PKMN_ITEM_BINDING_BAND,
        PKMN_ITEM_BEEDRILLITE
    };
    test_item_list_invalid_items(p_item_pocket, wrong_generation_items, 4);

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

static void gcn_key_item_pocket_test(
    struct pkmn_item_list* p_key_item_pocket,
    enum pkmn_game game
)
{
    TEST_ASSERT_NOT_NULL(p_key_item_pocket);
    TEST_ASSERT_NOT_NULL(p_key_item_pocket->p_internal);

    enum pkmn_error error = PKMN_ERROR_NONE;

    enum pkmn_item gcn_item = is_game_colosseum(game) ? PKMN_ITEM_EIN_FILE_S
                                                      : PKMN_ITEM_MIROR_RADAR;

    TEST_ASSERT_EQUAL_STRING("Key Items", p_key_item_pocket->p_name);
    TEST_ASSERT_EQUAL(game, p_key_item_pocket->game);
    TEST_ASSERT_EQUAL(43, p_key_item_pocket->capacity);

    // Make sure item slots start as completely empty.
    test_item_list_initial_values(p_key_item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(p_key_item_pocket, gcn_item);

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item colosseum_wrong_pocket_items[] =
    {
        PKMN_ITEM_POTION,
        PKMN_ITEM_GREAT_BALL,
        PKMN_ITEM_TM01,
        PKMN_ITEM_ORAN_BERRY,
        PKMN_ITEM_JOY_SCENT
    };
    static const enum pkmn_item xd_wrong_pocket_items[] =
    {
        PKMN_ITEM_POTION,
        PKMN_ITEM_GREAT_BALL,
        PKMN_ITEM_TM01,
        PKMN_ITEM_ORAN_BERRY,
        PKMN_ITEM_JOY_SCENT,
        PKMN_ITEM_BATTLE_CD_01
    };
    test_item_list_invalid_items(
        p_key_item_pocket,
        is_game_colosseum(game) ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_game_colosseum(game) ? 5 : 6
    );

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

    test_item_list_invalid_items(p_key_item_pocket, frlg_items, 3);
    test_item_list_invalid_items(p_key_item_pocket, emerald_items, 2);

    static const enum pkmn_item colosseum_items[] =
    {
        PKMN_ITEM_JAIL_KEY,
        PKMN_ITEM_ELEVATOR_KEY,
        PKMN_ITEM_SMALL_TABLET,
        PKMN_ITEM_F_DISK,
        PKMN_ITEM_R_DISK,
        PKMN_ITEM_L_DISK,
        PKMN_ITEM_D_DISK,
        PKMN_ITEM_U_DISK
    };
    static const enum pkmn_item xd_items[] =
    {
        PKMN_ITEM_KRANE_MEMO_1,
        PKMN_ITEM_KRANE_MEMO_2,
        PKMN_ITEM_KRANE_MEMO_3,
        PKMN_ITEM_KRANE_MEMO_4,
        PKMN_ITEM_KRANE_MEMO_5,
        PKMN_ITEM_VOICE_CASE_1,
        PKMN_ITEM_VOICE_CASE_2,
        PKMN_ITEM_VOICE_CASE_3
    };

    // Test setting items by index.
    test_item_list_set_item(
        p_key_item_pocket,
        is_game_colosseum(game) ? colosseum_items : xd_items,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_key_item_pocket,
        is_game_colosseum(game) ? colosseum_items : xd_items,
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
        PKMN_ITEM_MASTER_BALL
    );

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item colosseum_wrong_pocket_items[] =
    {
        PKMN_ITEM_EIN_FILE_S,
        PKMN_ITEM_POTION,
        PKMN_ITEM_TM01,
        PKMN_ITEM_ORAN_BERRY,
        PKMN_ITEM_JOY_SCENT
    };
    static const enum pkmn_item xd_wrong_pocket_items[] =
    {
        PKMN_ITEM_MIROR_RADAR,
        PKMN_ITEM_POTION,
        PKMN_ITEM_TM01,
        PKMN_ITEM_ORAN_BERRY,
        PKMN_ITEM_JOY_SCENT,
        PKMN_ITEM_BATTLE_CD_01
    };
    test_item_list_invalid_items(
        p_ball_pocket,
        is_game_colosseum(game) ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_game_colosseum(game) ? 5 : 6
    );

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
        PKMN_ITEM_TM01
    );

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item colosseum_wrong_pocket_items[] =
    {
        PKMN_ITEM_EIN_FILE_S,
        PKMN_ITEM_POTION,
        PKMN_ITEM_GREAT_BALL,
        PKMN_ITEM_ORAN_BERRY,
        PKMN_ITEM_JOY_SCENT
    };
    static const enum pkmn_item xd_wrong_pocket_items[] =
    {
        PKMN_ITEM_MIROR_RADAR,
        PKMN_ITEM_POTION,
        PKMN_ITEM_GREAT_BALL,
        PKMN_ITEM_ORAN_BERRY,
        PKMN_ITEM_JOY_SCENT,
        PKMN_ITEM_BATTLE_CD_01
    };
    test_item_list_invalid_items(
        p_tm_pocket,
        is_game_colosseum(game) ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_game_colosseum(game) ? 5 : 6
    );

    // Make sure we can't add items from later games.
    static const enum pkmn_item wrong_generation_items[] =
    {
        PKMN_ITEM_TM51,
        PKMN_ITEM_HM01
    };
    test_item_list_invalid_items(p_tm_pocket, wrong_generation_items, 2);

    static const enum pkmn_item items[] =
    {
        PKMN_ITEM_TM01,
        PKMN_ITEM_TM02,
        PKMN_ITEM_TM03,
        PKMN_ITEM_TM04,
        PKMN_ITEM_TM05,
        PKMN_ITEM_TM06,
        PKMN_ITEM_TM07,
        PKMN_ITEM_TM08
    };

    // Test setting items by index.
    test_item_list_set_item(
        p_tm_pocket,
        items,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_tm_pocket,
        items,
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
        PKMN_ITEM_ORAN_BERRY
    );

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item colosseum_wrong_pocket_items[] =
    {
        PKMN_ITEM_EIN_FILE_S,
        PKMN_ITEM_POTION,
        PKMN_ITEM_GREAT_BALL,
        PKMN_ITEM_TM01,
        PKMN_ITEM_JOY_SCENT
    };
    static const enum pkmn_item xd_wrong_pocket_items[] =
    {
        PKMN_ITEM_MIROR_RADAR,
        PKMN_ITEM_POTION,
        PKMN_ITEM_GREAT_BALL,
        PKMN_ITEM_TM01,
        PKMN_ITEM_JOY_SCENT,
        PKMN_ITEM_BATTLE_CD_01
    };
    test_item_list_invalid_items(
        p_berry_pocket,
        is_game_colosseum(game) ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_game_colosseum(game) ? 5 : 6
    );

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
        PKMN_ITEM_JOY_SCENT
    );

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item colosseum_wrong_pocket_items[] =
    {
        PKMN_ITEM_EIN_FILE_S,
        PKMN_ITEM_POTION,
        PKMN_ITEM_GREAT_BALL,
        PKMN_ITEM_TM01,
        PKMN_ITEM_ORAN_BERRY
    };
    static const enum pkmn_item xd_wrong_pocket_items[] =
    {
        PKMN_ITEM_MIROR_RADAR,
        PKMN_ITEM_POTION,
        PKMN_ITEM_GREAT_BALL,
        PKMN_ITEM_TM01,
        PKMN_ITEM_ORAN_BERRY,
        PKMN_ITEM_BATTLE_CD_01
    };
    test_item_list_invalid_items(
        p_cologne_pocket,
        is_game_colosseum(game) ? colosseum_wrong_pocket_items : xd_wrong_pocket_items,
        is_game_colosseum(game) ? 5 : 6
    );

    // Since this pocket can only have 3 items, we can't use our typical function, which requires 8.

    error = pkmn_item_list_add(
                p_cologne_pocket,
                PKMN_ITEM_JOY_SCENT,
                3
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_add(
                p_cologne_pocket,
                PKMN_ITEM_EXCITE_SCENT,
                3
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_add(
                p_cologne_pocket,
                PKMN_ITEM_VIVID_SCENT,
                3
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_list_remove(
                p_cologne_pocket,
                PKMN_ITEM_EXCITE_SCENT,
                3
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_remove(
                p_cologne_pocket,
                PKMN_ITEM_VIVID_SCENT,
                1
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(p_cologne_pocket, 0, PKMN_ITEM_JOY_SCENT, 3);
    check_item_at_index(p_cologne_pocket, 1, PKMN_ITEM_VIVID_SCENT, 2);
    check_item_at_index(p_cologne_pocket, 2, PKMN_ITEM_NONE, 0);

    error = pkmn_item_list_remove(
                p_cologne_pocket,
                PKMN_ITEM_JOY_SCENT,
                3
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_remove(
                p_cologne_pocket,
                PKMN_ITEM_VIVID_SCENT,
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
        PKMN_ITEM_BATTLE_CD_01
    );

    // Make sure we can't add items from other pockets.
    static const enum pkmn_item wrong_pocket_items[] =
    {
        PKMN_ITEM_MIROR_RADAR,
        PKMN_ITEM_POTION,
        PKMN_ITEM_GREAT_BALL,
        PKMN_ITEM_TM01,
        PKMN_ITEM_ORAN_BERRY,
        PKMN_ITEM_JOY_SCENT
    };
    test_item_list_invalid_items(
        p_battle_cd_pocket,
        wrong_pocket_items,
        6
    );

    static const enum pkmn_item items[] =
    {
        PKMN_ITEM_BATTLE_CD_01,
        PKMN_ITEM_BATTLE_CD_02,
        PKMN_ITEM_BATTLE_CD_03,
        PKMN_ITEM_BATTLE_CD_04,
        PKMN_ITEM_BATTLE_CD_05,
        PKMN_ITEM_BATTLE_CD_06,
        PKMN_ITEM_BATTLE_CD_07,
        PKMN_ITEM_BATTLE_CD_08
    };

    // Test setting items by index.
    test_item_list_set_item(
        p_battle_cd_pocket,
        items,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        p_battle_cd_pocket,
        items,
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
        PKMN_ITEM_POTION
    );

    // Make sure we can't add items from incompatible games.
    test_item_list_invalid_items(
        &item_pc,
        is_game_colosseum(game) ? COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEMS
                                : XD_WRONG_GAME_ALL_POCKET_ITEMS,
        9
    );

    // Test setting items by index.
    test_item_list_set_item(
        &item_pc,
        is_game_colosseum(game) ? COLOSSEUM_ALL_POCKET_ITEMS
                                : XD_ALL_POCKET_ITEMS,
        3
    );

    // Start adding and removing items, and make sure the numbers are accurate.
    test_item_list_add_remove(
        &item_pc,
        is_game_colosseum(game) ? COLOSSEUM_ALL_POCKET_ITEMS
                                : XD_ALL_POCKET_ITEMS,
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

    const enum pkmn_item gcn_key_item =
        is_game_colosseum(game) ? PKMN_ITEM_EIN_FILE_S : PKMN_ITEM_GONZAPS_KEY;
    const enum pkmn_item* all_pocket_items =
        is_game_colosseum(game) ? COLOSSEUM_ALL_POCKET_ITEMS : XD_ALL_POCKET_ITEMS;
    const enum pkmn_item* wrong_game_all_pocket_items =
        is_game_colosseum(game) ? COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEMS : XD_WRONG_GAME_ALL_POCKET_ITEMS;

    for(size_t item_index = 0; item_index < 8; ++item_index)
    {
        pkmn_item_bag_add(
            &item_bag,
            all_pocket_items[item_index],
            5
        );
        PKMN_TEST_ASSERT_SUCCESS(error);
    }

    check_item_at_index(&item_pocket, 0, PKMN_ITEM_POTION, 5);
    check_item_at_index(&item_pocket, 1, PKMN_ITEM_NONE, 0);

    check_item_at_index(&key_item_pocket, 0, gcn_key_item, 5);
    check_item_at_index(&key_item_pocket, 1, PKMN_ITEM_NONE, 0);

    check_item_at_index(&ball_pocket, 0, PKMN_ITEM_GREAT_BALL, 5);
    check_item_at_index(&ball_pocket, 1, PKMN_ITEM_NONE, 0);

    check_item_at_index(&tm_pocket, 0, PKMN_ITEM_TM01, 5);
    check_item_at_index(&tm_pocket, 1, PKMN_ITEM_TM02, 5);
    check_item_at_index(&tm_pocket, 2, PKMN_ITEM_NONE, 0);

    check_item_at_index(&berry_pocket, 0, PKMN_ITEM_ASPEAR_BERRY, 5);
    check_item_at_index(&berry_pocket, 1, PKMN_ITEM_NONE, 0);

    check_item_at_index(&cologne_pocket, 0, PKMN_ITEM_JOY_SCENT, 5);

    if(is_game_colosseum(game))
    {
        check_item_at_index(&cologne_pocket, 1, PKMN_ITEM_EXCITE_SCENT, 5);
    }
    else
    {
        check_item_at_index(&cologne_pocket, 1, PKMN_ITEM_NONE, 0);

        check_item_at_index(&battle_cd_pocket, 0, PKMN_ITEM_BATTLE_CD_01, 5);
        check_item_at_index(&battle_cd_pocket, 1, PKMN_ITEM_NONE, 0);
    }

    // Make sure removing items through the bag removes from the proper pockets.
    for(size_t item_index = 0; item_index < 8; ++item_index)
    {
        pkmn_item_bag_remove(
            &item_bag,
            all_pocket_items[item_index],
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

    check_item_at_index(&item_pocket, 0, PKMN_ITEM_NONE, 0);
    check_item_at_index(&item_pocket, 1, PKMN_ITEM_NONE, 0);

    check_item_at_index(&key_item_pocket, 0, PKMN_ITEM_NONE, 0);
    check_item_at_index(&key_item_pocket, 1, PKMN_ITEM_NONE, 0);

    check_item_at_index(&ball_pocket, 0, PKMN_ITEM_NONE, 0);
    check_item_at_index(&ball_pocket, 1, PKMN_ITEM_NONE, 0);

    check_item_at_index(&tm_pocket, 0, PKMN_ITEM_NONE, 0);
    check_item_at_index(&tm_pocket, 1, PKMN_ITEM_NONE, 0);
    check_item_at_index(&tm_pocket, 2, PKMN_ITEM_NONE, 0);

    check_item_at_index(&berry_pocket, 0, PKMN_ITEM_NONE, 0);
    check_item_at_index(&berry_pocket, 1, PKMN_ITEM_NONE, 0);

    check_item_at_index(&cologne_pocket, 0, PKMN_ITEM_NONE, 0);

    if(is_game_colosseum(game))
    {
        check_item_at_index(&cologne_pocket, 1, PKMN_ITEM_NONE, 0);
    }
    else
    {
        check_item_at_index(&cologne_pocket, 1, PKMN_ITEM_NONE, 0);

        check_item_at_index(&battle_cd_pocket, 0, PKMN_ITEM_NONE, 0);
        check_item_at_index(&battle_cd_pocket, 1, PKMN_ITEM_NONE, 0);
    }

    // Make sure we can't add items from later generations.
    test_item_bag_invalid_items(
        &item_bag,
        wrong_game_all_pocket_items,
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
