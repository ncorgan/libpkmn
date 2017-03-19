/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <unity.h>

#include <pkmn-c/pokemon.h>

static void gender_test(
    const char* game
) {
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_gender_t gender = PKMN_GENDERLESS;

    // Single-gender
    pkmn_pokemon_handle_t nidorina = NULL;
    error = pkmn_pokemon_make(
                &nidorina,
                "Nidorina",
                game,
                "",
                50
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(nidorina);

    error = pkmn_pokemon_get_gender(
                nidorina,
                &gender
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_FEMALE, gender);

    error = pkmn_pokemon_set_gender(
                nidorina,
                PKMN_FEMALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_set_gender(
                nidorina,
                PKMN_MALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_gender(
                nidorina,
                PKMN_GENDERLESS
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_pokemon_free(&nidorina);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(nidorina);

    pkmn_pokemon_handle_t nidorino = NULL;
    error = pkmn_pokemon_make(
                &nidorino,
                "Nidorino",
                game,
                "",
                50
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(nidorino);

    error = pkmn_pokemon_get_gender(
                nidorino,
                &gender
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_MALE, gender);

    error = pkmn_pokemon_set_gender(
                nidorino,
                PKMN_MALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_set_gender(
                nidorino,
                PKMN_FEMALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_gender(
                nidorino,
                PKMN_GENDERLESS
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_pokemon_free(&nidorino);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(nidorino);

    pkmn_pokemon_handle_t magnemite = NULL;
    error = pkmn_pokemon_make(
                &magnemite,
                "Magnemite",
                game,
                "",
                50
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(magnemite);

    error = pkmn_pokemon_get_gender(
                magnemite,
                &gender
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_GENDERLESS, gender);

    error = pkmn_pokemon_set_gender(
                magnemite,
                PKMN_GENDERLESS
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_set_gender(
                magnemite,
                PKMN_MALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_gender(
                magnemite,
                PKMN_FEMALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_pokemon_free(&magnemite);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(magnemite);

    static const char* mixed_pokemon[] = {
        "Charmander", // 87.% male
        "Growlithe",  // 75% male
        "Pidgey",     // 50% male
        "Vulpix",     // 25% male
        NULL
    };

    for(size_t i = 0; mixed_pokemon[i] != NULL; ++i) {
        pkmn_pokemon_handle_t pokemon = NULL;
        error = pkmn_pokemon_make(
                    &pokemon,
                    mixed_pokemon[i],
                    game,
                    "",
                    50
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_get_gender(
                    pokemon,
                    &gender
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_EQUAL(PKMN_GENDERLESS, gender);

        error = pkmn_pokemon_set_gender(
                    pokemon,
                    PKMN_MALE
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_gender(
                    pokemon,
                    &gender
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(PKMN_MALE, gender);

        error = pkmn_pokemon_set_gender(
                    pokemon,
                    PKMN_FEMALE
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_gender(
                    pokemon,
                    &gender
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(PKMN_FEMALE, gender);

        error = pkmn_pokemon_set_gender(
                    pokemon,
                    PKMN_GENDERLESS
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

        error = pkmn_pokemon_free(&pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(pokemon);
    }
}

void gold_gender_test() {
    gender_test("Gold");
}

void silver_gender_test() {
    gender_test("Silver");
}

void crystal_gender_test() {
    gender_test("Crystal");
}

void ruby_gender_test() {
    gender_test("Ruby");
}

void sapphire_gender_test() {
    gender_test("Sapphire");
}

void emerald_gender_test() {
    gender_test("Emerald");
}

void firered_gender_test() {
    gender_test("FireRed");
}

void leafgreen_gender_test() {
    gender_test("LeafGreen");
}
