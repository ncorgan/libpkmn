/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <unity.h>

#include <pkmn-c.h>

static const pkmn_pokemon_t empty_pokemon =
{
    .species = NULL,
    .game = NULL,
    ._internal = NULL
};

static void gender_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_gender_t gender = PKMN_GENDER_GENDERLESS;

    // Single-gender
    pkmn_pokemon_t nidorina = empty_pokemon;
    error = pkmn_pokemon_init(
                "Nidorina",
                game,
                "",
                50,
                &nidorina
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(nidorina._internal);

    error = pkmn_pokemon_get_gender(
                &nidorina,
                &gender
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, gender);

    error = pkmn_pokemon_set_gender(
                &nidorina,
                PKMN_GENDER_FEMALE
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_set_gender(
                &nidorina,
                PKMN_GENDER_MALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_gender(
                &nidorina,
                PKMN_GENDER_GENDERLESS
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_pokemon_free(&nidorina);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(nidorina._internal);

    pkmn_pokemon_t nidorino = empty_pokemon;
    error = pkmn_pokemon_init(
                "Nidorino",
                game,
                "",
                50,
                &nidorino
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(nidorino._internal);

    error = pkmn_pokemon_get_gender(
                &nidorino,
                &gender
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, gender);

    error = pkmn_pokemon_set_gender(
                &nidorino,
                PKMN_GENDER_MALE
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_set_gender(
                &nidorino,
                PKMN_GENDER_FEMALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_gender(
                &nidorino,
                PKMN_GENDER_GENDERLESS
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_pokemon_free(&nidorino);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(nidorino._internal);

    pkmn_pokemon_t magnemite = empty_pokemon;
    error = pkmn_pokemon_init(
                "Magnemite",
                game,
                "",
                50,
                &magnemite
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(magnemite._internal);

    error = pkmn_pokemon_get_gender(
                &magnemite,
                &gender
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_GENDERLESS, gender);

    error = pkmn_pokemon_set_gender(
                &magnemite,
                PKMN_GENDER_GENDERLESS
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_set_gender(
                &magnemite,
                PKMN_GENDER_MALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_gender(
                &magnemite,
                PKMN_GENDER_FEMALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_pokemon_free(&magnemite);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(magnemite._internal);

    static const char* mixed_pokemon[] =
    {
        "Charmander", // 87.% male
        "Growlithe",  // 75% male
        "Pidgey",     // 50% male
        "Vulpix",     // 25% male
        NULL
    };

    for(size_t pokemon_index = 0;
        mixed_pokemon[pokemon_index];
        ++pokemon_index)
    {
        pkmn_pokemon_t pokemon = empty_pokemon;
        error = pkmn_pokemon_init(
                    mixed_pokemon[pokemon_index],
                    game,
                    "",
                    50,
                    &pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_get_gender(
                    &pokemon,
                    &gender
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_EQUAL(PKMN_GENDER_GENDERLESS, gender);

        error = pkmn_pokemon_set_gender(
                    &pokemon,
                    PKMN_GENDER_MALE
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_get_gender(
                    &pokemon,
                    &gender
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, gender);

        error = pkmn_pokemon_set_gender(
                    &pokemon,
                    PKMN_GENDER_FEMALE
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_get_gender(
                    &pokemon,
                    &gender
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, gender);

        error = pkmn_pokemon_set_gender(
                    &pokemon,
                    PKMN_GENDER_GENDERLESS
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(pokemon._internal);
    }
}

void gold_gender_test()
{
    gender_test("Gold");
}

void silver_gender_test()
{
    gender_test("Silver");
}

void crystal_gender_test()
{
    gender_test("Crystal");
}

void ruby_gender_test()
{
    gender_test("Ruby");
}

void sapphire_gender_test()
{
    gender_test("Sapphire");
}

void emerald_gender_test()
{
    gender_test("Emerald");
}

void firered_gender_test()
{
    gender_test("FireRed");
}

void leafgreen_gender_test()
{
    gender_test("LeafGreen");
}

void colosseum_gender_test()
{
    gender_test("Colosseum");
}

void xd_gender_test()
{
    gender_test("XD");
}
