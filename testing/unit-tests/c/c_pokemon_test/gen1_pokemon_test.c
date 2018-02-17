/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include "pokemon_test.h"

#include <pkmn.h>

static void gen1_pokemon_test(
    const char* species,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(species);
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_t pokemon =
    {
        .species = NULL,
        .game = NULL,
        ._internal = NULL
    };

    error = pkmn_pokemon_init(
                species,
                game,
                "",
                30,
                &pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(pokemon._internal);

    pkmn_test_values_t test_values =
    {
        .valid_ball = "Great Ball",
        .invalid_balls = (char*[]){"Great Ball", NULL},

        .valid_item = "Potion",
        .invalid_items = (char*[]){"Potion", NULL},

        .expected_original_location = "Special",
        .valid_locations = (char*[]){"Route 1", NULL},
        .invalid_locations = (char*[]){"Route 1", NULL},

        .moves = (char*[]){"Slash", "Flamethrower", "Tail Whip", "Fire Blast", NULL},
        .invalid_moves = (char*[]){"Return", NULL},

        .valid_original_games = (char*[]){"Red", NULL},
        .invalid_original_games = (char*[]){"Red", NULL}
    };

    pokemon_test_common(
        &pokemon,
        &test_values
    );

    error = pkmn_pokemon_free(&pokemon);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

void red_pokemon_test()
{
    gen1_pokemon_test("Charmander", "Red");
}

void blue_pokemon_test()
{
    gen1_pokemon_test("Squirtle", "Blue");
}

void yellow_pokemon_test()
{
    gen1_pokemon_test("Bulbasaur", "Yellow");
}
