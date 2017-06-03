/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include "pokemon_test.h"

#include <pkmn.h>

static void gen2_pokemon_test(
    const char* species,
    const char* game
) {
    TEST_ASSERT_NOT_NULL(species);
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_handle_t pokemon = NULL;

    error = pkmn_pokemon_make(
                &pokemon,
                species,
                game,
                "",
                30
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(pokemon);

    pkmn_test_values_t test_values = {
        .valid_ball = "Great Ball",
        .invalid_balls = (char*[]){"Great Ball", NULL},

        .valid_item = "Berry",
        .invalid_items = (char*[]){"Razz Berry", "Bicycle", NULL},

        .expected_original_location = "Special",
        .valid_locations = (char*[]){"Sprout Tower", "Tohjo Falls", NULL},
        .invalid_locations = (char*[]){"Littleroot Town", "Petalburg Woods", NULL},

        .moves = (char*[]){"Slash", "Flamethrower", "Return", "Fire Blast", NULL},
        .invalid_moves = (char*[]){"Frenzy Plant", "Roost", NULL},

        .valid_original_games = (char*[]){"Gold", NULL},
        .invalid_original_games = (char*[]){"Gold", NULL}
    };

    pokemon_test_common(
        pokemon,
        &test_values
    );

    pkmn_gender_t gender = PKMN_GENDERLESS;
    int IV = 0;
    bool is_shiny = false;

    // Gender affects IVs, so make sure the abstraction reflects that.
    error = pkmn_pokemon_set_gender(
                pokemon,
                PKMN_MALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_IV(
                pokemon,
                "Attack",
                &IV
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(15, IV);

    error = pkmn_pokemon_set_IV(
                pokemon,
                "Attack",
                0
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_gender(
                pokemon,
                &gender
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_FEMALE, gender);
    error = pkmn_pokemon_set_IV(
                pokemon,
                "Attack",
                15
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_gender(
                pokemon,
                &gender
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_MALE, gender);

    // Shininess affects IVs, so make sure the abstraction reflects that.
    error = pkmn_pokemon_set_shininess(
                pokemon,
                false
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_is_shiny(
                pokemon,
                &is_shiny
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_FALSE(is_shiny);
    error = pkmn_pokemon_get_IV(
                pokemon,
                "Attack",
                &IV
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(13, IV);

    error = pkmn_pokemon_set_shininess(
                pokemon,
                true
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_is_shiny(
                pokemon,
                &is_shiny
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(is_shiny);
    static const char* stats[] = {
        "Attack", "Defense", "Speed", "Special"
    };
    for(int i = 0; i < 4; ++i) {
        error = pkmn_pokemon_get_IV(
                    pokemon,
                    stats[i],
                    &IV
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(((i == 0) ? 15 : 10), IV);
    }

    error = pkmn_pokemon_free(&pokemon);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

void gold_pokemon_test() {
    gen2_pokemon_test("Cyndaquil", "Gold");
}

void silver_pokemon_test() {
    gen2_pokemon_test("Totodile", "Silver");
}

void crystal_pokemon_test() {
    gen2_pokemon_test("Chikorita", "Crystal");
}
