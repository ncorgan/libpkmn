/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest-c/util.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/calculations/form.h>
#include <pkmn-c/database/move_entry.h>

#include <unity.h>

#include <ctype.h>
#include <string.h>

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};

static void gen2_unown_test(
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

        error = pkmn_pokemon_get_icon_filepath(
                    unown,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_TRUE(file_exists(strbuffer));

        error = pkmn_pokemon_get_sprite_filepath(
                    unown,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_TRUE(file_exists(strbuffer));

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

    // Make sure setting the form properly changes the IVs.
    for(size_t i = 0; i < unown_entry.forms.length; ++i) {
        error = pkmn_pokemon_set_form(
                    unown,
                    unown_entry.forms.strings[i]
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
    }

    error = pkmn_pokemon_free(&unown);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(unown);

    error = pkmn_database_pokemon_entry_free(&unown_entry);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
}

static void gen3_unown_test(
    const char* game
) {
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_handle_t unown = NULL;
    uint32_t personality = 0;

    pkmn_database_pokemon_entry_t unown_entry;
    error = pkmn_database_get_pokemon_entry(
                "Unown",
                game,
                "",
                &unown_entry
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_EQUAL(28, unown_entry.forms.length);

    for(size_t i = 0; i < unown_entry.forms.length; ++i) {
        error = pkmn_pokemon_make(
                    &unown,
                    "Unown",
                    game,
                    unown_entry.forms.strings[i],
                    5
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_get_form(
                    unown,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(strbuffer, unown_entry.forms.strings[i]);

        // Make sure personality is properly set.
        error = pkmn_pokemon_get_personality(
                    unown,
                    &personality
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_calculations_gen3_unown_form(
                    personality,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(strbuffer, unown_entry.forms.strings[i]);

        if(strcmp(game, "Colosseum") && strcmp(game, "XD")) {
            error = pkmn_pokemon_get_icon_filepath(
                        unown,
                        strbuffer,
                        sizeof(strbuffer)
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_TRUE(file_exists(strbuffer));

            error = pkmn_pokemon_get_sprite_filepath(
                        unown,
                        strbuffer,
                        sizeof(strbuffer)
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_TRUE(file_exists(strbuffer));
        }

        error = pkmn_pokemon_free(&unown);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(unown);
    }

    // Make sure setting the personality properly sets the form.
    error = pkmn_pokemon_make(
                &unown,
                "Unown",
                game,
                "A",
                5
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_set_personality(
                unown,
                0x4C07DE71
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_get_form(
                unown,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "B");

    for(size_t i = 0; i < unown_entry.forms.length; ++i) {
        error = pkmn_pokemon_set_form(
                    unown,
                    unown_entry.forms.strings[i]
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_get_personality(
                    unown,
                    &personality
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_calculations_gen3_unown_form(
                    personality,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(strbuffer, unown_entry.forms.strings[i]);
    }

    error = pkmn_pokemon_free(&unown);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(unown);

    error = pkmn_database_pokemon_entry_free(&unown_entry);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

void gold_unown_test() {
    gen2_unown_test("Gold");
}

void silver_unown_test() {
    gen2_unown_test("Silver");
}

void crystal_unown_test() {
    gen2_unown_test("Crystal");
}

void ruby_unown_test() {
    gen3_unown_test("Ruby");
}

void sapphire_unown_test() {
    gen3_unown_test("Sapphire");
}

void emerald_unown_test() {
    gen3_unown_test("Emerald");
}

void firered_unown_test() {
    gen3_unown_test("FireRed");
}

void leafgreen_unown_test() {
    gen3_unown_test("LeafGreen");
}

void colosseum_unown_test() {
    gen3_unown_test("Colosseum");
}

void xd_unown_test() {
    gen3_unown_test("XD");
}
