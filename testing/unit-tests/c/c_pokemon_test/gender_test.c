/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <unity.h>

#include <pkmn.h>

static const pkmn_pokemon2_t empty_pokemon =
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
    pkmn_pokemon2_t nidorina = empty_pokemon;
    error = pkmn_pokemon2_init(
                "Nidorina",
                game,
                "",
                50,
                &nidorina
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(nidorina._internal);

    error = pkmn_pokemon2_get_gender(
                &nidorina,
                &gender
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, gender);

    error = pkmn_pokemon2_set_gender(
                &nidorina,
                PKMN_GENDER_FEMALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon2_set_gender(
                &nidorina,
                PKMN_GENDER_MALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon2_set_gender(
                &nidorina,
                PKMN_GENDER_GENDERLESS
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_pokemon2_free(&nidorina);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(nidorina._internal);

    pkmn_pokemon2_t nidorino = empty_pokemon;
    error = pkmn_pokemon2_init(
                "Nidorino",
                game,
                "",
                50,
                &nidorino
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(nidorino._internal);

    error = pkmn_pokemon2_get_gender(
                &nidorino,
                &gender
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, gender);

    error = pkmn_pokemon2_set_gender(
                &nidorino,
                PKMN_GENDER_MALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon2_set_gender(
                &nidorino,
                PKMN_GENDER_FEMALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon2_set_gender(
                &nidorino,
                PKMN_GENDER_GENDERLESS
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_pokemon2_free(&nidorino);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(nidorino._internal);

    pkmn_pokemon2_t magnemite = empty_pokemon;
    error = pkmn_pokemon2_init(
                "Magnemite",
                game,
                "",
                50,
                &magnemite
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(magnemite._internal);

    error = pkmn_pokemon2_get_gender(
                &magnemite,
                &gender
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_GENDERLESS, gender);

    error = pkmn_pokemon2_set_gender(
                &magnemite,
                PKMN_GENDER_GENDERLESS
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon2_set_gender(
                &magnemite,
                PKMN_GENDER_MALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon2_set_gender(
                &magnemite,
                PKMN_GENDER_FEMALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_pokemon2_free(&magnemite);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
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
        pkmn_pokemon2_t pokemon = empty_pokemon;
        error = pkmn_pokemon2_init(
                    mixed_pokemon[pokemon_index],
                    game,
                    "",
                    50,
                    &pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon2_get_gender(
                    &pokemon,
                    &gender
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_EQUAL(PKMN_GENDER_GENDERLESS, gender);

        error = pkmn_pokemon2_set_gender(
                    &pokemon,
                    PKMN_GENDER_MALE
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon2_get_gender(
                    &pokemon,
                    &gender
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, gender);

        error = pkmn_pokemon2_set_gender(
                    &pokemon,
                    PKMN_GENDER_FEMALE
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon2_get_gender(
                    &pokemon,
                    &gender
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, gender);

        error = pkmn_pokemon2_set_gender(
                    &pokemon,
                    PKMN_GENDER_GENDERLESS
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

        error = pkmn_pokemon2_free(&pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
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
