/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn.h>

static void ability_list_test() {
    pkmn_string_list_t abilities = {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    // Make sure invalid calls return an error
    error_code = pkmn_database_ability_list(
                     0, &abilities
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_RANGE_ERROR);

    error_code = pkmn_database_ability_list(
                     6, &abilities
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(abilities.strings);
    TEST_ASSERT_EQUAL(abilities.length, 191);
    TEST_ASSERT_EQUAL_STRING(abilities.strings[0], "Adaptability");
    TEST_ASSERT_EQUAL_STRING(abilities.strings[abilities.length-1], "Zen Mode");

    pkmn_string_list_free(&abilities);
    TEST_ASSERT_NULL(abilities.strings);
    TEST_ASSERT_EQUAL(abilities.length, 0);
}

static void game_list_test() {
    pkmn_string_list_t games = {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    // Make sure invalid calls return an error
    error_code = pkmn_database_game_list(
                     7, true,
                     &games
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_RANGE_ERROR);

    error_code = pkmn_database_game_list(
                     6, true,
                     &games
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(games.strings);
    TEST_ASSERT_EQUAL(games.length, 26);
    TEST_ASSERT_EQUAL_STRING(games.strings[0], "Red");
    TEST_ASSERT_EQUAL_STRING(games.strings[games.length-1], "Alpha Sapphire");

    pkmn_string_list_free(&games);
    TEST_ASSERT_NULL(games.strings);
    TEST_ASSERT_EQUAL(games.length, 0);
}

static void item_list_test() {
    pkmn_string_list_t items = {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    // Make sure invalid calls return an error
    error_code = pkmn_database_item_list(
                     "Not a game",
                     &items
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_INVALID_ARGUMENT);

    error_code = pkmn_database_item_list(
                     "HeartGold",
                     &items
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(items.strings);
    TEST_ASSERT_EQUAL(items.length, 513);
    TEST_ASSERT_EQUAL_STRING(items.strings[0], "Adamant Orb");
    TEST_ASSERT_EQUAL_STRING(items.strings[items.length-1], "Zoom Lens");

    pkmn_string_list_free(&items);
    TEST_ASSERT_NULL(items.strings);
    TEST_ASSERT_EQUAL(items.length, 0);
}

static void location_list_test() {
    pkmn_string_list_t locations = {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    // Make sure invalid calls return an error
    error_code = pkmn_database_location_list(
                     "Not a game", false,
                     &locations
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_INVALID_ARGUMENT);

    error_code = pkmn_database_location_list(
                     "Emerald", false,
                     &locations
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(locations.strings);
    TEST_ASSERT_EQUAL(locations.length, 106);
    TEST_ASSERT_EQUAL_STRING(locations.strings[0], "Abandoned Ship");
    TEST_ASSERT_EQUAL_STRING(locations.strings[locations.length-1], "Victory Road");

    pkmn_string_list_free(&locations);
    TEST_ASSERT_NULL(locations.strings);
    TEST_ASSERT_EQUAL(locations.length, 0);
}

static void move_list_test() {
    pkmn_string_list_t moves = {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    // Make sure invalid calls return an error
    error_code = pkmn_database_move_list(
                     "Not a game",
                     &moves
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_INVALID_ARGUMENT);

    error_code = pkmn_database_move_list(
                     "Red",
                     &moves
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(moves.strings);
    TEST_ASSERT_EQUAL(moves.length, 165);
    TEST_ASSERT_EQUAL_STRING(moves.strings[0], "Pound");
    TEST_ASSERT_EQUAL_STRING(moves.strings[moves.length-1], "Struggle");

    pkmn_string_list_free(&moves);
    TEST_ASSERT_NULL(moves.strings);
    TEST_ASSERT_EQUAL(moves.length, 0);
}

static void nature_list_test() {
    pkmn_string_list_t natures = {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    error_code = pkmn_database_nature_list(
                     &natures
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(natures.strings);
    TEST_ASSERT_EQUAL(natures.length, 25);
    TEST_ASSERT_EQUAL_STRING(natures.strings[0], "Adamant");
    TEST_ASSERT_EQUAL_STRING(natures.strings[natures.length-1], "Timid");

    pkmn_string_list_free(&natures);
    TEST_ASSERT_NULL(natures.strings);
    TEST_ASSERT_EQUAL(natures.length, 0);
}

static void pokemon_list_test() {
    pkmn_string_list_t pokemon = {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    error_code = pkmn_database_pokemon_list(
                    0, true,
                    &pokemon
                );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_RANGE_ERROR);

    error_code = pkmn_database_pokemon_list(
                     1, true,
                     &pokemon
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(pokemon.strings);
    TEST_ASSERT_EQUAL(pokemon.length, 151);
    TEST_ASSERT_EQUAL_STRING(pokemon.strings[0], "Bulbasaur");
    TEST_ASSERT_EQUAL_STRING(pokemon.strings[pokemon.length-1], "Mew");

    pkmn_string_list_free(&pokemon);
    TEST_ASSERT_NULL(pokemon.strings);
    TEST_ASSERT_EQUAL(pokemon.length, 0);
}

static void region_list_test() {
    pkmn_string_list_t regions = {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    error_code = pkmn_database_region_list(
                     &regions
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(regions.strings);
    TEST_ASSERT_EQUAL(regions.length, 7);
    TEST_ASSERT_EQUAL_STRING(regions.strings[0], "Kanto");
    TEST_ASSERT_EQUAL_STRING(regions.strings[regions.length-1], "Kalos");

    pkmn_string_list_free(&regions);
    TEST_ASSERT_NULL(regions.strings);
    TEST_ASSERT_EQUAL(regions.length, 0);
}

static void ribbon_list_test() {
    // TODO
}

static void super_training_medal_list_test() {
    pkmn_string_list_t super_training_medals = {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    error_code = pkmn_database_super_training_medal_list(
                     &super_training_medals
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(super_training_medals.strings);
    TEST_ASSERT_EQUAL(super_training_medals.length, 30);
    TEST_ASSERT_EQUAL_STRING(super_training_medals.strings[0], "Sp. Atk Level 1");
    TEST_ASSERT_EQUAL_STRING(super_training_medals.strings[super_training_medals.length-1], "The Battle for the Best!");

    pkmn_string_list_free(&super_training_medals);
    TEST_ASSERT_NULL(super_training_medals.strings);
    TEST_ASSERT_EQUAL(super_training_medals.length, 0);
}

static void type_list_test() {
    pkmn_string_list_t types = {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error_code = PKMN_ERROR_NONE;

    // Make sure invalid calls return an error
    error_code = pkmn_database_type_list(
                     "Not a game",
                     &types
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_INVALID_ARGUMENT);

    error_code = pkmn_database_type_list(
                     "Alpha Sapphire",
                     &types
                 );
    TEST_ASSERT_EQUAL(error_code, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(types.strings);
    TEST_ASSERT_EQUAL(types.length, 18);
    TEST_ASSERT_EQUAL_STRING(types.strings[0], "Normal");
    TEST_ASSERT_EQUAL_STRING(types.strings[types.length-1], "Fairy");

    pkmn_string_list_free(&types);
    TEST_ASSERT_NULL(types.strings);
    TEST_ASSERT_EQUAL(types.length, 0);
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
    PKMN_C_TEST(type_list_test)
)
