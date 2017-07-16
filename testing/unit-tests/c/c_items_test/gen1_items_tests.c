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

#define STRBUFFER_LEN 1024

static const char* item_names[] = {
    "Potion",
    "Great Ball",
    "Ether",
    "PP Up",
    "TM34",
    "Moon Stone",
    "Bicycle",
    "Full Heal"
};
static const char* wrong_generation_item_names[] = {
    "Amulet Coin",
    "Apicot Berry",
    "Air Mail",
    "Air Balloon",
    "Aqua Suit"
};

static void gen1_item_list_common(
    pkmn_item_list_handle_t items,
    const char* game
) {
    pkmn_string_list_t full_item_list = {
        .strings = NULL,
        .length = 0
    };
    pkmn_string_list_t valid_items = {
        .strings = NULL,
        .length = 0
    };

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(items);

    // Confirm errors are returned when expected.
    test_item_list_out_of_range_error(
        items,
        "Potion"
    );

    // Confirm items from later generations can't be added.
    test_item_list_invalid_items(
        items,
        wrong_generation_item_names,
        5
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    test_item_list_add_remove(
        items,
        item_names,
        8
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_valid_items(
            items, &valid_items
        )
    );
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_database_item_list(
            game, &full_item_list
        )
    );
    TEST_ASSERT_EQUAL(full_item_list.length, valid_items.length);

    pkmn_string_list_free(&full_item_list);
    TEST_ASSERT_NULL(full_item_list.strings);
    TEST_ASSERT_EQUAL(0, full_item_list.length);

    pkmn_string_list_free(&valid_items);
    TEST_ASSERT_NULL(valid_items.strings);
    TEST_ASSERT_EQUAL(0, valid_items.length);
}

void gen1_item_list_test(
    pkmn_item_list_handle_t items,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN] = "";

    // Check unchanging and initial values.
    const char* expected_name = "Items";
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_name(
            items,
            strbuffer,
            sizeof(strbuffer)
        )
    );
    TEST_ASSERT_EQUAL_STRING(expected_name, strbuffer);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_game(
            items,
            strbuffer,
            sizeof(strbuffer)
        )
    );
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    int capacity = 0;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_capacity(
            items, &capacity
        )
    );
    TEST_ASSERT_EQUAL(20, capacity);

    int num_items = 0;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            items, &num_items
        )
    );
    TEST_ASSERT_EQUAL(0, num_items);

    gen1_item_list_common(items, game);
}

void gen1_pc_test(
    pkmn_item_list_handle_t pc,
    const char* game
) {
    char strbuffer[STRBUFFER_LEN] = "";

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

    int capacity = 0;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_capacity(
            pc, &capacity
        )
    );
    TEST_ASSERT_EQUAL(50, capacity);

    int num_items = 0;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            pc, &num_items
        )
    );
    TEST_ASSERT_EQUAL(0, num_items);

    gen1_item_list_common(pc, game);
}

void gen1_item_bag_test(
    pkmn_item_bag_handle_t bag,
    const char* game
) {
    pkmn_string_list_t pocket_names = {
        .strings = NULL,
        .length = 0
    };
    pkmn_item_slots_t item_slots = {
        .item_slots = NULL,
        .length = 0
    };

    char strbuffer[STRBUFFER_LEN] = "";

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

    test_item_bag_invalid_items(
        bag,
        wrong_generation_item_names,
        5
    );

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_get_pocket_names(
            bag, &pocket_names
        )
    );
    TEST_ASSERT_EQUAL(1, pocket_names.length);

    pkmn_string_list_free(&pocket_names);
    TEST_ASSERT_NULL(pocket_names.strings);
    TEST_ASSERT_EQUAL(0, pocket_names.length);

    pkmn_item_list_handle_t items;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_bag_get_pocket(
            bag, "Items", &items
        )
    );

    gen1_item_list_test(items, game);

    // Make sure adding items through the bag adds to the pocket.
    int num_items = 0;
    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_get_num_items(
            items, &num_items
        )
    );
    TEST_ASSERT_EQUAL(0, num_items);

    for(int i = 0; i < 8; ++i) {
        TEST_ASSERT_EQUAL(
            PKMN_ERROR_NONE,
            pkmn_item_bag_add(
                bag,
                item_names[i],
                i+1
            )
        );
    }

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_as_array(
            items,
            &item_slots
        )
    );

    for(int i = 0; i < 8; ++i) {
        TEST_ASSERT_EQUAL_STRING(item_names[i], item_slots.item_slots[i].item);
        TEST_ASSERT_EQUAL(i+1, item_slots.item_slots[i].amount);
    }

    TEST_ASSERT_EQUAL_STRING("None", item_slots.item_slots[8].item);
    TEST_ASSERT_EQUAL(0, item_slots.item_slots[8].amount);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_slots_free(&item_slots)
    );
    TEST_ASSERT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL(0, item_slots.length);

    for(int i = 0; i < 8; ++i) {
        TEST_ASSERT_EQUAL(
            PKMN_ERROR_NONE,
            pkmn_item_bag_remove(
                bag,
                item_names[i],
                i+1
            )
        );
    }

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_as_array(
            items,
            &item_slots
        )
    );

    for(int i = 0; i < 9; ++i) {
        TEST_ASSERT_EQUAL_STRING("None", item_slots.item_slots[i].item);
        TEST_ASSERT_EQUAL(0, item_slots.item_slots[i].amount);
    }

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_slots_free(&item_slots)
    );
    TEST_ASSERT_NULL(item_slots.item_slots);
    TEST_ASSERT_EQUAL(0, item_slots.length);

    TEST_ASSERT_EQUAL(
        PKMN_ERROR_NONE,
        pkmn_item_list_free(&items)
    );
    TEST_ASSERT_NULL(items);
}
