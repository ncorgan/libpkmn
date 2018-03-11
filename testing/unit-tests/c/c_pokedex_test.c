/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"
#include "pkmntest-c/util.h"

#include <pkmn-c.h>

#include <stdlib.h>
#include <time.h>

static void pokedex_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokedex_t pokedex =
    {
        .game = NULL,
        ._internal = NULL
    };

    int generation = game_to_generation(game);

    error = pkmn_pokedex_init(
                game,
                &pokedex
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING(game, pokedex.game);
    TEST_ASSERT_NOT_NULL(pokedex._internal);

    // Check initial values.

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_pokedex_strerror(&pokedex)
    );

    size_t num_seen = 0;
    error = pkmn_pokedex_get_num_seen(
                &pokedex,
                &num_seen
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(0, num_seen);

    size_t num_caught = 0;
    error = pkmn_pokedex_get_num_caught(
                &pokedex,
                &num_caught
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(0, num_caught);

    // Set random Pokémon to be seen and caught.
    pkmn_string_list_t all_pokemon =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_database_pokemon_list(generation, true, &all_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(all_pokemon.strings);

    num_seen = (size_t)((rand() % (int)all_pokemon.length/2 + 1));
    num_caught = (size_t)((rand() % (int)all_pokemon.length/2 + 1));

    for(size_t pokemon_index = 0; pokemon_index < num_seen; ++pokemon_index)
    {
        error = pkmn_pokedex_set_has_seen(
                    &pokedex,
                    all_pokemon.strings[pokemon_index],
                    true
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        bool has_seen = false;
        error = pkmn_pokedex_has_seen(
                    &pokedex,
                    all_pokemon.strings[pokemon_index],
                    &has_seen
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_TRUE(has_seen);
    }

    size_t num_seen_after = 0;
    error = pkmn_pokedex_get_num_seen(
                &pokedex,
                &num_seen_after
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(num_seen, num_seen_after);

    for(size_t pokemon_index = (all_pokemon.length-num_caught); pokemon_index < all_pokemon.length; ++pokemon_index)
    {
        error = pkmn_pokedex_set_has_caught(
                    &pokedex,
                    all_pokemon.strings[pokemon_index],
                    true
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        bool has_caught = false;
        error = pkmn_pokedex_has_caught(
                    &pokedex,
                    all_pokemon.strings[pokemon_index],
                    &has_caught
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_TRUE(has_caught);
    }

    size_t num_caught_after = 0;
    error = pkmn_pokedex_get_num_caught(
                &pokedex,
                &num_caught_after
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(num_caught, num_caught_after);

    // Remove all seen.

    pkmn_string_list_t all_seen =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_pokedex_get_all_seen(
                &pokedex,
                &all_seen
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(all_seen.strings);

    for(size_t pokemon_index = 0; pokemon_index < all_seen.length; ++pokemon_index)
    {
        error = pkmn_pokedex_set_has_seen(
                    &pokedex,
                    all_seen.strings[pokemon_index],
                    false
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        bool has_seen = true;
        error = pkmn_pokedex_has_seen(
                    &pokedex,
                    all_seen.strings[pokemon_index],
                    &has_seen
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_FALSE(has_seen);
    }

    num_seen_after = 0;
    error = pkmn_pokedex_get_num_seen(
                &pokedex,
                &num_seen_after
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(0, num_seen_after);

    error = pkmn_string_list_free(&all_seen);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(all_seen.strings);

    // Remove all caught.

    pkmn_string_list_t all_caught =
    {
        .strings = NULL,
        .length = 0
    };
    error = pkmn_pokedex_get_all_caught(
                &pokedex,
                &all_caught
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(all_caught.strings);

    for(size_t pokemon_index = 0; pokemon_index < all_caught.length; ++pokemon_index)
    {
        error = pkmn_pokedex_set_has_caught(
                    &pokedex,
                    all_caught.strings[pokemon_index],
                    false
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        bool has_caught = true;
        error = pkmn_pokedex_has_caught(
                    &pokedex,
                    all_caught.strings[pokemon_index],
                    &has_caught
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_FALSE(has_caught);
    }

    num_caught_after = 0;
    error = pkmn_pokedex_get_num_caught(
                &pokedex,
                &num_caught_after
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(0, num_caught_after);

    error = pkmn_string_list_free(&all_caught);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(all_caught.strings);

    error = pkmn_string_list_free(&all_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(all_pokemon.strings);

    error = pkmn_pokedex_free(&pokedex);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(pokedex.game);
    TEST_ASSERT_NULL(pokedex._internal);
}

#define POKEDEX_TEST_FCN(game) \
static void game ## _test_pokedex() \
{ \
    pokedex_test(#game); \
}

POKEDEX_TEST_FCN(Red)
POKEDEX_TEST_FCN(Blue)
POKEDEX_TEST_FCN(Yellow)

POKEDEX_TEST_FCN(Gold)
POKEDEX_TEST_FCN(Silver)
POKEDEX_TEST_FCN(Crystal)

POKEDEX_TEST_FCN(Ruby)
POKEDEX_TEST_FCN(Sapphire)
POKEDEX_TEST_FCN(Emerald)
POKEDEX_TEST_FCN(FireRed)
POKEDEX_TEST_FCN(LeafGreen)

PKMN_C_TEST_MAIN(
    srand((unsigned int)time(0));

    PKMN_C_TEST(Red_test_pokedex)
    PKMN_C_TEST(Blue_test_pokedex)
    PKMN_C_TEST(Yellow_test_pokedex)

    PKMN_C_TEST(Gold_test_pokedex)
    PKMN_C_TEST(Silver_test_pokedex)
    PKMN_C_TEST(Crystal_test_pokedex)

    PKMN_C_TEST(Ruby_test_pokedex)
    PKMN_C_TEST(Sapphire_test_pokedex)
    PKMN_C_TEST(Emerald_test_pokedex)
    PKMN_C_TEST(FireRed_test_pokedex)
    PKMN_C_TEST(LeafGreen_test_pokedex)
)
