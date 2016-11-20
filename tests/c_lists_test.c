/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn.h>

static void ability_list_test() {
    pkmn_string_list_t abilities = NULL;
    size_t list_length = 0;
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    // Make sure invalid generations return PKMN_ERROR_OUT_OF_RANGE
    error_code = pkmn_database_ability_list(
                     0, &abilities,
                     &list_length
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_OUT_OF_RANGE);

    error_code = pkmn_database_ability_list(
                     6, &abilities,
                     &list_length
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(abilities);
    TEST_ASSERT_EQUAL(list_length, 191);

    pkmn_string_list_free(&abilities, list_length);
    TEST_ASSERT_NULL(abilities);
}

static void game_list_test() {
    pkmn_string_list_t games = NULL;
    size_t list_length = 0;
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    // Make sure invalid generations return PKMN_ERROR_OUT_OF_RANGE
    error_code = pkmn_database_game_list(
                     7, true,
                     &games,
                     &list_length
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_OUT_OF_RANGE);

    error_code = pkmn_database_game_list(
                     6, true,
                     &games,
                     &list_length
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(games);
    TEST_ASSERT_EQUAL(list_length, 26);

    pkmn_string_list_free(&games, list_length);
    TEST_ASSERT_NULL(games);
}

/*
 * TODO: rest when database branch merged in
 *  * Check invalid params where appropriate
 *  * Check for non-null string lists
 *  * Check string list lengths
 */

static void item_list_test() {
    pkmn_string_list_t items = NULL;
    size_t list_length = 0;
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    error_code = pkmn_database_item_list(
                     "Red",
                     &items,
                     &list_length
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT(list_length > 0);

    pkmn_string_list_free(&items, list_length);
    TEST_ASSERT_NULL(items);
}

static void location_list_test() {
    pkmn_string_list_t locations = NULL;
    size_t list_length = 0;
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    error_code = pkmn_database_location_list(
                     "Ruby", false,
                     &locations,
                     &list_length
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);

    pkmn_string_list_free(&locations, list_length);
    TEST_ASSERT_NULL(locations);
}

static void move_list_test() {
    pkmn_string_list_t moves = NULL;
    size_t list_length = 0;
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    error_code = pkmn_database_move_list(
                     "Red",
                     &moves,
                     &list_length
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(moves[0], "Pound");
    TEST_ASSERT(list_length > 0);

    pkmn_string_list_free(&moves, list_length);
    TEST_ASSERT_NULL(moves);
}

static void nature_list_test() {
    pkmn_string_list_t natures = NULL;
    size_t list_length = 0;
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    error_code = pkmn_database_nature_list(
                     &natures,
                     &list_length
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(list_length, 25);

    pkmn_string_list_free(&natures, list_length);
    TEST_ASSERT_NULL(natures);
}

static void pokemon_list_test() {
    pkmn_string_list_t pokemon = NULL;
    size_t list_length = 0;
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    error_code = pkmn_database_pokemon_list(
                     1, true,
                     &pokemon,
                     &list_length
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(list_length, 151);
    TEST_ASSERT_EQUAL_STRING(pokemon[0], "Bulbasaur");
    TEST_ASSERT_EQUAL_STRING(pokemon[150], "Mew");

    pkmn_string_list_free(&pokemon, list_length);
    TEST_ASSERT_NULL(pokemon);
}

static void region_list_test() {
    pkmn_string_list_t regions = NULL;
    size_t list_length = 0;
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    error_code = pkmn_database_region_list(
                     &regions,
                     &list_length
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);

    pkmn_string_list_free(&regions, list_length);
    TEST_ASSERT_NULL(regions);
}

static void ribbon_list_test() {
    // TODO
}

static void super_training_medal_list_test() {
    pkmn_string_list_t super_training_medals = NULL;
    size_t list_length = 0;
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    error_code = pkmn_database_super_training_medal_list(
                     &super_training_medals,
                     &list_length
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);

    pkmn_string_list_free(&super_training_medals, list_length);
    TEST_ASSERT_NULL(super_training_medals);
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(ability_list_test)
    PKMN_C_TEST(game_list_test)
    PKMN_C_TEST(item_list_test)
    PKMN_C_TEST(location_list_test)
    PKMN_C_TEST(move_list_test)
    PKMN_C_TEST(nature_list_test)
    PKMN_C_TEST(pokemon_list_test)
    PKMN_C_TEST(region_list_test)
    PKMN_C_TEST(ribbon_list_test)
    PKMN_C_TEST(super_training_medal_list_test)
)
