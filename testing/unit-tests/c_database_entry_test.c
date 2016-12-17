/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn.h>

#include <string.h>

static void assert_item_entry_uninitialized(
    pkmn_database_item_entry_t* item_entry
) {
    TEST_ASSERT_NULL(item_entry->name);
    TEST_ASSERT_NULL(item_entry->game);
    TEST_ASSERT_NULL(item_entry->category);
    TEST_ASSERT_NULL(item_entry->pocket);
    TEST_ASSERT_NULL(item_entry->description);
    TEST_ASSERT_EQUAL(item_entry->cost, 0);
    TEST_ASSERT_FALSE(item_entry->holdable);
    TEST_ASSERT_EQUAL(item_entry->fling_power, 0);
    TEST_ASSERT_NULL(item_entry->fling_effect);
}

static void item_entry_test() {
    pkmn_database_item_entry_t item_entry = {
        .name = NULL,
        .game = NULL,
        .category = NULL,
        .pocket = NULL,
        .description = NULL,
        .cost = 0,
        .holdable = false,
        .fling_power = 0,
        .fling_effect = NULL
    };

    /*
     * Make sure expected errors are returned and there is no effect on
     * the output upon failure.
     */
    TEST_ASSERT_EQUAL(
        pkmn_database_get_item_entry(
            "Not an item",
            "SoulSilver",
            &item_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_item_entry_uninitialized(
        &item_entry
    );
    TEST_ASSERT_EQUAL(
        pkmn_database_get_item_entry(
            "Fast Ball",
            "Not a game",
            &item_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_item_entry_uninitialized(
        &item_entry
    );
    TEST_ASSERT_EQUAL(
        pkmn_database_get_item_entry(
            "Fast Ball",
            "Red",
            &item_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_item_entry_uninitialized(
        &item_entry
    );

    TEST_ASSERT_EQUAL(
        pkmn_database_get_item_entry(
            "Fast Ball",
            "SoulSilver",
            &item_entry
        ),
        PKMN_ERROR_NONE
    );

    // Make sure fields are properly populated
    TEST_ASSERT_EQUAL_STRING(item_entry.name, "Fast Ball");
    TEST_ASSERT_EQUAL_STRING(item_entry.game, "SoulSilver");
    TEST_ASSERT_EQUAL_STRING(item_entry.category, "Apricorn balls");
    TEST_ASSERT_EQUAL_STRING(item_entry.pocket, "Poké Balls");
    TEST_ASSERT_NOT_NULL(item_entry.description);
    TEST_ASSERT_EQUAL(item_entry.cost, 300);
    TEST_ASSERT(item_entry.holdable);
    TEST_ASSERT_EQUAL(item_entry.fling_power, 0);
    TEST_ASSERT_EQUAL_STRING(item_entry.fling_effect, "None");

    // Make sure freeing properly works
    TEST_ASSERT_EQUAL(
        pkmn_database_item_entry_free(&item_entry),
        PKMN_ERROR_NONE
    );
    assert_item_entry_uninitialized(
        &item_entry
    );
}

static void assert_move_entry_uninitialized(
    pkmn_database_move_entry_t* move_entry
) {
    TEST_ASSERT_NULL(move_entry->name);
    TEST_ASSERT_NULL(move_entry->game);
    TEST_ASSERT_NULL(move_entry->description);
    TEST_ASSERT_NULL(move_entry->target);
    TEST_ASSERT_NULL(move_entry->damage_class);
    TEST_ASSERT_EQUAL(move_entry->base_power, 0);
    for(int i = 0; i < 4; ++i) {
        TEST_ASSERT_EQUAL(move_entry->pp[i], 0);
    }
    TEST_ASSERT_EQUAL_FLOAT(move_entry->accuracy, 0.0f);
    TEST_ASSERT_NULL(move_entry->effect);
    TEST_ASSERT_NULL(move_entry->contest_type);
    TEST_ASSERT_NULL(move_entry->contest_effect);
    TEST_ASSERT_NULL(move_entry->super_contest_effect);
}

static void move_entry_test() {
    pkmn_database_move_entry_t move_entry = {
        .name = NULL,
        .game = NULL,
        .description = NULL,
        .target = NULL,
        .damage_class = NULL,
        .base_power = 0,
        .pp = {0,0,0,0},
        .accuracy = 0.0f,
        .effect = NULL,
        .contest_type = NULL,
        .contest_effect = NULL,
        .super_contest_effect = NULL
    };

    /*
     * Make sure expected errors are returned and there is no effect on
     * the output upon failure.
     */
    TEST_ASSERT_EQUAL(
        pkmn_database_get_move_entry(
            "Not a move",
            "Silver",
            &move_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_move_entry_uninitialized(
        &move_entry
    );
    TEST_ASSERT_EQUAL(
        pkmn_database_get_move_entry(
            "Octazooka",
            "Not a game",
            &move_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_move_entry_uninitialized(
        &move_entry
    );
    TEST_ASSERT_EQUAL(
        pkmn_database_get_move_entry(
            "Octazooka",
            "Red",
            &move_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_move_entry_uninitialized(
        &move_entry
    );

    TEST_ASSERT_EQUAL(
        pkmn_database_get_move_entry(
            "Octazooka",
            "Silver",
            &move_entry
        ),
        PKMN_ERROR_NONE
    );

    // Make sure fields are properly populated
    TEST_ASSERT_EQUAL_STRING(move_entry.name, "Octazooka");
    TEST_ASSERT_EQUAL_STRING(move_entry.game, "Silver");
    TEST_ASSERT_NOT_NULL(move_entry.description);
    TEST_ASSERT_EQUAL_STRING(move_entry.target, "Selected Pokémon");
    TEST_ASSERT_EQUAL_STRING(move_entry.damage_class, "Special");
    TEST_ASSERT_EQUAL(move_entry.base_power, 65);
    TEST_ASSERT_EQUAL(move_entry.pp[0], 10);
    TEST_ASSERT_EQUAL(move_entry.pp[1], 12);
    TEST_ASSERT_EQUAL(move_entry.pp[2], 14);
    TEST_ASSERT_EQUAL(move_entry.pp[3], 16);
    TEST_ASSERT_EQUAL_FLOAT(move_entry.accuracy, 0.85f);
    TEST_ASSERT_NOT_NULL(move_entry.effect);
    TEST_ASSERT_EQUAL_STRING(move_entry.contest_type, "None");
    TEST_ASSERT_EQUAL_STRING(move_entry.contest_effect, "None");
    TEST_ASSERT_EQUAL_STRING(move_entry.super_contest_effect, "None");

    // Make sure freeing properly works
    TEST_ASSERT_EQUAL(
        pkmn_database_move_entry_free(&move_entry),
        PKMN_ERROR_NONE
    );
    assert_move_entry_uninitialized(
        &move_entry
    );
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(item_entry_test)
    PKMN_C_TEST(move_entry_test)
)
