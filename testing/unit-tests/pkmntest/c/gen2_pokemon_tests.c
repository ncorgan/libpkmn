/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_tests_common.h"

#include <pkmntest-c/gen2_pokemon_tests.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/calculations/form.h>

#include <unity.h>

#include <ctype.h>
#include <string.h>

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};

void pkmntest_gen2_invalid_pokemon_test(
    const char* game
) {
    test_invalid_pokemon(game);
}

void pkmntest_gen2_unown_test(
    const char* game
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_handle_t unown = NULL;

    pkmn_database_pokemon_entry_t unown_entry;
    error = pkmn_database_get_pokemon_entry(
                "Unown",
                game,
                "",
                &unown_entry
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    TEST_ASSERT_EQUAL(unown_entry.forms.length, 26);

    for(size_t i = 0; i < unown_entry.forms.length; ++i) {
        error = pkmn_pokemon_make(
                    &unown,
                    "Unown",
                    game,
                    unown_entry.forms.strings[i],
                    5
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_get_form(
                    unown,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL_STRING(strbuffer, unown_entry.forms.strings[i]);

        // Make sure IVs are properly set
        int IV_attack = 0;
        int IV_defense = 0;
        int IV_speed = 0;
        int IV_special = 0;

        error = pkmn_pokemon_get_IV(
                    unown,
                    "Attack",
                    &IV_attack
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_get_IV(
                    unown,
                    "Defense",
                    &IV_defense
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_get_IV(
                    unown,
                    "Speed",
                    &IV_speed
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_get_IV(
                    unown,
                    "Special",
                    &IV_special
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_calculations_gen2_unown_form(
                    IV_attack,
                    IV_defense,
                    IV_speed,
                    IV_special,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL_STRING(strbuffer, unown_entry.forms.strings[i]);

        error = pkmn_pokemon_free(&unown);
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_NULL(unown);
    }

    // Make sure setting IVs properly changes the form.
    error = pkmn_pokemon_make(
                &unown,
                "Unown",
                game,
                "A",
                5
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_set_IV(
                unown,
                "Attack",
                10
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_set_IV(
                unown,
                "Defense",
                9
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_set_IV(
                unown,
                "Speed",
                1
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_set_IV(
                unown,
                "Special",
                14
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_get_form(
                unown,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "G");

    error = pkmn_pokemon_free(&unown);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(unown);

    error = pkmn_database_pokemon_entry_free(&unown_entry);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
}
