/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "items_tests_common.h"

#include <pkmntest-c/gba_items_tests.h>

#include <pkmn-c/database/lists.h>

// For pksav_gba_game_t
#include <pksav/gba.h>

#include <unity.h>

#include <string.h>

#define STRBUFFER_LEN 1024

static void game_name_to_enum(
    const char* game,
    pksav_gba_game_t* result
) {
    TEST_ASSERT_NOT_NULL(game);
    TEST_ASSERT_NOT_NULL(result);

    if(strcmp(game, "Ruby") == 0 ||
       strcmp(game, "Sapphire") == 0)
    {
        *result = PKSAV_GBA_RS;
    } else if(strcmp(game, "Emerald") == 0) {
        *result = PKSAV_GBA_EMERALD;
    } else if(strcmp(game, "FireRed") == 0 ||
              strcmp(game, "LeafGreen") == 0)
    {
        *result = PKSAV_GBA_FRLG;
    } else {
        TEST_ASSERT(false);
    }
}

void pkmntest_gba_item_pocket_test(
    pkmn_item_list_handle_t item_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];
    size_t actual_strlen = 0;

    int capacity = 0;
    int expected_capacity = 0;
    int num_items = 0;
    pkmn_string_list_t valid_items = {
        .strings = NULL,
        .length = 0
    };
    pksav_gba_game_t gba_game = PKSAV_GBA_RS;

    // Get game type
    game_name_to_enum(
        game,
        &gba_game
    );

    // Check unchanging and initial values.
    const char* expected_name = "Items";
    TEST_ASSERT_EQUAL(pkmn_item_list_get_name(
                          item_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(expected_name)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_game(
                          item_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(game)+1);

    switch(gba_game) {
        case PKSAV_GBA_RS:
            expected_capacity = 20;
            break;

        case PKSAV_GBA_EMERALD:
            expected_capacity = 30;
            break;

        case PKSAV_GBA_FRLG:
            expected_capacity = 42;
            break;

        default:
            TEST_ASSERT(false);
    }

    TEST_ASSERT_EQUAL(pkmn_item_list_get_capacity(
                          item_pocket,
                          &capacity
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(capacity, expected_capacity);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          item_pocket,
                          &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 0);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        item_pocket,
        "Potion"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_items[] = {
        "Bicycle",
        "Master Ball",
        "HM01",
        "Razz Berry"
    };
    test_item_list_items_from_wrong_pocket(
        item_pocket,
        wrong_items,
        4
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    const char* item_names[] = {
        "Potion",
        "Orange Mail",
        "Lava Cookie",
        "Stardust",
        "Shadow Mail",
        "Pink Scarf",
        "Antidote",
        "Green Shard"
    };
    test_item_list_add_remove(
        item_pocket,
        item_names,
        8
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_get_valid_items(
                          item_pocket,
                          &valid_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT(valid_items.length > 0);
    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(valid_items.length, 0);
}

void pkmntest_gba_key_item_pocket_test(
    pkmn_item_list_handle_t key_item_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];
    size_t actual_strlen = 0;

    int capacity = 0;
    int expected_capacity = 0;
    int num_items = 0;
    pkmn_string_list_t valid_items = {
        .strings = NULL,
        .length = 0
    };
    pksav_gba_game_t gba_game = PKSAV_GBA_RS;

    // Get game type
    game_name_to_enum(
        game,
        &gba_game
    );

    // Check unchanging and initial values.
    const char* expected_name = "Key Items";
    TEST_ASSERT_EQUAL(pkmn_item_list_get_name(
                          key_item_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(expected_name)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_game(
                          key_item_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(game)+1);

    switch(gba_game) {
        case PKSAV_GBA_RS:
            expected_capacity = 20;
            break;

        default:
            expected_capacity = 30;
            break;
    }

    TEST_ASSERT_EQUAL(pkmn_item_list_get_capacity(
                          key_item_pocket,
                          &capacity
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(capacity, expected_capacity);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          key_item_pocket,
                          &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 0);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(key_item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        key_item_pocket,
        "Basement Key"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_items[] = {
        "Potion",
        "Master Ball",
        "HM01",
        "Razz Berry"
    };
    test_item_list_items_from_wrong_pocket(
        key_item_pocket,
        wrong_items,
        4
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    const char* item_names[] = {
        "Wailmer Pail",
        "Basement Key",
        "Meteorite",
        "Old Rod",
        "Red Orb",
        "Root Fossil",
        "Contest Pass",
        "Eon Ticket"
    };
    test_item_list_add_remove(
        key_item_pocket,
        item_names,
        8
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_get_valid_items(
                          key_item_pocket,
                          &valid_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT(valid_items.length > 0);
    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(valid_items.length, 0);
}

void pkmntest_gba_ball_pocket_test(
    pkmn_item_list_handle_t ball_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];
    size_t actual_strlen = 0;

    int capacity = 0;
    int expected_capacity = 0;
    int num_items = 0;
    pkmn_string_list_t valid_items = {
        .strings = NULL,
        .length = 0
    };
    pksav_gba_game_t gba_game = PKSAV_GBA_RS;

    // Get game type
    game_name_to_enum(
        game,
        &gba_game
    );

    // Check unchanging and initial values.
    const char* expected_name = "Poké Balls";
    TEST_ASSERT_EQUAL(pkmn_item_list_get_name(
                          ball_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(expected_name)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_game(
                          ball_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(game)+1);

    switch(gba_game) {
        case PKSAV_GBA_FRLG:
            expected_capacity = 13;
            break;

        default:
            expected_capacity = 16;
            break;
    }

    TEST_ASSERT_EQUAL(pkmn_item_list_get_capacity(
                          ball_pocket,
                          &capacity
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(capacity, expected_capacity);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          ball_pocket,
                          &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 0);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(ball_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        ball_pocket,
        "Master Ball"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_items[] = {
        "Potion",
        "Bicycle",
        "HM01",
        "Razz Berry"
    };
    test_item_list_items_from_wrong_pocket(
        ball_pocket,
        wrong_items,
        4
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    const char* item_names[] = {
        "Master Ball",
        "Ultra Ball",
        "Great Ball",
        "Poké Ball",
        "Safari Ball",
        "Net Ball",
        "Dive Ball",
        "Nest Ball",
    };
    test_item_list_add_remove(
        ball_pocket,
        item_names,
        8
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_get_valid_items(
                          ball_pocket,
                          &valid_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT(valid_items.length > 0);
    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(valid_items.length, 0);
}

void pkmntest_gba_tmhm_pocket_test(
    pkmn_item_list_handle_t tmhm_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];
    size_t actual_strlen = 0;

    int capacity = 0;
    int expected_capacity = 0;
    int num_items = 0;
    pkmn_string_list_t valid_items = {
        .strings = NULL,
        .length = 0
    };
    pksav_gba_game_t gba_game = PKSAV_GBA_RS;

    // Get game type
    game_name_to_enum(
        game,
        &gba_game
    );

    // Check unchanging and initial values.
    const char* expected_name = (gba_game == PKSAV_GBA_FRLG) ? "TM Case"
                                                             : "TMs & HMs";
    TEST_ASSERT_EQUAL(pkmn_item_list_get_name(
                          tmhm_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(expected_name)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_game(
                          tmhm_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(game)+1);

    switch(gba_game) {
        case PKSAV_GBA_FRLG:
            expected_capacity = 58;
            break;

        default:
            expected_capacity = 64;
            break;
    }

    TEST_ASSERT_EQUAL(pkmn_item_list_get_capacity(
                          tmhm_pocket,
                          &capacity
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(capacity, expected_capacity);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          tmhm_pocket,
                          &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 0);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(tmhm_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        tmhm_pocket,
        "TM01"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_items[] = {
        "Potion",
        "Bicycle",
        "Great Ball",
        "Razz Berry"
    };
    test_item_list_items_from_wrong_pocket(
        tmhm_pocket,
        wrong_items,
        4
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    const char* item_names[] = {
        "TM01",
        "HM01",
        "TM02",
        "HM02",
        "TM03",
        "HM03",
        "TM04",
        "HM04",
    };
    test_item_list_add_remove(
        tmhm_pocket,
        item_names,
        8
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_get_valid_items(
                          tmhm_pocket,
                          &valid_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT(valid_items.length > 0);
    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(valid_items.length, 0);
}

void pkmntest_gba_berry_pocket_test(
    pkmn_item_list_handle_t berry_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];
    size_t actual_strlen = 0;

    int capacity = 0;
    int expected_capacity = 0;
    int num_items = 0;
    pkmn_string_list_t valid_items = {
        .strings = NULL,
        .length = 0
    };
    pksav_gba_game_t gba_game = PKSAV_GBA_RS;

    // Get game type
    game_name_to_enum(
        game,
        &gba_game
    );

    // Check unchanging and initial values.
    const char* expected_name = (gba_game == PKSAV_GBA_FRLG) ? "Berry Pouch"
                                                             : "Berries";
    TEST_ASSERT_EQUAL(pkmn_item_list_get_name(
                          berry_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(expected_name)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_game(
                          berry_pocket,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(game)+1);

    switch(gba_game) {
        case PKSAV_GBA_FRLG:
            expected_capacity = 43;
            break;

        default:
            expected_capacity = 46;
            break;
    }

    TEST_ASSERT_EQUAL(pkmn_item_list_get_capacity(
                          berry_pocket,
                          &capacity
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(capacity, expected_capacity);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          berry_pocket,
                          &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 0);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(berry_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        berry_pocket,
        "Razz Berry"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_items[] = {
        "Potion",
        "Bicycle",
        "Great Ball",
        "HM02"
    };
    test_item_list_items_from_wrong_pocket(
        berry_pocket,
        wrong_items,
        4
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    const char* item_names[] = {
        "Cheri Berry",
        "Razz Berry",
        "Lum Berry",
        "Pinap Berry",
        "Aspear Berry",
        "Iapapa Berry",
        "Wiki Berry",
        "Apicot Berry",
    };
    test_item_list_add_remove(
        berry_pocket,
        item_names,
        8
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_get_valid_items(
                          berry_pocket,
                          &valid_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT(valid_items.length > 0);
    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(valid_items.length, 0);
}

void pkmntest_gba_pc_test(
    pkmn_item_list_handle_t pc,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];
    size_t actual_strlen = 0;

    int capacity = 0;
    int num_items = 0;
    pkmn_string_list_t full_item_list = {
        .strings = NULL,
        .length = 0
    };
    pkmn_string_list_t valid_items = {
        .strings = NULL,
        .length = 0
    };

    // Check unchanging and initial values.
    const char* expected_name = "PC";
    TEST_ASSERT_EQUAL(pkmn_item_list_get_name(
                          pc,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(expected_name)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_game(
                          pc,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(game)+1);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_capacity(
                          pc,
                          &capacity
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(capacity, 50);

    TEST_ASSERT_EQUAL(pkmn_item_list_get_num_items(
                          pc,
                          &num_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(num_items, 0);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(pc);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        pc,
        "Potion"
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    const char* item_names[] = {
        "Potion",
        "Mach Bike",
        "Great Ball",
        "TM01",
        "Aspear Berry",
        "Wailmer Pail",
        "Master Ball",
        "HM04"
    };
    test_item_list_add_remove(
        pc,
        item_names,
        8
    );

    TEST_ASSERT_EQUAL(pkmn_item_list_get_valid_items(
                          pc,
                          &valid_items
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pkmn_database_item_list(
                          game,
                          &full_item_list
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(full_item_list.length, valid_items.length);

    pkmn_string_list_free(&full_item_list);
    TEST_ASSERT_NULL(full_item_list.strings);
    TEST_ASSERT_EQUAL(full_item_list.length, 0);

    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(full_item_list.length, 0);
}

void pkmntest_gba_item_bag_test(
    pkmn_item_bag_handle_t bag,
    const char* game
) {
    pkmn_string_list_t pocket_names = {
        .strings = NULL,
        .length = 0
    };

    char strbuffer[STRBUFFER_LEN] = "";
    size_t actual_strlen = 0;

    pksav_gba_game_t gba_game = PKSAV_GBA_RS;

    // Get game type
    game_name_to_enum(
        game,
        &gba_game
    );

    // Check unchanging and initial values.
    TEST_ASSERT_EQUAL(pkmn_item_bag_get_game(
                          bag,
                          strbuffer,
                          STRBUFFER_LEN,
                          &actual_strlen
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(game)+1);

    TEST_ASSERT_EQUAL(pkmn_item_bag_get_pocket_names(
                          bag,
                          &pocket_names
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pocket_names.length, 5);

    pkmn_string_list_free(&pocket_names);
    TEST_ASSERT_NULL(pocket_names.strings);
    TEST_ASSERT_EQUAL(pocket_names.length, 0);

    // Test pockets.
    pkmn_item_list_handle_t item_list_handle = NULL;

    TEST_ASSERT_EQUAL(pkmn_item_bag_get_pocket(
                          bag,
                          "Items",
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    pkmntest_gba_item_pocket_test(
        item_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_list_handle);

    TEST_ASSERT_EQUAL(pkmn_item_bag_get_pocket(
                          bag,
                          "Key Items",
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    pkmntest_gba_key_item_pocket_test(
        item_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_list_handle);

    TEST_ASSERT_EQUAL(pkmn_item_bag_get_pocket(
                          bag,
                          "Poké Balls",
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    pkmntest_gba_ball_pocket_test(
        item_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_list_handle);

    const char* expected_tmhm_name = (gba_game == PKSAV_GBA_FRLG) ? "TM Case"
                                                                  : "TMs & HMs";
    TEST_ASSERT_EQUAL(pkmn_item_bag_get_pocket(
                          bag,
                          expected_tmhm_name,
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    pkmntest_gba_tmhm_pocket_test(
        item_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_list_handle);

    const char* expected_berry_name = (gba_game == PKSAV_GBA_FRLG) ? "Berry Pouch"
                                                                   : "Berries";
    TEST_ASSERT_EQUAL(pkmn_item_bag_get_pocket(
                          bag,
                          expected_berry_name,
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    pkmntest_gba_berry_pocket_test(
        item_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(pkmn_item_list_free(
                          &item_list_handle
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(item_list_handle);
}
