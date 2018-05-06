/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn-c.h>

static void ability_list_test()
{
    struct pkmn_string_list abilities =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = PKMN_ERROR_NONE;

    // Make sure invalid calls return an error
    error = pkmn_database_ability_list(
                0,
                &abilities
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_database_ability_list(
                6, &abilities
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(abilities.pp_strings);
    TEST_ASSERT_EQUAL(191, abilities.length);
    TEST_ASSERT_EQUAL_STRING(
        "Adaptability",
        abilities.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING("Zen Mode", abilities.pp_strings[abilities.length-1]);

    pkmn_string_list_free(&abilities);
    TEST_ASSERT_NULL(abilities.pp_strings);
    TEST_ASSERT_EQUAL(0, abilities.length);
}

static void game_list_test()
{
    struct pkmn_string_list games =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = PKMN_ERROR_NONE;

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
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(games.pp_strings);
    TEST_ASSERT_EQUAL(26, games.length);
    TEST_ASSERT_EQUAL_STRING(
        "Red",
        games.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Alpha Sapphire",
        games.pp_strings[games.length-1]
    );

    pkmn_string_list_free(&games);
    TEST_ASSERT_NULL(games.pp_strings);
    TEST_ASSERT_EQUAL(0, games.length);
}

static void gamecube_shadow_pokemon_list_test()
{
    struct pkmn_string_list gamecube_shadow_pokemon =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_database_gamecube_shadow_pokemon_list(
                true,
                &gamecube_shadow_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(48, gamecube_shadow_pokemon.length);

    pkmn_string_list_free(&gamecube_shadow_pokemon);
    TEST_ASSERT_NULL(gamecube_shadow_pokemon.pp_strings);
    TEST_ASSERT_EQUAL(0, gamecube_shadow_pokemon.length);

    error = pkmn_database_gamecube_shadow_pokemon_list(
                false, &gamecube_shadow_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(83, gamecube_shadow_pokemon.length);

    pkmn_string_list_free(&gamecube_shadow_pokemon);
    TEST_ASSERT_NULL(gamecube_shadow_pokemon.pp_strings);
    TEST_ASSERT_EQUAL(0, gamecube_shadow_pokemon.length);
}

static void item_list_test()
{
    struct pkmn_string_list items =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = PKMN_ERROR_NONE;

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
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(items.pp_strings);
    TEST_ASSERT_EQUAL(513, items.length);
    TEST_ASSERT_EQUAL_STRING(
        "Adamant Orb",
        items.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Zoom Lens",
        items.pp_strings[items.length-1]
    );

    pkmn_string_list_free(&items);
    TEST_ASSERT_NULL(items.pp_strings);
    TEST_ASSERT_EQUAL(0, items.length);
}

static void location_list_test()
{
    struct pkmn_string_list locations =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = PKMN_ERROR_NONE;

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
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(locations.pp_strings);
    TEST_ASSERT_EQUAL(106, locations.length);
    TEST_ASSERT_EQUAL_STRING(
        "Abandoned Ship",
        locations.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Victory Road",
        locations.pp_strings[locations.length-1]
    );

    pkmn_string_list_free(&locations);
    TEST_ASSERT_NULL(locations.pp_strings);
    TEST_ASSERT_EQUAL(0, locations.length);
}

static void move_list_test()
{
    struct pkmn_string_list moves =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = PKMN_ERROR_NONE;

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
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(moves.pp_strings);
    TEST_ASSERT_EQUAL(165, moves.length);
    TEST_ASSERT_EQUAL_STRING(
        "Pound",
        moves.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Struggle",
        moves.pp_strings[moves.length-1]
    );

    pkmn_string_list_free(&moves);
    TEST_ASSERT_NULL(moves.pp_strings);
    TEST_ASSERT_EQUAL(0, moves.length);
}

static void nature_list_test()
{
    struct pkmn_string_list natures =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_database_nature_list(
                &natures
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(natures.pp_strings);
    TEST_ASSERT_EQUAL(25, natures.length);
    TEST_ASSERT_EQUAL_STRING(
        "Hardy",
        natures.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Quirky",
        natures.pp_strings[natures.length-1]
    );

    pkmn_string_list_free(&natures);
    TEST_ASSERT_NULL(natures.pp_strings);
    TEST_ASSERT_EQUAL(0, natures.length);
}

static void pokemon_list_test()
{
    struct pkmn_string_list pokemon =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = PKMN_ERROR_NONE;

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
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(pokemon.pp_strings);
    TEST_ASSERT_EQUAL(151, pokemon.length);
    TEST_ASSERT_EQUAL_STRING(
        "Bulbasaur",
        pokemon.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Mew",
        pokemon.pp_strings[pokemon.length-1]
    );

    pkmn_string_list_free(&pokemon);
    TEST_ASSERT_NULL(pokemon.pp_strings);
    TEST_ASSERT_EQUAL(0, pokemon.length);
}

static void region_list_test()
{
    struct pkmn_string_list regions =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_database_region_list(
                &regions
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(regions.pp_strings);
    TEST_ASSERT_EQUAL(7, regions.length);
    TEST_ASSERT_EQUAL_STRING(
        "Kanto",
        regions.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Kalos",
        regions.pp_strings[regions.length-1]
    );

    pkmn_string_list_free(&regions);
    TEST_ASSERT_NULL(regions.pp_strings);
    TEST_ASSERT_EQUAL(0, regions.length);
}

static void ribbon_list_test()
{
    // TODO
}

static void super_training_medal_list_test()
{
    struct pkmn_string_list super_training_medals =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_database_super_training_medal_list(
                &super_training_medals
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(super_training_medals.pp_strings);
    TEST_ASSERT_EQUAL(30, super_training_medals.length);
    TEST_ASSERT_EQUAL_STRING(
        "Sp. Atk Level 1",
        super_training_medals.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "The Battle for the Best!",
        super_training_medals.pp_strings[super_training_medals.length-1]
    );

    pkmn_string_list_free(&super_training_medals);
    TEST_ASSERT_NULL(super_training_medals.pp_strings);
    TEST_ASSERT_EQUAL(0, super_training_medals.length);
}

static void type_list_test()
{
    struct pkmn_string_list types =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = PKMN_ERROR_NONE;

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
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(types.pp_strings);
    TEST_ASSERT_EQUAL(18, types.length);
    TEST_ASSERT_EQUAL_STRING(
        "Normal",
        types.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Fairy",
        types.pp_strings[types.length-1]
    );

    pkmn_string_list_free(&types);
    TEST_ASSERT_NULL(types.pp_strings);
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
