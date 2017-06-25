/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_items_test.h"

#include <pkmn-c/database/lists.h>

// For pksav_gba_game_t
#include <pksav/gba.h>

#include <unity.h>

#include <string.h>

#define STRBUFFER_LEN 1024

static const char* all_pocket_item_names[] = {
    "Potion",
    "Mach Bike",
    "Great Ball",
    "TM01",
    "Aspear Berry",
    "Wailmer Pail",
    "Master Ball",
    "HM04"
};
static const char* wrong_game_all_pocket_items[] = {
    "Pink Bow", "Black Sludge",
    "Ein File S", "Gonzap's Key",
    "GS Ball", "Poffin Items",
    "TM51",
    "Berry", "Occa Berry"
};

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

void gba_item_pocket_test(
    pkmn_item_list_handle_t item_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];

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
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_name(
            item_pocket,
            strbuffer,
            sizeof(strbuffer)
        )
    );
    TEST_ASSERT_EQUAL_STRING(expected_name, strbuffer);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_game(
            item_pocket,
            strbuffer,
            sizeof(strbuffer)
        )
    );
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

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

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_capacity(
            item_pocket,
            &capacity
        )
    );
    TEST_ASSERT_EQUAL(expected_capacity, capacity);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            item_pocket,
            &num_items
        )
    );
    TEST_ASSERT_EQUAL(0, num_items);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        item_pocket,
        "Potion"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_items[] = {
        "Bicycle",
        "Master Ball",
        "HM01",
        "Razz Berry"
    };
    test_item_list_invalid_items(
        item_pocket,
        wrong_pocket_items,
        4
    );

    // Make sure we can't add items from other generations.
    const char* wrong_generation_items[] = {
        "Pink Bow",
        "Black Sludge",
        "Binding Band",
        "Beedrillite"
    };
    test_item_list_invalid_items(
        item_pocket,
        wrong_generation_items,
        4
    );

    // Make sure we can't add items from Gamecube games.
    const char* gcn_items[] = {
        "Time Flute",
        "Poké Snack",
    };
    test_item_list_invalid_items(
        item_pocket,
        gcn_items,
        2
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

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_valid_items(
            item_pocket,
            &valid_items
        )
    );
    TEST_ASSERT(valid_items.length > 0);
    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

void gba_key_item_pocket_test(
    pkmn_item_list_handle_t key_item_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];

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
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_name(
            key_item_pocket,
            strbuffer,
            sizeof(strbuffer)
        )
    );
    TEST_ASSERT_EQUAL_STRING(expected_name, strbuffer);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_game(
            key_item_pocket,
            strbuffer,
            sizeof(strbuffer)
        )
    );
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    switch(gba_game) {
        case PKSAV_GBA_RS:
            expected_capacity = 20;
            break;

        default:
            expected_capacity = 30;
            break;
    }

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_capacity(
            key_item_pocket,
            &capacity
        )
    );
    TEST_ASSERT_EQUAL(expected_capacity, capacity);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            key_item_pocket,
            &num_items
        )
    );
    TEST_ASSERT_EQUAL(0, num_items);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(key_item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        key_item_pocket,
        "Basement Key"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_items[] = {
        "Potion",
        "Master Ball",
        "HM01",
        "Razz Berry"
    };
    test_item_list_invalid_items(
        key_item_pocket,
        wrong_pocket_items,
        4
    );

    // Make sure we can't add items from other generations.
    const char* wrong_generation_items[] = {
        "GS Ball",
        "Poffin Items",
        "DNA Splicers",
        "Aqua Suit"
    };
    test_item_list_invalid_items(
        key_item_pocket,
        wrong_generation_items,
        4
    );

    // Make sure we can't add items from incompatible Generation III games.
    const char* gcn_items[] = {
        "Ein File S",
        "Powerup Part",
        "Gonzap's Key",
        "Krane Memo 1"
    };
    test_item_list_invalid_items(
        key_item_pocket,
        gcn_items,
        4
    );
    if(strcmp(game, "Ruby") == 0 ||
       strcmp(game, "Sapphire") == 0)
    {
        const char* frlg_items[] = {
            "Helix Fossil",
            "Tea",
            "Ruby"
        };
        test_item_list_invalid_items(
            key_item_pocket,
            frlg_items,
            3
        );
    }
    if(strcmp(game, "Emerald") != 0) {
        const char* emerald_items[] = {
            "Magma Emblem",
            "Old Sea Map"
        };
        test_item_list_invalid_items(
            key_item_pocket,
            emerald_items,
            2
        );
    }

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

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_valid_items(
            key_item_pocket,
            &valid_items
        )
    );
    TEST_ASSERT(valid_items.length > 0);
    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

