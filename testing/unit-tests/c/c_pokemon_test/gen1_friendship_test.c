/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest-c/util.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/database/move_entry.h>

#include <unity.h>

static void gen1_friendship_test(
    const char* game
) {
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_handle_t pokemon = NULL;
    int friendship = 0;

    error = pkmn_pokemon_make(
                &pokemon,
                "Pikachu",
                game,
                "",
                5
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_set_friendship(
                pokemon,
                123
            );
    if(!strcmp(game, "Yellow")) {
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_friendship(
                    pokemon,
                    &friendship
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(123, friendship);
    } else {
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_get_friendship(
                    pokemon,
                    &friendship
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }

    error = pkmn_pokemon_free(&pokemon);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pokemon);

    // For Yellow, also check a non-Pikachu
    if(!strcmp(game, "Yellow")) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    "Mewtwo",
                    game,
                    "",
                    70
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_set_friendship(
                    pokemon,
                    123
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_get_friendship(
                    pokemon,
                    &friendship
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_free(&pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(pokemon);
    }
}

void red_friendship_test() {
    gen1_friendship_test("Red");
}

void blue_friendship_test() {
    gen1_friendship_test("Blue");
}

void yellow_friendship_test() {
    gen1_friendship_test("Yellow");
}
