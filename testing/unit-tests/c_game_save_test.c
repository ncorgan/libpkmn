/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmntest-c/game_save_tests.h>

#include <pkmn.h>

static pkmn_error_t error = PKMN_ERROR_NONE;
static pkmn_game_save_type_t game_save_type = PKMN_GAME_SAVE_TYPE_NONE;
static pkmn_game_save_handle_t game_save = NULL;

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};
static size_t actual_strlen = 0;

/*
 * Red
 */
static void red_test() {
    const char* filepath = "@POKEMON_RED_SAV@";

    // Make sure we'll be using the correct implementation.
    error = pkmn_game_save_detect_type(
                filepath,
                &game_save_type
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(game_save_type, PKMN_GAME_SAVE_TYPE_RED_BLUE_YELLOW);

    error = pkmn_game_save_from_file(
                &game_save,
                filepath
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_game_save_get_game(
                game_save,
                strbuffer,
                STRBUFFER_LEN,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Red");

    pkmntest_game_save_test_common_fields(game_save);
}

/*
 * TODO: Blue
 */

/*
 * Yellow
 */
static void yellow_test() {
    const char* filepath = "@POKEMON_YELLOW_SAV@";

    // Make sure we'll be using the correct implementation.
    error = pkmn_game_save_detect_type(
                filepath,
                &game_save_type
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(game_save_type, PKMN_GAME_SAVE_TYPE_RED_BLUE_YELLOW);

    error = pkmn_game_save_from_file(
                &game_save,
                filepath
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_game_save_get_game(
                game_save,
                strbuffer,
                STRBUFFER_LEN,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Yellow");

    pkmntest_game_save_test_common_fields(game_save);
}

/*
 * Gold
 */
static void gold_test() {
    const char* filepath = "@POKEMON_GOLD_SAV@";

    // Make sure we'll be using the correct implementation.
    error = pkmn_game_save_detect_type(
                filepath,
                &game_save_type
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(game_save_type, PKMN_GAME_SAVE_TYPE_GOLD_SILVER);

    error = pkmn_game_save_from_file(
                &game_save,
                filepath
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_game_save_get_game(
                game_save,
                strbuffer,
                STRBUFFER_LEN,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Gold");

    pkmntest_game_save_test_common_fields(game_save);
}

/*
 * TODO: Silver
 */

/*
 * Crystal
 */
static void crystal_test() {
    const char* filepath = "@POKEMON_CRYSTAL_SAV@";

    // Make sure we'll be using the correct implementation.
    error = pkmn_game_save_detect_type(
                filepath,
                &game_save_type
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(game_save_type, PKMN_GAME_SAVE_TYPE_CRYSTAL);

    error = pkmn_game_save_from_file(
                &game_save,
                filepath
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_game_save_get_game(
                game_save,
                strbuffer,
                STRBUFFER_LEN,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Crystal");

    pkmntest_game_save_test_common_fields(game_save);
}

/*
 * Ruby
 */
static void ruby_test() {
    const char* filepath = "@POKEMON_RUBY_SAV@";

    // Make sure we'll be using the correct implementation.
    error = pkmn_game_save_detect_type(
                filepath,
                &game_save_type
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(game_save_type, PKMN_GAME_SAVE_TYPE_RUBY_SAPPHIRE);

    error = pkmn_game_save_from_file(
                &game_save,
                filepath
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_game_save_get_game(
                game_save,
                strbuffer,
                STRBUFFER_LEN,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Ruby");

    pkmntest_game_save_test_common_fields(game_save);
}

/*
 * TODO: Sapphire
 */

/*
 * Emerald
 */
static void emerald_test() {
    const char* filepath = "@POKEMON_EMERALD_SAV@";

    // Make sure we'll be using the correct implementation.
    error = pkmn_game_save_detect_type(
                filepath,
                &game_save_type
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(game_save_type, PKMN_GAME_SAVE_TYPE_EMERALD);

    error = pkmn_game_save_from_file(
                &game_save,
                filepath
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_game_save_get_game(
                game_save,
                strbuffer,
                STRBUFFER_LEN,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Emerald");

    pkmntest_game_save_test_common_fields(game_save);
}

/*
 * FireRed
 */
static void firered_test() {
    const char* filepath = "@POKEMON_FIRERED_SAV@";

    // Make sure we'll be using the correct implementation.
    error = pkmn_game_save_detect_type(
                filepath,
                &game_save_type
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(game_save_type, PKMN_GAME_SAVE_TYPE_FIRERED_LEAFGREEN);

    error = pkmn_game_save_from_file(
                &game_save,
                filepath
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_game_save_get_game(
                game_save,
                strbuffer,
                STRBUFFER_LEN,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "FireRed");

    pkmntest_game_save_test_common_fields(game_save);
}

/*
 * TODO: LeafGreen
 */

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(red_test)
    PKMN_C_TEST(yellow_test)
    PKMN_C_TEST(gold_test)
    PKMN_C_TEST(crystal_test)
    PKMN_C_TEST(ruby_test)
    PKMN_C_TEST(emerald_test)
    PKMN_C_TEST(firered_test)
)