void gba_ball_pocket_test(
    pkmn_item_list_handle_t ball_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];

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
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_name(
            ball_pocket,
            strbuffer,
            sizeof(strbuffer)
        )
    );
    TEST_ASSERT_EQUAL_STRING(expected_name, strbuffer);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_game(
            ball_pocket,
            strbuffer,
            sizeof(strbuffer)
        )
    );
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    switch(gba_game) {
        case PKSAV_GBA_FRLG:
            expected_capacity = 13;
            break;

        default:
            expected_capacity = 16;
            break;
    }

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_capacity(
            ball_pocket,
            &capacity
        )
    );
    TEST_ASSERT_EQUAL(expected_capacity, capacity);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            ball_pocket,
            &num_items
        )
    );
    TEST_ASSERT_EQUAL(0, num_items);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(ball_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        ball_pocket,
        "Master Ball"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_items[] = {
        "Potion",
        "Bicycle",
        "HM01",
        "Razz Berry"
    };
    test_item_list_invalid_items(
        ball_pocket,
        wrong_pocket_items,
        4
    );

    // Make sure we can't add items from other generations.
    const char* wrong_generation_items[] = {
        "Moon Ball",
        "Heal Ball",
        "Dream Ball"
    };
    test_item_list_invalid_items(
        ball_pocket,
        wrong_generation_items,
        3
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

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_valid_items(
            ball_pocket,
            &valid_items
        )
    );
    TEST_ASSERT(valid_items.length > 0);
    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

void gba_tmhm_pocket_test(
    pkmn_item_list_handle_t tmhm_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];

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
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_name(
            tmhm_pocket,
            strbuffer,
            sizeof(strbuffer)
        )
    );
    TEST_ASSERT_EQUAL_STRING(expected_name, strbuffer);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_game(
            tmhm_pocket,
            strbuffer,
            sizeof(strbuffer)
        )
    );
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    switch(gba_game) {
        case PKSAV_GBA_FRLG:
            expected_capacity = 58;
            break;

        default:
            expected_capacity = 64;
            break;
    }

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_capacity(
            tmhm_pocket,
            &capacity
        )
    );
    TEST_ASSERT_EQUAL(expected_capacity, capacity);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            tmhm_pocket,
            &num_items
        )
    );
    TEST_ASSERT_EQUAL(0, num_items);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(tmhm_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        tmhm_pocket,
        "TM01"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_items[] = {
        "Potion",
        "Bicycle",
        "Great Ball",
        "Razz Berry"
    };
    test_item_list_invalid_items(
        tmhm_pocket,
        wrong_pocket_items,
        4
    );

    // Make sure we can't add items from other generations.
    const char* wrong_generation_items[] = {
        "TM51"
    };
    test_item_list_invalid_items(
        tmhm_pocket,
        wrong_generation_items,
        1
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

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_valid_items(
            tmhm_pocket,
            &valid_items
        )
    );
    TEST_ASSERT(valid_items.length > 0);
    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

void gba_berry_pocket_test(
    pkmn_item_list_handle_t berry_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];

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
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_name(
            berry_pocket,
            strbuffer,
            sizeof(strbuffer)
        )
    );
    TEST_ASSERT_EQUAL_STRING(expected_name, strbuffer);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_game(
            berry_pocket,
            strbuffer,
            sizeof(strbuffer)
        )
    );
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    switch(gba_game) {
        case PKSAV_GBA_FRLG:
            expected_capacity = 43;
            break;

        default:
            expected_capacity = 46;
            break;
    }

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_capacity(
            berry_pocket,
            &capacity
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(expected_capacity, capacity);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            berry_pocket,
            &num_items
        )
    );
    TEST_ASSERT_EQUAL(0, num_items);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(berry_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        berry_pocket,
        "Razz Berry"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_items[] = {
        "Potion",
        "Bicycle",
        "Great Ball",
        "HM02"
    };
    test_item_list_invalid_items(
        berry_pocket,
        wrong_pocket_items,
        4
    );

    // Make sure we can't add items from other generations.
    const char* wrong_generation_items[] = {
        "Berry",
        "Occa Berry",
        "Roseli Berry"
    };
    test_item_list_invalid_items(
        berry_pocket,
        wrong_generation_items,
        3
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

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_valid_items(
            berry_pocket,
            &valid_items
        )
    );
    TEST_ASSERT(valid_items.length > 0);
    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

void gba_pc_test(
    pkmn_item_list_handle_t pc,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];

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
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_name(
            pc,
            strbuffer,
            sizeof(strbuffer)
        )
    );
    TEST_ASSERT_EQUAL_STRING(expected_name, strbuffer);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_game(
            pc,
            strbuffer,
            sizeof(strbuffer)
        )
    );
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_capacity(
            pc,
            &capacity
        )
    );
    TEST_ASSERT_EQUAL(50, capacity);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            pc,
            &num_items
        )
    );
    TEST_ASSERT_EQUAL(0, num_items);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(pc);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        pc,
        "Potion"
    );

    // Make sure we can't add items from other generations or incompatible Generation III games.
    test_item_list_invalid_items(
        pc,
        wrong_game_all_pocket_items,
        9
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    test_item_list_add_remove(
        pc,
        all_pocket_item_names,
        8
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_valid_items(
            pc,
            &valid_items
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_database_item_list(
            game,
            &full_item_list
        )
    );
    TEST_ASSERT_EQUAL(valid_items.length, full_item_list.length);

    pkmn_string_list_free(&full_item_list);
    TEST_ASSERT_NULL(full_item_list.strings);
    TEST_ASSERT_EQUAL(0, full_item_list.length);

    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, full_item_list.length);
}

