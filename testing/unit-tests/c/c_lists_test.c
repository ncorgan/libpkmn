/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn.h>

static void ability_list_test()
{
    pkmn_string_list_t abilities =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = PKMN_ERROR_NONE;

    // Make sure invalid calls return an error
    error = pkmn_database_ability_list(
                0,
                &abilities
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_database_ability_list(
                6, &abilities
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(abilities.strings);
    TEST_ASSERT_EQUAL(191, abilities.length);
    TEST_ASSERT_EQUAL_STRING(
        "Adaptability",
        abilities.strings[0]
    );
    TEST_ASSERT_EQUAL_STRING("Zen Mode", abilities.strings[abilities.length-1]);

    pkmn_string_list_free(&abilities);
    TEST_ASSERT_NULL(abilities.strings);
    TEST_ASSERT_EQUAL(0, abilities.length);
}

static void game_list_test()
{
    pkmn_string_list_t games =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = PKMN_ERROR_NONE;

    // Make sure invalid calls return an error
    error = pkmn_database_game_list(
                     7, true,
                     &games
                 );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_database_game_list(
                6,
                true,
                &games
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(games.strings);
    TEST_ASSERT_EQUAL(26, games.length);
    TEST_ASSERT_EQUAL_STRING(
        "Red",
        games.strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Alpha Sapphire",
        games.strings[games.length-1]
    );

    pkmn_string_list_free(&games);
    TEST_ASSERT_NULL(games.strings);
    TEST_ASSERT_EQUAL(0, games.length);
}

static void gamecube_shadow_pokemon_list_test()
{
    pkmn_string_list_t gamecube_shadow_pokemon =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_database_gamecube_shadow_pokemon_list(
                true,
                &gamecube_shadow_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(48, gamecube_shadow_pokemon.length);

    pkmn_string_list_free(&gamecube_shadow_pokemon);
    TEST_ASSERT_NULL(gamecube_shadow_pokemon.strings);
    TEST_ASSERT_EQUAL(0, gamecube_shadow_pokemon.length);

    error = pkmn_database_gamecube_shadow_pokemon_list(
                false, &gamecube_shadow_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(83, gamecube_shadow_pokemon.length);

    pkmn_string_list_free(&gamecube_shadow_pokemon);
    TEST_ASSERT_NULL(gamecube_shadow_pokemon.strings);
    TEST_ASSERT_EQUAL(0, gamecube_shadow_pokemon.length);
}

static void item_list_test()
{
    pkmn_string_list_t items =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = PKMN_ERROR_NONE;

    // Make sure invalid calls return an error
    error = pkmn_database_item_list(
                "Not a game",
                &items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_database_item_list(
                "HeartGold",
                &items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(items.strings);
    TEST_ASSERT_EQUAL(513, items.length);
    TEST_ASSERT_EQUAL_STRING(
        "Adamant Orb",
        items.strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Zoom Lens",
        items.strings[items.length-1]
    );

    pkmn_string_list_free(&items);
    TEST_ASSERT_NULL(items.strings);
    TEST_ASSERT_EQUAL(0, items.length);
}

static void location_list_test()
{
    pkmn_string_list_t locations =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = PKMN_ERROR_NONE;

    // Make sure invalid calls return an error
    error = pkmn_database_location_list(
                "Not a game", false,
                &locations
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_database_location_list(
                     "Emerald", false,
                     &locations
                 );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(locations.strings);
    TEST_ASSERT_EQUAL(106, locations.length);
    TEST_ASSERT_EQUAL_STRING(
        "Abandoned Ship",
        locations.strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Victory Road",
        locations.strings[locations.length-1]
    );

    pkmn_string_list_free(&locations);
    TEST_ASSERT_NULL(locations.strings);
    TEST_ASSERT_EQUAL(0, locations.length);
}

static void move_list_test()
{
    pkmn_string_list_t moves =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = PKMN_ERROR_NONE;

    // Make sure invalid calls return an error
    error = pkmn_database_move_list(
                "Not a game",
                &moves
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_database_move_list(
                "Red",
                &moves
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(moves.strings);
    TEST_ASSERT_EQUAL(165, moves.length);
    TEST_ASSERT_EQUAL_STRING(
        "Pound",
        moves.strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Struggle",
        moves.strings[moves.length-1]
    );

    pkmn_string_list_free(&moves);
    TEST_ASSERT_NULL(moves.strings);
    TEST_ASSERT_EQUAL(0, moves.length);
}

static void nature_list_test()
{
    pkmn_string_list_t natures =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_database_nature_list(
                &natures
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(natures.strings);
    TEST_ASSERT_EQUAL(25, natures.length);
    TEST_ASSERT_EQUAL_STRING(
        "Hardy",
        natures.strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Quirky",
        natures.strings[natures.length-1]
    );

    pkmn_string_list_free(&natures);
    TEST_ASSERT_NULL(natures.strings);
    TEST_ASSERT_EQUAL(0, natures.length);
}

static void pokemon_list_test()
{
    pkmn_string_list_t pokemon =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_database_pokemon_list(
                0,
                true,
                &pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_database_pokemon_list(
               1,
               true,
               &pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(pokemon.strings);
    TEST_ASSERT_EQUAL(151, pokemon.length);
    TEST_ASSERT_EQUAL_STRING(
        "Bulbasaur",
        pokemon.strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Mew",
        pokemon.strings[pokemon.length-1]
    );

    pkmn_string_list_free(&pokemon);
    TEST_ASSERT_NULL(pokemon.strings);
    TEST_ASSERT_EQUAL(0, pokemon.length);
}

static void region_list_test()
{
    pkmn_string_list_t regions =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_database_region_list(
                &regions
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(regions.strings);
    TEST_ASSERT_EQUAL(7, regions.length);
    TEST_ASSERT_EQUAL_STRING(
        "Kanto",
        regions.strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Kalos",
        regions.strings[regions.length-1]
    );

    pkmn_string_list_free(&regions);
    TEST_ASSERT_NULL(regions.strings);
    TEST_ASSERT_EQUAL(0, regions.length);
}

static void ribbon_list_test()
{
    // TODO
}

static void super_training_medal_list_test()
{
    pkmn_string_list_t super_training_medals =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_database_super_training_medal_list(
                &super_training_medals
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(super_training_medals.strings);
    TEST_ASSERT_EQUAL(30, super_training_medals.length);
    TEST_ASSERT_EQUAL_STRING(
        "Sp. Atk Level 1",
        super_training_medals.strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "The Battle for the Best!",
        super_training_medals.strings[super_training_medals.length-1]
    );

    pkmn_string_list_free(&super_training_medals);
    TEST_ASSERT_NULL(super_training_medals.strings);
    TEST_ASSERT_EQUAL(0, super_training_medals.length);
}

static void type_list_test()
{
    pkmn_string_list_t types =
    {
        .strings = NULL,
        .length = 0
    };
    pkmn_error_t error = PKMN_ERROR_NONE;

    // Make sure invalid calls return an error
    error = pkmn_database_type_list(
                "Not a game",
                &types
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_database_type_list(
                "Alpha Sapphire",
                &types
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(types.strings);
    TEST_ASSERT_EQUAL(18, types.length);
    TEST_ASSERT_EQUAL_STRING(
        "Normal",
        types.strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Fairy",
        types.strings[types.length-1]
    );

    pkmn_string_list_free(&types);
    TEST_ASSERT_NULL(types.strings);
    TEST_ASSERT_EQUAL(0, types.length);
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(ability_list_test)
    PKMN_C_TEST(game_list_test)
    PKMN_C_TEST(gamecube_shadow_pokemon_list_test)
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
