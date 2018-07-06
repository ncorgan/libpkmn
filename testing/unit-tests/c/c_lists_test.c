/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn-c.h>

static void ability_list_test()
{
    const size_t EXPECTED_NUM_ABILITIES = 191;
    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_ability_enum_list abilities =
    {
        .p_enums = NULL,
        .length = 0
    };
    struct pkmn_string_list ability_names =
    {
        .pp_strings = NULL,
        .length = 0
    };

    // Make sure invalid calls return an error.
    error = pkmn_database_ability_list(
                0,
                &abilities
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_database_ability_name_list(
                0,
                &ability_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Enums

    error = pkmn_database_ability_list(
                6,
                &abilities
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(abilities.p_enums);
    TEST_ASSERT_EQUAL(
        EXPECTED_NUM_ABILITIES,
        abilities.length
    );

    TEST_ASSERT_EQUAL(
        PKMN_ABILITY_STENCH,
        abilities.p_enums[0]
    );
    TEST_ASSERT_EQUAL(
        PKMN_ABILITY_DELTA_STREAM,
        abilities.p_enums[abilities.length-1]
    );

    // Strings

    error = pkmn_database_ability_name_list(
                6,
                &ability_names
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(ability_names.pp_strings);
    TEST_ASSERT_EQUAL(
        EXPECTED_NUM_ABILITIES,
        ability_names.length
    );

    TEST_ASSERT_EQUAL_STRING(
        "Stench",
        ability_names.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Delta Stream",
        ability_names.pp_strings[ability_names.length-1]
    );

    // Make sure lists match.

    TEST_ASSERT_EQUAL(abilities.length, ability_names.length);
    for(size_t ability_index = 0;
        ability_index < abilities.length;
        ++ability_index)
    {
        enum pkmn_ability converted_ability = PKMN_ABILITY_NONE;

        error = pkmn_string_to_ability(
                    ability_names.pp_strings[ability_index],
                    &converted_ability
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL_MESSAGE(
            abilities.p_enums[ability_index],
            converted_ability,
            ability_names.pp_strings[ability_index]
        );
    }

    // Clean up.

    pkmn_string_list_free(&ability_names);
    TEST_ASSERT_NULL(ability_names.pp_strings);
    TEST_ASSERT_EQUAL(0, ability_names.length);

    pkmn_ability_enum_list_free(&abilities);
    TEST_ASSERT_NULL(abilities.p_enums);
    TEST_ASSERT_EQUAL(0, abilities.length);
}

static void game_list_test()
{
    const size_t EXPECTED_NUM_GAMES = 26;
    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_game_enum_list games =
    {
        .p_enums = NULL,
        .length = 0
    };
    struct pkmn_string_list game_names =
    {
        .pp_strings = NULL,
        .length = 0
    };

    // Make sure invalid calls return an error.
    error = pkmn_database_game_list(
                     7,
                     true,
                     &games
                 );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_database_game_name_list(
                     7,
                     true,
                     &game_names
                 );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Enums

    error = pkmn_database_game_list(
                6,
                true,
                &games
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(games.p_enums);
    TEST_ASSERT_EQUAL(EXPECTED_NUM_GAMES, games.length);
    TEST_ASSERT_EQUAL(
        PKMN_GAME_RED,
        games.p_enums[0]
    );
    TEST_ASSERT_EQUAL(
        PKMN_GAME_ALPHA_SAPPHIRE,
        games.p_enums[games.length-1]
    );

    // Strings

    error = pkmn_database_game_name_list(
                6,
                true,
                &game_names
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(game_names.pp_strings);
    TEST_ASSERT_EQUAL(EXPECTED_NUM_GAMES, game_names.length);
    TEST_ASSERT_EQUAL_STRING(
        "Red",
        game_names.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Alpha Sapphire",
        game_names.pp_strings[game_names.length-1]
    );

    // Make sure lists match.

    TEST_ASSERT_EQUAL(games.length, game_names.length);
    for(size_t game_index = 0;
        game_index < games.length;
        ++game_index)
    {
        enum pkmn_game converted_game = PKMN_GAME_NONE;

        error = pkmn_string_to_game(
                    game_names.pp_strings[game_index],
                    &converted_game
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL_MESSAGE(
            games.p_enums[game_index],
            converted_game,
            game_names.pp_strings[game_index]
        );
    }

    // Clean up.

    pkmn_string_list_free(&game_names);
    TEST_ASSERT_NULL(game_names.pp_strings);
    TEST_ASSERT_EQUAL(0, game_names.length);

    pkmn_game_enum_list_free(&games);
    TEST_ASSERT_NULL(games.p_enums);
    TEST_ASSERT_EQUAL(0, games.length);
}

static void gamecube_shadow_pokemon_list_test()
{
    const size_t EXPECTED_NUM_COLOSSEUM_POKEMON = 48;
    const size_t EXPECTED_NUM_XD_POKEMON = 83;
    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_species_enum_list gamecube_shadow_pokemon =
    {
        .p_enums = NULL,
        .length = 0
    };
    struct pkmn_string_list gamecube_shadow_pokemon_names =
    {
        .pp_strings = NULL,
        .length = 0
    };

    // Enums

    error = pkmn_database_gamecube_shadow_pokemon_list(
                true,
                &gamecube_shadow_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(gamecube_shadow_pokemon.p_enums);
    TEST_ASSERT_EQUAL(
        EXPECTED_NUM_COLOSSEUM_POKEMON,
        gamecube_shadow_pokemon.length
    );

    pkmn_species_enum_list_free(&gamecube_shadow_pokemon);
    TEST_ASSERT_NULL(gamecube_shadow_pokemon.p_enums);
    TEST_ASSERT_EQUAL(0, gamecube_shadow_pokemon.length);

    error = pkmn_database_gamecube_shadow_pokemon_list(
                false,
                &gamecube_shadow_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(gamecube_shadow_pokemon.p_enums);
    TEST_ASSERT_EQUAL(
        EXPECTED_NUM_XD_POKEMON,
        gamecube_shadow_pokemon.length
    );

    // Strings

    error = pkmn_database_gamecube_shadow_pokemon_name_list(
                true,
                &gamecube_shadow_pokemon_names
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(gamecube_shadow_pokemon_names.pp_strings);
    TEST_ASSERT_EQUAL(
        EXPECTED_NUM_COLOSSEUM_POKEMON,
        gamecube_shadow_pokemon_names.length
    );

    pkmn_string_list_free(&gamecube_shadow_pokemon_names);
    TEST_ASSERT_NULL(gamecube_shadow_pokemon_names.pp_strings);
    TEST_ASSERT_EQUAL(0, gamecube_shadow_pokemon_names.length);

    error = pkmn_database_gamecube_shadow_pokemon_name_list(
                false,
                &gamecube_shadow_pokemon_names
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(gamecube_shadow_pokemon_names.pp_strings);
    TEST_ASSERT_EQUAL(
        EXPECTED_NUM_XD_POKEMON,
        gamecube_shadow_pokemon_names.length
    );

    // Make sure lists match.

    TEST_ASSERT_EQUAL(
        gamecube_shadow_pokemon.length,
        gamecube_shadow_pokemon_names.length
    );
    for(size_t gamecube_shadow_pokemon_index = 0;
        gamecube_shadow_pokemon_index < gamecube_shadow_pokemon.length;
        ++gamecube_shadow_pokemon_index)
    {
        enum pkmn_species converted_species = PKMN_SPECIES_NONE;

        error = pkmn_string_to_species(
                    gamecube_shadow_pokemon_names.pp_strings[gamecube_shadow_pokemon_index],
                    &converted_species
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL_MESSAGE(
            gamecube_shadow_pokemon.p_enums[gamecube_shadow_pokemon_index],
            converted_species,
            gamecube_shadow_pokemon_names.pp_strings[gamecube_shadow_pokemon_index]
        );
    }

    // Clean up.

    pkmn_string_list_free(&gamecube_shadow_pokemon_names);
    TEST_ASSERT_NULL(gamecube_shadow_pokemon_names.pp_strings);
    TEST_ASSERT_EQUAL(0, gamecube_shadow_pokemon_names.length);

    pkmn_species_enum_list_free(&gamecube_shadow_pokemon);
    TEST_ASSERT_NULL(gamecube_shadow_pokemon.p_enums);
    TEST_ASSERT_EQUAL(0, gamecube_shadow_pokemon.length);
}

static void hm_move_list_test()
{
    const size_t EXPECTED_NUM_HM_MOVES = 5;
    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_move_enum_list hm_moves =
    {
        .p_enums = NULL,
        .length = 0
    };
    struct pkmn_string_list hm_move_names =
    {
        .pp_strings = NULL,
        .length = 0
    };

    // Make sure invalid calls return an error.
    error = pkmn_database_hm_move_list(
                PKMN_GAME_NONE,
                &hm_moves
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_database_hm_move_name_list(
                PKMN_GAME_NONE,
                &hm_move_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    // Enums

    error = pkmn_database_hm_move_list(
                PKMN_GAME_RED,
                &hm_moves
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(hm_moves.p_enums);
    TEST_ASSERT_EQUAL(
        EXPECTED_NUM_HM_MOVES,
        hm_moves.length
    );

    TEST_ASSERT_EQUAL(
        PKMN_MOVE_CUT,
        hm_moves.p_enums[0]
    );
    TEST_ASSERT_EQUAL(
        PKMN_MOVE_FLASH,
        hm_moves.p_enums[hm_moves.length-1]
    );

    pkmn_move_enum_list_free(&hm_moves);
    TEST_ASSERT_NULL(hm_moves.p_enums);
    TEST_ASSERT_EQUAL(0, hm_moves.length);

    // Strings

    error = pkmn_database_hm_move_name_list(
                PKMN_GAME_RED,
                &hm_move_names
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(hm_move_names.pp_strings);
    TEST_ASSERT_EQUAL(
        EXPECTED_NUM_HM_MOVES,
        hm_move_names.length
    );

    TEST_ASSERT_EQUAL_STRING(
        "Cut",
        hm_move_names.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Flash",
        hm_move_names.pp_strings[hm_move_names.length-1]
    );

    pkmn_string_list_free(&hm_move_names);
    TEST_ASSERT_NULL(hm_move_names.pp_strings);
    TEST_ASSERT_EQUAL(0, hm_move_names.length);
}

static void item_list_test()
{
    const size_t EXPECTED_NUM_ITEMS = 513;
    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_item_enum_list items =
    {
        .p_enums = NULL,
        .length = 0
    };
    struct pkmn_string_list item_names =
    {
        .pp_strings = NULL,
        .length = 0
    };

    // Make sure invalid calls return an error.
    error = pkmn_database_item_list(
                PKMN_GAME_NONE,
                &items
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_database_item_name_list(
                PKMN_GAME_NONE,
                &item_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    // Enums

    error = pkmn_database_item_list(
                PKMN_GAME_HEARTGOLD,
                &items
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(items.p_enums);
    TEST_ASSERT_EQUAL(
        EXPECTED_NUM_ITEMS,
        items.length
    );

    TEST_ASSERT_EQUAL(
        PKMN_ITEM_MASTER_BALL,
        items.p_enums[0]
    );
    TEST_ASSERT_EQUAL(
        PKMN_ITEM_PHOTO_ALBUM,
        items.p_enums[items.length-1]
    );

    pkmn_item_enum_list_free(&items);
    TEST_ASSERT_NULL(items.p_enums);
    TEST_ASSERT_EQUAL(0, items.length);

    // Strings

    error = pkmn_database_item_name_list(
                PKMN_GAME_HEARTGOLD,
                &item_names
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(item_names.pp_strings);
    TEST_ASSERT_EQUAL(
        EXPECTED_NUM_ITEMS,
        item_names.length
    );

    TEST_ASSERT_EQUAL_STRING(
        "Master Ball",
        item_names.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Photo Album",
        item_names.pp_strings[item_names.length-1]
    );

    pkmn_string_list_free(&item_names);
    TEST_ASSERT_NULL(item_names.pp_strings);
    TEST_ASSERT_EQUAL(0, item_names.length);
}

static void location_list_test()
{
    struct pkmn_string_list location_names =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = PKMN_ERROR_NONE;

    // Make sure invalid calls return an error
    error = pkmn_database_location_name_list(
                PKMN_GAME_NONE, false,
                &location_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_database_location_name_list(
                     PKMN_GAME_EMERALD,
                     false,
                     &location_names
                 );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(location_names.pp_strings);
    TEST_ASSERT_EQUAL(106, location_names.length);
    TEST_ASSERT_EQUAL_STRING(
        "Abandoned Ship",
        location_names.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Victory Road",
        location_names.pp_strings[location_names.length-1]
    );

    pkmn_string_list_free(&location_names);
    TEST_ASSERT_NULL(location_names.pp_strings);
    TEST_ASSERT_EQUAL(0, location_names.length);
}

static void move_list_test()
{
    const size_t EXPECTED_NUM_MOVES = 165;
    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_move_enum_list moves =
    {
        .p_enums = NULL,
        .length = 0
    };
    struct pkmn_string_list move_names =
    {
        .pp_strings = NULL,
        .length = 0
    };

    // Make sure invalid calls return an error.
    error = pkmn_database_move_list(
                PKMN_GAME_NONE,
                &moves
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_database_move_name_list(
                PKMN_GAME_NONE,
                &move_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    // Enums

    error = pkmn_database_move_list(
                PKMN_GAME_RED,
                &moves
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(moves.p_enums);
    TEST_ASSERT_EQUAL(
        EXPECTED_NUM_MOVES,
        moves.length
    );

    TEST_ASSERT_EQUAL(
        PKMN_MOVE_POUND,
        moves.p_enums[0]
    );
    TEST_ASSERT_EQUAL(
        PKMN_MOVE_STRUGGLE,
        moves.p_enums[moves.length-1]
    );

    pkmn_move_enum_list_free(&moves);
    TEST_ASSERT_NULL(moves.p_enums);
    TEST_ASSERT_EQUAL(0, moves.length);

    // Strings

    error = pkmn_database_move_name_list(
                PKMN_GAME_RED,
                &move_names
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(move_names.pp_strings);
    TEST_ASSERT_EQUAL(
        EXPECTED_NUM_MOVES,
        move_names.length
    );

    TEST_ASSERT_EQUAL_STRING(
        "Pound",
        move_names.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Struggle",
        move_names.pp_strings[move_names.length-1]
    );

    pkmn_string_list_free(&move_names);
    TEST_ASSERT_NULL(move_names.pp_strings);
    TEST_ASSERT_EQUAL(0, move_names.length);
}

static void nature_list_test()
{
    const size_t EXPECTED_NUM_NATURES = 25;
    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_nature_enum_list natures =
    {
        .p_enums = NULL,
        .length = 0
    };
    struct pkmn_string_list nature_names =
    {
        .pp_strings = NULL,
        .length = 0
    };

    // Enums

    error = pkmn_database_nature_list(&natures);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(natures.p_enums);
    TEST_ASSERT_EQUAL(
        EXPECTED_NUM_NATURES,
        natures.length
    );

    TEST_ASSERT_EQUAL(
        PKMN_NATURE_HARDY,
        natures.p_enums[0]
    );
    TEST_ASSERT_EQUAL(
        PKMN_NATURE_QUIRKY,
        natures.p_enums[natures.length-1]
    );

    pkmn_nature_enum_list_free(&natures);
    TEST_ASSERT_NULL(natures.p_enums);
    TEST_ASSERT_EQUAL(0, natures.length);

    // Strings

    error = pkmn_database_nature_name_list(&nature_names);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(nature_names.pp_strings);
    TEST_ASSERT_EQUAL(
        EXPECTED_NUM_NATURES,
        nature_names.length
    );

    TEST_ASSERT_EQUAL_STRING(
        "Hardy",
        nature_names.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Quirky",
        nature_names.pp_strings[nature_names.length-1]
    );

    pkmn_string_list_free(&nature_names);
    TEST_ASSERT_NULL(nature_names.pp_strings);
    TEST_ASSERT_EQUAL(0, nature_names.length);
}

static void pokemon_list_test()
{
    struct pkmn_string_list pokemon =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_database_pokemon_name_list(
                0,
                true,
                &pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_database_pokemon_name_list(
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

    error = pkmn_database_region_name_list(
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

    error = pkmn_database_super_training_medal_name_list(
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

static void tm_move_list_test()
{
    struct pkmn_string_list tm_moves =
    {
        .pp_strings = NULL,
        .length = 0
    };
    enum pkmn_error error = PKMN_ERROR_NONE;

    // Make sure invalid calls return an error
    error = pkmn_database_tm_move_name_list(
                PKMN_GAME_NONE,
                &tm_moves
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_database_tm_move_name_list(
                PKMN_GAME_RED,
                &tm_moves
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(tm_moves.pp_strings);
    TEST_ASSERT_EQUAL(50, tm_moves.length);

    TEST_ASSERT_EQUAL_STRING(
        "Mega Punch",
        tm_moves.pp_strings[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Substitute",
        tm_moves.pp_strings[49]
    );

    pkmn_string_list_free(&tm_moves);
    TEST_ASSERT_NULL(tm_moves.pp_strings);
    TEST_ASSERT_EQUAL(0, tm_moves.length);
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
    error = pkmn_database_type_name_list(
                PKMN_GAME_NONE,
                &types
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_database_type_name_list(
                PKMN_GAME_ALPHA_SAPPHIRE,
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
    PKMN_C_TEST(hm_move_list_test)
    PKMN_C_TEST(item_list_test)
    PKMN_C_TEST(location_list_test)
    PKMN_C_TEST(move_list_test)
    PKMN_C_TEST(nature_list_test)
    PKMN_C_TEST(pokemon_list_test)
    PKMN_C_TEST(region_list_test)
    PKMN_C_TEST(ribbon_list_test)
    PKMN_C_TEST(super_training_medal_list_test)
    PKMN_C_TEST(tm_move_list_test)
    PKMN_C_TEST(type_list_test)
)