void gba_item_bag_test(
    pkmn_item_bag_handle_t bag,
    const char* game
) {
    pkmn_string_list_t pocket_names = {
        .strings = NULL,
        .length = 0
    };

    char strbuffer[STRBUFFER_LEN] = "";

    pksav_gba_game_t gba_game = PKSAV_GBA_RS;

    // Get game type
    game_name_to_enum(
        game,
        &gba_game
    );

    // Check unchanging and initial values.
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_get_game(
            bag,
            strbuffer,
            sizeof(strbuffer)
        )
    );
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_get_pocket_names(
            bag,
            &pocket_names
        )
    );
    TEST_ASSERT_EQUAL(5, pocket_names.length);

    pkmn_string_list_free(&pocket_names);
    TEST_ASSERT_NULL(pocket_names.strings);
    TEST_ASSERT_EQUAL(0, pocket_names.length);

    // Test pockets.
    pkmn_item_list_handle_t item_list_handle = NULL;
    pkmn_item_list_handle_t key_item_list_handle = NULL;
    pkmn_item_list_handle_t ball_list_handle = NULL;
    pkmn_item_list_handle_t tmhm_list_handle = NULL;
    pkmn_item_list_handle_t berry_list_handle = NULL;
    int num_items = 0;

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_get_pocket(
            bag,
            "Items",
            &item_list_handle
        )
    );
    gba_item_pocket_test(
        item_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            item_list_handle,
            &num_items
        )
    );
    TEST_ASSERT_EQUAL(0, num_items);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_get_pocket(
            bag,
            "Key Items",
            &key_item_list_handle
        )
    );
    gba_key_item_pocket_test(
        key_item_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            key_item_list_handle,
            &num_items
        )
    );
    TEST_ASSERT_EQUAL(0, num_items);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_get_pocket(
            bag,
            "Poké Balls",
            &ball_list_handle
        )
    );
    gba_ball_pocket_test(
        ball_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            ball_list_handle,
            &num_items
        )
    );
    TEST_ASSERT_EQUAL(0, num_items);

    const char* expected_tmhm_name = (gba_game == PKSAV_GBA_FRLG) ? "TM Case"
                                                                  : "TMs & HMs";
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_get_pocket(
            bag,
            expected_tmhm_name,
            &tmhm_list_handle
        )
    );
    gba_tmhm_pocket_test(
        tmhm_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            tmhm_list_handle,
            &num_items
        )
    );
    TEST_ASSERT_EQUAL(0, num_items);

    const char* expected_berry_name = (gba_game == PKSAV_GBA_FRLG) ? "Berry Pouch"
                                                                   : "Berries";
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_get_pocket(
            bag,
            expected_berry_name,
            &berry_list_handle
        )
    );
    gba_berry_pocket_test(
        berry_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            berry_list_handle,
            &num_items
        )
    );
    TEST_ASSERT_EQUAL(0, num_items);

    /*
     * Make sure adding items through the bag adds to the proper pockets.
     * To avoid unnecessary allocations, the C tests will just check the
     * first item slot.
     */
    pkmn_item_slot_t item_slot = {
        .item = {0},
        .amount = 0
    };

    for(int i = 0; i < 8; ++i) {
        TEST_ASSERT_EQUAL(
            PKMN_ERROR_NONE,
            pkmn_item_bag_add(
                bag,
                all_pocket_item_names[i],
                5
            )
        );
    }

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_at(
            item_list_handle,
            0,
            &item_slot
        )
    );
    TEST_ASSERT_EQUAL_STRING("Potion", item_slot.item);
    TEST_ASSERT_EQUAL(5, item_slot.amount);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_at(
            key_item_list_handle,
            0,
            &item_slot
        )
    );
    TEST_ASSERT_EQUAL_STRING("Mach Bike", item_slot.item);
    TEST_ASSERT_EQUAL(5, item_slot.amount);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_at(
            ball_list_handle,
            0,
            &item_slot
        )
    );
    TEST_ASSERT_EQUAL_STRING("Great Ball", item_slot.item);
    TEST_ASSERT_EQUAL(5, item_slot.amount);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_at(
            tmhm_list_handle,
            0,
            &item_slot
        )
    );
    TEST_ASSERT_EQUAL_STRING("TM01", item_slot.item);
    TEST_ASSERT_EQUAL(5, item_slot.amount);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_at(
            berry_list_handle,
            0,
            &item_slot
        )
    );
    TEST_ASSERT_EQUAL_STRING("Aspear Berry", item_slot.item);
    TEST_ASSERT_EQUAL(5, item_slot.amount);

    /*
     * Make sure adding items through the bag adds to the proper pockets.
     * To avoid unnecessary allocations, the C tests will just check the
     * first item slot.
     */
    for(int i = 0; i < 8; ++i) {
        TEST_ASSERT_EQUAL(
            PKMN_ERROR_NONE,
            pkmn_item_bag_remove(
                bag,
                all_pocket_item_names[i],
                5
            )
        );
    }

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_at(
            item_list_handle,
            0,
            &item_slot
        )
    );
    TEST_ASSERT_EQUAL_STRING("None", item_slot.item);
    TEST_ASSERT_EQUAL(0, item_slot.amount);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_at(
            key_item_list_handle,
            0,
            &item_slot
        )
    );
    TEST_ASSERT_EQUAL_STRING("None", item_slot.item);
    TEST_ASSERT_EQUAL(0, item_slot.amount);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_at(
            ball_list_handle,
            0,
            &item_slot
        )
    );
    TEST_ASSERT_EQUAL_STRING("None", item_slot.item);
    TEST_ASSERT_EQUAL(0, item_slot.amount);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_at(
            tmhm_list_handle,
            0,
            &item_slot
        )
    );
    TEST_ASSERT_EQUAL_STRING("None", item_slot.item);
    TEST_ASSERT_EQUAL(0, item_slot.amount);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_at(
            berry_list_handle,
            0,
            &item_slot
        )
    );
    TEST_ASSERT_EQUAL_STRING("None", item_slot.item);
    TEST_ASSERT_EQUAL(0, item_slot.amount);

    // Make sure we can't add items from later generations.
    test_item_bag_invalid_items(
        bag,
        wrong_game_all_pocket_items,
        9
    );

    // Cleanup
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(&item_list_handle)
    );
    TEST_ASSERT_NULL(item_list_handle);
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(&key_item_list_handle)
    );
    TEST_ASSERT_NULL(key_item_list_handle);
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(&ball_list_handle)
    );
    TEST_ASSERT_NULL(ball_list_handle);
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(&tmhm_list_handle)
    );
    TEST_ASSERT_NULL(tmhm_list_handle);
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(&berry_list_handle)
    );
    TEST_ASSERT_NULL(berry_list_handle);
}
