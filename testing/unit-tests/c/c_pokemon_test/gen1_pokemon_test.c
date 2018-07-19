/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include "pokemon_test.h"

#include <pkmn-c.h>

static void gen1_pokemon_test(
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

        .valid_item = PKMN_ITEM_POTION,
        .p_invalid_items = (enum pkmn_item[]){PKMN_ITEM_POTION, PKMN_ITEM_NONE},

        .expected_original_location = "Special",
        .valid_locations = (char*[]){"Route 1", NULL},
        .invalid_locations = (char*[]){"Route 1", NULL},

        .moves = (char*[]){"Slash", "Flamethrower", "Tail Whip", "Fire Blast", NULL},
        .invalid_moves = (char*[]){"Return", NULL},

        .valid_original_games = (enum pkmn_game[]){PKMN_GAME_RED, PKMN_GAME_NONE},
        .invalid_original_games = (enum pkmn_game[]){PKMN_GAME_RED, PKMN_GAME_NONE}
    };

    pokemon_test_common(
        &pokemon,
        &test_values
    );

    // Make sure we can read the relevant attributes.
    int catch_rate = 0;
    error = pkmn_pokemon_get_numeric_attribute(
                &pokemon,
                "Catch rate",
                &catch_rate
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_pokemon_free(&pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

void red_pokemon_test()
{
    gen1_pokemon_test(PKMN_SPECIES_CHARMANDER, PKMN_GAME_RED);
}

void blue_pokemon_test()
{
    gen1_pokemon_test(PKMN_SPECIES_SQUIRTLE, PKMN_GAME_BLUE);
}

void yellow_pokemon_test()
{
    gen1_pokemon_test(PKMN_SPECIES_BULBASAUR, PKMN_GAME_YELLOW);
}
