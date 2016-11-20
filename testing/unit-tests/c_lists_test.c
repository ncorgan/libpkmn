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
    TEST_ASSERT_EQUAL_STRING(abilities[0], "Adaptability");
    TEST_ASSERT_EQUAL_STRING(abilities[list_length-1], "Zen Mode");

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
    TEST_ASSERT_EQUAL_STRING(games[0], "Red");
    TEST_ASSERT_EQUAL_STRING(games[list_length-1], "Alpha Sapphire");

    pkmn_string_list_free(&games, list_length);
    TEST_ASSERT_NULL(games);
}

static void item_list_test() {
    pkmn_string_list_t items = NULL;
    size_t list_length = 0;
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    error_code = pkmn_database_item_list(
                     "HeartGold",
                     &items,
                     &list_length
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(items);
    TEST_ASSERT_EQUAL(list_length, 513);
    TEST_ASSERT_EQUAL_STRING(items[0], "Adamant Orb");
    TEST_ASSERT_EQUAL_STRING(items[list_length-1], "Zoom Lens");

    pkmn_string_list_free(&items, list_length);
    TEST_ASSERT_NULL(items);
}

static void location_list_test() {
    pkmn_string_list_t locations = NULL;
    size_t list_length = 0;
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    error_code = pkmn_database_location_list(
                     "Emerald", false,
                     &locations,
                     &list_length
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(locations);
    TEST_ASSERT_EQUAL(list_length, 106);
    TEST_ASSERT_EQUAL_STRING(locations[0], "Abandoned Ship");
    TEST_ASSERT_EQUAL_STRING(locations[list_length-1], "Victory Road");

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
    TEST_ASSERT_NOT_NULL(moves);
    TEST_ASSERT_EQUAL(list_length, 165);
    TEST_ASSERT_EQUAL_STRING(moves[0], "Pound");
    TEST_ASSERT_EQUAL_STRING(moves[list_length-1], "Struggle");

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
    TEST_ASSERT_NOT_NULL(natures);
    TEST_ASSERT_EQUAL(list_length, 25);
    TEST_ASSERT_EQUAL_STRING(natures[0], "Adamant");
    TEST_ASSERT_EQUAL_STRING(natures[list_length-1], "Timid");

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
    TEST_ASSERT_NOT_NULL(pokemon);
    TEST_ASSERT_EQUAL(list_length, 151);
    TEST_ASSERT_EQUAL_STRING(pokemon[0], "Bulbasaur");
    TEST_ASSERT_EQUAL_STRING(pokemon[list_length-1], "Mew");

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
    TEST_ASSERT_NOT_NULL(regions);
    TEST_ASSERT_EQUAL(list_length, 7);
    TEST_ASSERT_EQUAL_STRING(regions[0], "Kanto");
    TEST_ASSERT_EQUAL_STRING(regions[list_length-1], "Kalos");

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
    TEST_ASSERT_NOT_NULL(super_training_medals);
    TEST_ASSERT_EQUAL(list_length, 30);
    TEST_ASSERT_EQUAL_STRING(super_training_medals[0], "Sp. Atk Level 1");
    TEST_ASSERT_EQUAL_STRING(super_training_medals[list_length-1], "The Battle for the Best!");

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
