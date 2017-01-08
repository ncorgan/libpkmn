/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "items_tests_common.h"

#include <pkmntest-c/gen2_items_tests.h>

#include <pkmn-c/database/lists.h>

#include <unity.h>

#include <string.h>

#define STRBUFFER_LEN 1024

static const char* all_pocket_item_names[] = {
    "Potion",
    "Bicycle",
    "Great Ball",
    "TM28",
    "Berry",
    "SquirtBottle",
    "Friend Ball",
    "HM01"
};
static const char* crystal_items[] = {
    "Clear Bell",
    "GS Ball",
    "Blue Card",
    "Egg Ticket"
};
static const char* wrong_generation_all_pocket_items[] = {
    "Black Sludge",
    "Poffin Items",
    "TM51",
    "Occa Berry"
};

void pkmntest_gen2_item_pocket_test(
    pkmn_item_list_handle_t item_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];

    int capacity = 0;
    int num_items = 0;
    pkmn_string_list_t valid_items = {
        .strings = NULL,
        .length = 0
    };

    // Check unchanging and initial values.
    const char* expected_name = "Items";
    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_name(
            item_pocket,
            strbuffer,
            sizeof(strbuffer)
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_game(
            item_pocket,
            strbuffer,
            sizeof(strbuffer)
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_capacity(
            item_pocket,
            &capacity
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(capacity, 20);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_num_items(
            item_pocket,
            &num_items
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(num_items, 0);

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
        "HM01"
    };
    test_item_list_invalid_items(
        item_pocket,
        wrong_pocket_items,
        3
    );

    // Make sure we can't add items from later generations.
    const char* wrong_generation_items[] = {
        "Black Flute",
        "Black Sludge",
        "Binding Sand",
        "Beedrillite"
    };
    test_item_list_invalid_items(
        item_pocket,
        wrong_generation_items,
        4
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const char* item_names[] = {
        "Potion",
        "HP Up",
        "Wht Apricorn",
        "Lucky Egg",
        "Flower Mail",
        "Burn Heal",
        "PSNCureBerry",
        "Stick"
    };
    test_item_list_add_remove(
        item_pocket,
        item_names,
        8
    );

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_valid_items(
            item_pocket,
            &valid_items
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT(valid_items.length > 0);
    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(valid_items.length, 0);
}

void pkmntest_gen2_key_item_pocket_test(
    pkmn_item_list_handle_t key_item_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];

    int capacity = 0;
    int num_items = 0;
    pkmn_string_list_t valid_items = {
        .strings = NULL,
        .length = 0
    };

    // Check unchanging and initial values.
    const char* expected_name = "KeyItems";
    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_name(
            key_item_pocket,
            strbuffer,
            sizeof(strbuffer)
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_game(
            key_item_pocket,
            strbuffer,
            sizeof(strbuffer)
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_capacity(
            key_item_pocket,
            &capacity
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(capacity, 26);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_num_items(
            key_item_pocket,
            &num_items
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(num_items, 0);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(key_item_pocket);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        key_item_pocket,
        "Bicycle"
    );

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_items[] = {
        "Potion",
        "Master Ball",
        "HM01"
    };
    test_item_list_invalid_items(
        key_item_pocket,
        wrong_pocket_items,
        3
    );

    // Make sure we can't add items from later generations.
    const char* wrong_generation_items[] = {
        "Mach Bike",
        "Jade Orb",
        "Light Stone",
        "Aqua Suit"
    };
    test_item_list_invalid_items(
        key_item_pocket,
        wrong_generation_items,
        4
    );

    // Crystal-specific items.
    if(strcmp(game, "Crystal") == 0) {
        for(int i = 0; i < 4; ++i) {
            TEST_ASSERT_EQUAL(
                pkmn_item_list_add(
                    key_item_pocket,
                    crystal_items[i],
                    1
                ),
                PKMN_ERROR_NONE
            );
            TEST_ASSERT_EQUAL(
                pkmn_item_list_remove(
                    key_item_pocket,
                    crystal_items[i],
                    1
                ),
                PKMN_ERROR_NONE
            );
        }
    } else {
        test_item_list_invalid_items(
            key_item_pocket,
            crystal_items,
            3
        );
    }

    // Start adding and removing stuff, and make sure the numbers are accurate.
    const char* item_names[] = {
        "Bicycle",
        "Basement Key",
        "SecretPotion",
        "Mystery Egg",
        "Silver Wing",
        "Lost Item",
        "SquirtBottle",
        "Rainbow Wing"
    };
    test_item_list_add_remove(
        key_item_pocket,
        item_names,
        8
    );

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_valid_items(
            key_item_pocket,
            &valid_items
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT(valid_items.length > 0);
    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(valid_items.length, 0);
}

void pkmntest_gen2_ball_pocket_test(
    pkmn_item_list_handle_t ball_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];

    int capacity = 0;
    int num_items = 0;
    pkmn_string_list_t valid_items = {
        .strings = NULL,
        .length = 0
    };

    // Check unchanging and initial values.
    const char* expected_name = "Balls";
    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_name(
            ball_pocket,
            strbuffer,
            sizeof(strbuffer)
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_game(
            ball_pocket,
            strbuffer,
            sizeof(strbuffer)
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_capacity(
            ball_pocket,
            &capacity
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(capacity, 12);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_num_items(
            ball_pocket,
            &num_items
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(num_items, 0);

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
        "HM01"
    };
    test_item_list_invalid_items(
        ball_pocket,
        wrong_pocket_items,
        3
    );

    // Make sure we can't add items from later generations.
    const char* wrong_generation_items[] = {
        "Premier Ball",
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
        "Great Ball",
        "Poké Ball",
        "Park Ball",
        "Fast Ball",
        "Master Ball",
        "Friend Ball",
        "Love Ball",
        "Level Ball"
    };
    test_item_list_add_remove(
        ball_pocket,
        item_names,
        8
    );

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_valid_items(
            ball_pocket,
            &valid_items
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT(valid_items.length > 0);
    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(valid_items.length, 0);
}

void pkmntest_gen2_tmhm_pocket_test(
    pkmn_item_list_handle_t tmhm_pocket,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN];

    int capacity = 0;
    int num_items = 0;
    pkmn_item_slots_t item_slots = {
        .item_slots = NULL,
        .length = 0
    };
    pkmn_string_list_t valid_items = {
        .strings = NULL,
        .length = 0
    };

    // Check unchanging and initial values.
    const char* expected_name = "TM/HM";
    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_name(
            tmhm_pocket,
            strbuffer,
            sizeof(strbuffer)
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_game(
            tmhm_pocket,
            strbuffer,
            sizeof(strbuffer)
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_capacity(
            tmhm_pocket,
            &capacity
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(capacity, 57);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_num_items(
            tmhm_pocket,
            &num_items
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(num_items, 0);

    // Make sure item slots start as correctly empty.
    TEST_ASSERT_EQUAL(pkmn_item_list_as_array(
                          tmhm_pocket,
                          &item_slots
                      ), PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(item_slots.length, 57);
    for(int i = 1; i <= 50; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "TM%02d", i);
        TEST_ASSERT_EQUAL_STRING(item_slots.item_slots[i-1].item, name);
        TEST_ASSERT_EQUAL(item_slots.item_slots[i-1].amount, 0);
    }
    for(int i = 1; i <= 7; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "HM%02d", i);
        TEST_ASSERT_EQUAL_STRING(item_slots.item_slots[50+i-1].item, name);
        TEST_ASSERT_EQUAL(item_slots.item_slots[50+i-1].amount, 0);
    }
    pkmn_item_slots_free(&item_slots);
    TEST_ASSERT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL(item_slots.length, 0);

    // Make sure we can't add items from other pockets.
    const char* wrong_pocket_items[] = {
        "Potion",
        "Master Ball",
        "Bicycle"
    };
    test_item_list_invalid_items(
        tmhm_pocket,
        wrong_pocket_items,
        3
    );

    // Make sure we can't add items from later generations.
    const char* wrong_generation_items[] = {
        "TM51"
    };
    test_item_list_invalid_items(
        tmhm_pocket,
        wrong_generation_items,
        1
    );

    /*
     * Start adding and removing stuff, and make sure the numbers are accurate.
     *
     * While this is supposed to match the other pkmntest libraries, there are
     * so many allocations done with string lists that we'll just do it after
     * each set of commands.
     */

    for(int i = 1; i <= 50; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "TM%02d", i);
        TEST_ASSERT_EQUAL(
            pkmn_item_list_add(
                tmhm_pocket,
                name,
                50
            ),
            PKMN_ERROR_NONE
        );
        TEST_ASSERT_EQUAL(
            pkmn_item_list_get_num_items(
                tmhm_pocket,
                &num_items
            ),
            PKMN_ERROR_NONE
        );
        TEST_ASSERT_EQUAL(num_items, i);
    }
    TEST_ASSERT_EQUAL(
        pkmn_item_list_as_array(
            tmhm_pocket,
            &item_slots
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(item_slots.length, 57);
    for(int i = 1; i <= 50; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "TM%02d", i);

        TEST_ASSERT_EQUAL_STRING(item_slots.item_slots[i-1].item, name);
        TEST_ASSERT_EQUAL(item_slots.item_slots[i-1].amount, 50);
    }
    pkmn_item_slots_free(&item_slots);
    TEST_ASSERT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL(item_slots.length, 0);

    for(int i = 50; i >= 1; --i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "TM%02d", i);
        TEST_ASSERT_EQUAL(
            pkmn_item_list_remove(
                tmhm_pocket,
                name,
                50
            ),
            PKMN_ERROR_NONE
        );
        TEST_ASSERT_EQUAL(
            pkmn_item_list_get_num_items(
                tmhm_pocket,
                &num_items
            ),
            PKMN_ERROR_NONE
        );
        TEST_ASSERT_EQUAL(num_items, i-1);
    }
    TEST_ASSERT_EQUAL(
        pkmn_item_list_as_array(
            tmhm_pocket,
            &item_slots
        ),
    PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(item_slots.length, 57);
    for(int i = 1; i <= 50; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "TM%02d", i);

        TEST_ASSERT_EQUAL_STRING(item_slots.item_slots[i-1].item, name);
        TEST_ASSERT_EQUAL(item_slots.item_slots[i-1].amount, 0);
    }
    pkmn_item_slots_free(&item_slots);
    TEST_ASSERT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL(item_slots.length, 0);

    for(int i = 1; i <= 7; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "HM%02d", i);
        TEST_ASSERT_EQUAL(
            pkmn_item_list_add(
                tmhm_pocket,
                name,
                50
            ),
            PKMN_ERROR_NONE
        );
        TEST_ASSERT_EQUAL(
            pkmn_item_list_get_num_items(
                tmhm_pocket,
                &num_items
            ),
            PKMN_ERROR_NONE
        );
        TEST_ASSERT_EQUAL(num_items, i);
    }
    TEST_ASSERT_EQUAL(
        pkmn_item_list_as_array(
            tmhm_pocket,
            &item_slots
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(item_slots.length, 57);
    for(int i = 1; i <= 7; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "HM%02d", i);

        TEST_ASSERT_EQUAL_STRING(item_slots.item_slots[50+i-1].item, name);
        TEST_ASSERT_EQUAL(item_slots.item_slots[50+i-1].amount, 50);
    }
    pkmn_item_slots_free(&item_slots);
    TEST_ASSERT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL(item_slots.length, 0);

    for(int i = 7; i >= 1; --i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "HM%02d", i);
        TEST_ASSERT_EQUAL(
            pkmn_item_list_remove(
                tmhm_pocket,
                name,
                50
            ),
            PKMN_ERROR_NONE
        );
        TEST_ASSERT_EQUAL(
            pkmn_item_list_get_num_items(
                tmhm_pocket,
                &num_items
            ),
            PKMN_ERROR_NONE
        );
        TEST_ASSERT_EQUAL(num_items, i-1);
    }
    TEST_ASSERT_EQUAL(
        pkmn_item_list_as_array(
            tmhm_pocket,
            &item_slots
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(item_slots.length, 57);
    for(int i = 1; i <= 7; ++i) {
        char name[5] = {0};
        snprintf(name, sizeof(name), "HM%02d", i);

        TEST_ASSERT_EQUAL_STRING(item_slots.item_slots[50+i-1].item, name);
        TEST_ASSERT_EQUAL(item_slots.item_slots[50+i-1].amount, 0);
    }
    pkmn_item_slots_free(&item_slots);
    TEST_ASSERT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL(item_slots.length, 0);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_valid_items(
            tmhm_pocket,
            &valid_items
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(valid_items.length, 57);
    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(valid_items.length, 0);
}

void pkmntest_gen2_pc_test(
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
        pkmn_item_list_get_name(
            pc,
            strbuffer,
            sizeof(strbuffer)
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_name);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_game(
            pc,
            strbuffer,
            sizeof(strbuffer)
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_capacity(
            pc,
            &capacity
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(capacity, 50);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_num_items(
            pc,
            &num_items
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(num_items, 0);

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(pc);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        pc,
        "Potion"
    );

    // Crystal-specific items.
    if(strcmp(game, "Crystal") == 0) {
        for(int i = 0; i < 4; ++i) {
            TEST_ASSERT_EQUAL(
                pkmn_item_list_add(
                    pc,
                    crystal_items[i],
                    1
                ),
                PKMN_ERROR_NONE
            );
            TEST_ASSERT_EQUAL(
                pkmn_item_list_remove(
                    pc,
                    crystal_items[i],
                    1
                ),
                PKMN_ERROR_NONE
            );
        }

        TEST_ASSERT_EQUAL(
            pkmn_item_list_get_num_items(
                pc,
                &num_items
            ),
            PKMN_ERROR_NONE
        );
        TEST_ASSERT_EQUAL(num_items, 0);
    } else {
        test_item_list_invalid_items(
            pc,
            crystal_items,
            3
        );
    }

    // Make sure we can't add items from later generations.
    test_item_list_invalid_items(
        pc,
        wrong_generation_all_pocket_items,
        4
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    test_item_list_add_remove(
        pc,
        all_pocket_item_names,
        8
    );

    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_valid_items(
            pc,
            &valid_items
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(
        pkmn_database_item_list(
            game,
            &full_item_list
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(full_item_list.length, valid_items.length);

    pkmn_string_list_free(&full_item_list);
    TEST_ASSERT_NULL(full_item_list.strings);
    TEST_ASSERT_EQUAL(full_item_list.length, 0);

    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(valid_items.length, 0);
}

void pkmntest_gen2_item_bag_test(
    pkmn_item_bag_handle_t bag,
    const char* game
) {
    pkmn_string_list_t pocket_names = {
        .strings = NULL,
        .length = 0
    };

    char strbuffer[STRBUFFER_LEN] = "";

    // Check unchanging and initial values.
    TEST_ASSERT_EQUAL(
        pkmn_item_bag_get_game(
            bag,
            strbuffer,
            sizeof(strbuffer)
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);

    TEST_ASSERT_EQUAL(
        pkmn_item_bag_get_pocket_names(
            bag,
            &pocket_names
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(pocket_names.length, 4);

    pkmn_string_list_free(&pocket_names);
    TEST_ASSERT_NULL(pocket_names.strings);
    TEST_ASSERT_EQUAL(pocket_names.length, 0);

    // Test pockets.
    pkmn_item_list_handle_t item_list_handle = NULL;
    pkmn_item_list_handle_t keyitem_list_handle = NULL;
    pkmn_item_list_handle_t ball_list_handle = NULL;
    pkmn_item_list_handle_t tmhm_list_handle = NULL;
    int num_items = 0;

    TEST_ASSERT_EQUAL(
        pkmn_item_bag_get_pocket(
            bag,
            "Items",
            &item_list_handle
        ),
        PKMN_ERROR_NONE
    );
    pkmntest_gen2_item_pocket_test(
        item_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_num_items(
            item_list_handle,
            &num_items
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(num_items, 0);

    TEST_ASSERT_EQUAL(
        pkmn_item_bag_get_pocket(
            bag,
            "KeyItems",
            &keyitem_list_handle
        ),
        PKMN_ERROR_NONE
    );
    pkmntest_gen2_key_item_pocket_test(
        keyitem_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_num_items(
            keyitem_list_handle,
            &num_items
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(num_items, 0);

    TEST_ASSERT_EQUAL(
        pkmn_item_bag_get_pocket(
            bag,
            "Balls",
            &ball_list_handle
        ),
        PKMN_ERROR_NONE
    );
    pkmntest_gen2_ball_pocket_test(
        ball_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_num_items(
            ball_list_handle,
            &num_items
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(num_items, 0);

    TEST_ASSERT_EQUAL(
        pkmn_item_bag_get_pocket(
            bag,
            "TM/HM",
            &tmhm_list_handle
        ),
        PKMN_ERROR_NONE
    );
    pkmntest_gen2_tmhm_pocket_test(
        tmhm_list_handle,
        game
    );
    TEST_ASSERT_EQUAL(
        pkmn_item_list_get_num_items(
            tmhm_list_handle,
            &num_items
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(num_items, 0);

    /*
     * Make sure adding items through the bag adds to the proper pockets.
     * To avoid unnecessary allocations, the C tests will just check the
     * first item slot.
     */
    pkmn_item_slot_t item_slot = {
        .item = NULL,
        .amount = 0
    };

    for(int i = 0; i < 8; ++i) {
        TEST_ASSERT_EQUAL(
            pkmn_item_bag_add(
                bag,
                all_pocket_item_names[i],
                5
            ),
            PKMN_ERROR_NONE
        );
    }

    TEST_ASSERT_EQUAL(
        pkmn_item_list_at(
            item_list_handle,
            0,
            &item_slot
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(item_slot.item, "Potion");
    TEST_ASSERT_EQUAL(item_slot.amount, 5);
    TEST_ASSERT_EQUAL(
        pkmn_item_slot_free(&item_slot),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_NULL(item_slot.item);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_at(
            keyitem_list_handle,
            0,
            &item_slot
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(item_slot.item, "Bicycle");
    TEST_ASSERT_EQUAL(item_slot.amount, 5);
    TEST_ASSERT_EQUAL(
        pkmn_item_slot_free(&item_slot),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_NULL(item_slot.item);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_at(
            ball_list_handle,
            0,
            &item_slot
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(item_slot.item, "Great Ball");
    TEST_ASSERT_EQUAL(item_slot.amount, 5);
    TEST_ASSERT_EQUAL(
        pkmn_item_slot_free(&item_slot),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_NULL(item_slot.item);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_at(
            tmhm_list_handle,
            27,
            &item_slot
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(item_slot.item, "TM28");
    TEST_ASSERT_EQUAL(item_slot.amount, 5);
    TEST_ASSERT_EQUAL(
        pkmn_item_slot_free(&item_slot),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_NULL(item_slot.item);

    /*
     * Make sure removing items through the bag removes from the proper pockets.
     * To avoid unnecessary allocations, the C tests will just check the
     * first item slot.
     */
    for(int i = 0; i < 8; ++i) {
        TEST_ASSERT_EQUAL(
            pkmn_item_bag_remove(
                bag,
                all_pocket_item_names[i],
                5
            ),
            PKMN_ERROR_NONE
        );
    }

    TEST_ASSERT_EQUAL(
        pkmn_item_list_at(
            item_list_handle,
            0,
            &item_slot
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(item_slot.item, "None");
    TEST_ASSERT_EQUAL(item_slot.amount, 0);
    TEST_ASSERT_EQUAL(
        pkmn_item_slot_free(&item_slot),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_NULL(item_slot.item);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_at(
            keyitem_list_handle,
            0,
            &item_slot
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(item_slot.item, "None");
    TEST_ASSERT_EQUAL(item_slot.amount, 0);
    TEST_ASSERT_EQUAL(
        pkmn_item_slot_free(&item_slot),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_NULL(item_slot.item);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_at(
            ball_list_handle,
            0,
            &item_slot
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(item_slot.item, "None");
    TEST_ASSERT_EQUAL(item_slot.amount, 0);
    TEST_ASSERT_EQUAL(
        pkmn_item_slot_free(&item_slot),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_NULL(item_slot.item);

    TEST_ASSERT_EQUAL(
        pkmn_item_list_at(
            tmhm_list_handle,
            27,
            &item_slot
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(item_slot.item, "TM28");
    TEST_ASSERT_EQUAL(item_slot.amount, 0);
    TEST_ASSERT_EQUAL(
        pkmn_item_slot_free(&item_slot),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_NULL(item_slot.item);

    // Make sure we can't add/remove Crystal-specific items with a Gold/Silver bag.
    if(strcmp(game, "Crystal") == 0) {
        for(int i = 0; i < 4; ++i) {
            TEST_ASSERT_EQUAL(
                pkmn_item_bag_add(
                    bag,
                    crystal_items[i],
                    1
                ),
                PKMN_ERROR_NONE
            );

            TEST_ASSERT_EQUAL(
                pkmn_item_list_at(
                    keyitem_list_handle,
                    0,
                    &item_slot
                ),
                PKMN_ERROR_NONE
            );
            TEST_ASSERT_EQUAL_STRING(item_slot.item, crystal_items[i]);
            TEST_ASSERT_EQUAL(item_slot.amount, 1);
            TEST_ASSERT_EQUAL(
                pkmn_item_slot_free(&item_slot),
                PKMN_ERROR_NONE
            );
            TEST_ASSERT_NULL(item_slot.item);

            TEST_ASSERT_EQUAL(
                pkmn_item_bag_remove(
                    bag,
                    crystal_items[i],
                    1
                ),
                PKMN_ERROR_NONE
            );

            TEST_ASSERT_EQUAL(
                pkmn_item_list_at(
                    keyitem_list_handle,
                    0,
                    &item_slot
                ),
                PKMN_ERROR_NONE
            );
            TEST_ASSERT_EQUAL_STRING(item_slot.item, "None");
            TEST_ASSERT_EQUAL(item_slot.amount, 0);
            TEST_ASSERT_EQUAL(
                pkmn_item_slot_free(&item_slot),
                PKMN_ERROR_NONE
            );
            TEST_ASSERT_NULL(item_slot.item);
        }
    } else {
        test_item_bag_invalid_items(
            bag,
            crystal_items,
            3
        );
    }

    // Make sure we can't add items from later generations.
    test_item_bag_invalid_items(
        bag,
        wrong_generation_all_pocket_items,
        4
    );

    // Cleanup
    TEST_ASSERT_EQUAL(
        pkmn_item_list_free(&item_list_handle),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_NULL(item_list_handle);
    TEST_ASSERT_EQUAL(
        pkmn_item_list_free(&keyitem_list_handle),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_NULL(keyitem_list_handle);
    TEST_ASSERT_EQUAL(
        pkmn_item_list_free(&ball_list_handle),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_NULL(ball_list_handle);
    TEST_ASSERT_EQUAL(
        pkmn_item_list_free(&tmhm_list_handle),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_NULL(tmhm_list_handle);
}
