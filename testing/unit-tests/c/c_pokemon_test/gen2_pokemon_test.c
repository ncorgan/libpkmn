/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include "pokemon_test.h"

#include <pkmn-c.h>

static void gen2_pokemon_test(
    enum pkmn_species species,
    enum pkmn_game game
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokemon pokemon =
    {
        .species = PKMN_SPECIES_NONE,
        .game = PKMN_GAME_NONE,
        .p_internal = NULL
    };

    error = pkmn_pokemon_init(
                species,
                game,
                "",
                30,
                &pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(pokemon.p_internal);

    pkmn_test_values_t test_values =
    {
        .valid_ball = PKMN_BALL_GREAT_BALL,
        .p_invalid_balls = (enum pkmn_ball[]){PKMN_BALL_GREAT_BALL, PKMN_BALL_NONE},

        .valid_item = PKMN_ITEM_BERRY,
        .p_invalid_items = (enum pkmn_item[]){
            PKMN_ITEM_RAZZ_BERRY,
            PKMN_ITEM_BICYCLE,
            PKMN_ITEM_NONE
        },

        .expected_original_location = "Special",
        .valid_locations = (char*[]){"Sprout Tower", "Tohjo Falls", NULL},
        .invalid_locations = (char*[]){"Littleroot Town", "Petalburg Woods", NULL},

        .p_moves = (enum pkmn_move[])
        {
            PKMN_MOVE_SLASH,
            PKMN_MOVE_FLAMETHROWER,
            PKMN_MOVE_RETURN,
            PKMN_MOVE_FIRE_BLAST,
            PKMN_MOVE_NONE
        },
        .p_invalid_moves = (enum pkmn_move[])
        {
            PKMN_MOVE_FRENZY_PLANT,
            PKMN_MOVE_ROOST,
            PKMN_MOVE_NONE
        },

        .valid_original_games = (enum pkmn_game[]){PKMN_GAME_GOLD, PKMN_GAME_NONE},
        .invalid_original_games = (enum pkmn_game[]){PKMN_GAME_GOLD, PKMN_GAME_NONE}
    };

    pokemon_test_common(
        &pokemon,
        &test_values
    );

    enum pkmn_gender gender = PKMN_GENDER_GENDERLESS;
    int IVs[PKMN_NUM_STATS] = {0};
    bool is_shiny = false;

    // Gender affects IVs, so make sure the abstraction reflects that.
    error = pkmn_pokemon_set_gender(
                &pokemon,
                PKMN_GENDER_MALE
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_get_IVs(
                &pokemon,
                IVs,
                PKMN_NUM_STATS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(15, IVs[PKMN_STAT_ATTACK]);

    error = pkmn_pokemon_set_IV(
                &pokemon,
                PKMN_STAT_ATTACK,
                0
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_get_gender(
                &pokemon,
                &gender
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, gender);
    error = pkmn_pokemon_set_IV(
                &pokemon,
                PKMN_STAT_ATTACK,
                15
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_get_gender(
                &pokemon,
                &gender
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, gender);

    // Shininess affects IVs, so make sure the abstraction reflects that.
    error = pkmn_pokemon_set_is_shiny(
                &pokemon,
                false
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_is_shiny(
                &pokemon,
                &is_shiny
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_FALSE(is_shiny);
    error = pkmn_pokemon_get_IVs(
                &pokemon,
                IVs,
                PKMN_NUM_STATS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(13, IVs[PKMN_STAT_ATTACK]);

    error = pkmn_pokemon_set_is_shiny(
                &pokemon,
                true
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_is_shiny(
                &pokemon,
                &is_shiny
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_TRUE(is_shiny);
    error = pkmn_pokemon_get_IVs(
                &pokemon,
                IVs,
                PKMN_NUM_STATS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL(15, IVs[PKMN_STAT_ATTACK]);
    TEST_ASSERT_EQUAL(10, IVs[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_EQUAL(10, IVs[PKMN_STAT_SPEED]);
    TEST_ASSERT_EQUAL(10, IVs[PKMN_STAT_SPECIAL]);

    error = pkmn_pokemon_free(&pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

void gold_pokemon_test()
{
    gen2_pokemon_test(PKMN_SPECIES_CYNDAQUIL, PKMN_GAME_GOLD);
}

void silver_pokemon_test()
{
    gen2_pokemon_test(PKMN_SPECIES_TOTODILE, PKMN_GAME_SILVER);
}

void crystal_pokemon_test()
{
    gen2_pokemon_test(PKMN_SPECIES_CHIKORITA, PKMN_GAME_CRYSTAL);
}
