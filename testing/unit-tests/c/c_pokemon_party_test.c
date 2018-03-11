/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn-c.h>

static const pkmn_pokemon_t empty_pokemon =
{
    .species = NULL,
    .game = NULL,
    ._internal = NULL
};
static const pkmn_pokemon_list_t empty_pokemon_list =
{
    .pokemon = NULL,
    .length = 0
};

static void test_empty_pokemon_party(
    pkmn_pokemon_party_t* party_ptr
)
{
    TEST_ASSERT_NOT_NULL(party_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    size_t num_pokemon = 6;

    pkmn_pokemon_list_t pokemon_list = empty_pokemon_list;

    error = pkmn_pokemon_party_get_num_pokemon(
                party_ptr,
                &num_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(0, num_pokemon);

    error = pkmn_pokemon_party_as_list(
                party_ptr,
                &pokemon_list
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(pokemon_list.pokemon);
    TEST_ASSERT_EQUAL(6, pokemon_list.length);

    for(size_t pokemon_index = 0; pokemon_index < pokemon_list.length; ++pokemon_index)
    {
        TEST_ASSERT_EQUAL_STRING(
            "None",
            pokemon_list.pokemon[pokemon_index].species
        );
        TEST_ASSERT_EQUAL_STRING(
            party_ptr->game,
            pokemon_list.pokemon[pokemon_index].game
        );
    }

    error = pkmn_pokemon_list_free(&pokemon_list);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(pokemon_list.pokemon);
    TEST_ASSERT_EQUAL(0, pokemon_list.length);
}

static void test_setting_pokemon_in_party(
    pkmn_pokemon_party_t* party_ptr
)
{
    TEST_ASSERT_NOT_NULL(party_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    size_t num_pokemon = 0;

    pkmn_pokemon_t original_first = empty_pokemon;
    pkmn_pokemon_t original_second = empty_pokemon;

    error = pkmn_pokemon_party_get_pokemon(
                party_ptr,
                0,
                &original_first
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(original_first._internal);

    error = pkmn_pokemon_party_get_pokemon(
                party_ptr,
                1,
                &original_second
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(original_second._internal);

    // Make sure we can't get Pokémon at invalid indices.
    error = pkmn_pokemon_party_get_pokemon(
                party_ptr,
                -1,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon_party_get_pokemon(
                party_ptr,
                party_ptr->capacity + 1,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Make sure we can't set Pokémon at invalid indices.
    error = pkmn_pokemon_party_set_pokemon(
                party_ptr,
                -1,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon_party_set_pokemon(
                party_ptr,
                party_ptr->capacity + 1,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    /*
     * Create new Pokémon and place in party. The original variables should
     * have the same underlying Pokémon.
     */

    pkmn_pokemon_t bulbasaur = empty_pokemon;
    pkmn_pokemon_t charmander = empty_pokemon;
    pkmn_pokemon_t squirtle = empty_pokemon;

    error = pkmn_pokemon_init(
                "Bulbasaur",
                party_ptr->game,
                "",
                5,
                &bulbasaur
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(bulbasaur._internal);

    error = pkmn_pokemon_init(
                "Charmander",
                party_ptr->game,
                "",
                5,
                &charmander
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(charmander._internal);

    error = pkmn_pokemon_init(
                "Squirtle",
                party_ptr->game,
                "",
                5,
                &squirtle
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(squirtle._internal);

    error = pkmn_pokemon_party_set_pokemon(
                party_ptr,
                0,
                &bulbasaur
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_party_get_num_pokemon(
                party_ptr,
                &num_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(1, num_pokemon);

    error = pkmn_pokemon_party_set_pokemon(
                party_ptr,
                1,
                &charmander
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_party_get_num_pokemon(
                party_ptr,
                &num_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(2, num_pokemon);

    // Replace one of the new ones.
    error = pkmn_pokemon_party_set_pokemon(
                party_ptr,
                0,
                &squirtle
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_party_get_num_pokemon(
                party_ptr,
                &num_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(2, num_pokemon);

    // Copy a Pokémon whose memory is already part of the party. Make sure we
    // can't copy it to itself.

    pkmn_pokemon_t second_in_party = empty_pokemon;
    error = pkmn_pokemon_party_get_pokemon(
                party_ptr,
                1,
                &second_in_party
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(second_in_party._internal);

    error = pkmn_pokemon_party_set_pokemon(
                party_ptr,
                1,
                &second_in_party
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_party_set_pokemon(
                party_ptr,
                2,
                &second_in_party
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_party_get_num_pokemon(
                party_ptr,
                &num_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(3, num_pokemon);

    // We should be able to clear the last contiguous Pokémon.
    error = pkmn_pokemon_party_set_pokemon(
                party_ptr,
                2,
                &original_first
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_party_get_num_pokemon(
        party_ptr,
        &num_pokemon
    );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(2, num_pokemon);

    // Put it back.
    error = pkmn_pokemon_party_set_pokemon(
                party_ptr,
                2,
                &second_in_party
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_party_get_num_pokemon(
        party_ptr,
        &num_pokemon
    );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(3, num_pokemon);

    error = pkmn_pokemon_free(&second_in_party);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(second_in_party._internal);

    // Check that Pokémon cannot be placed non-contiguously.

    error = pkmn_pokemon_party_set_pokemon(
                party_ptr,
                1,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_party_get_num_pokemon(
                party_ptr,
                &num_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(3, num_pokemon);

    error = pkmn_pokemon_party_get_pokemon(
                party_ptr,
                1,
                &second_in_party
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING("Charmander", second_in_party.species);

    error = pkmn_pokemon_free(&second_in_party);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(second_in_party._internal);

    error = pkmn_pokemon_party_set_pokemon(
                party_ptr,
                4,
                &bulbasaur
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    pkmn_pokemon_t fifth_in_party = empty_pokemon;
    error = pkmn_pokemon_party_get_pokemon(
                party_ptr,
                4,
                &fifth_in_party
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING("None", fifth_in_party.species);

    error = pkmn_pokemon_free(&fifth_in_party);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(fifth_in_party._internal);

    /*
     * Now check everything we've created. Each variable should have the
     * same Pokémon underneath, even if the pointer has changed.
     */
    pkmn_pokemon_list_t pokemon_list = empty_pokemon_list;
    error = pkmn_pokemon_party_as_list(
                party_ptr,
                &pokemon_list
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(pokemon_list.pokemon);
    TEST_ASSERT_EQUAL(6, pokemon_list.length);

    TEST_ASSERT_EQUAL_STRING(
        "Squirtle",
        pokemon_list.pokemon[0].species
    );
    TEST_ASSERT_EQUAL_STRING(
        "Charmander",
        pokemon_list.pokemon[1].species
    );
    TEST_ASSERT_EQUAL_STRING(
        "Charmander",
        pokemon_list.pokemon[2].species
    );
    TEST_ASSERT_EQUAL_STRING("None", original_first.species);
    TEST_ASSERT_EQUAL_STRING("None", original_second.species);
    TEST_ASSERT_EQUAL_STRING("Bulbasaur", bulbasaur.species);
    TEST_ASSERT_EQUAL_STRING("Charmander", charmander.species);
    TEST_ASSERT_EQUAL_STRING("Squirtle", squirtle.species);

    // Cleanup

    error = pkmn_pokemon_list_free(&pokemon_list);
    TEST_ASSERT_NULL(pokemon_list.pokemon);
    TEST_ASSERT_EQUAL(0, pokemon_list.length);

    error = pkmn_pokemon_free(&original_first);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(original_first._internal);

    error = pkmn_pokemon_free(&original_second);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(original_second._internal);

    error = pkmn_pokemon_free(&bulbasaur);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(bulbasaur._internal);

    error = pkmn_pokemon_free(&charmander);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(charmander._internal);

    error = pkmn_pokemon_free(&squirtle);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(squirtle._internal);
}

static void pokemon_party_test_common(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_pokemon_party_t party =
    {
        .game = NULL,
        .capacity = 0,
        ._internal = NULL
    };

    error = pkmn_pokemon_party_init(
                game,
                &party
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(party._internal);
    TEST_ASSERT_EQUAL_STRING(game, party.game);
    TEST_ASSERT_EQUAL(6, party.capacity);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_pokemon_party_strerror(&party)
    );

    test_empty_pokemon_party(&party);
    test_setting_pokemon_in_party(&party);

    error = pkmn_pokemon_party_free(&party);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(party.game);
    TEST_ASSERT_EQUAL(0, party.capacity);
    TEST_ASSERT_NULL(party._internal);
}

// Red

static void red_pokemon_party_test()
{
    pokemon_party_test_common("Red");
}

// Blue

static void blue_pokemon_party_test()
{
    pokemon_party_test_common("Blue");
}

// Yellow

static void yellow_pokemon_party_test()
{
    pokemon_party_test_common("Yellow");
}

// Gold

static void gold_pokemon_party_test()
{
    pokemon_party_test_common("Gold");
}

// Silver

static void silver_pokemon_party_test()
{
    pokemon_party_test_common("Silver");
}

// Crystal

static void crystal_pokemon_party_test()
{
    pokemon_party_test_common("Crystal");
}

// Ruby

static void ruby_pokemon_party_test()
{
    pokemon_party_test_common("Ruby");
}

// Sapphire

static void sapphire_pokemon_party_test()
{
    pokemon_party_test_common("Sapphire");
}

// Emerald

static void emerald_pokemon_party_test()
{
    pokemon_party_test_common("Emerald");
}

// FireRed

static void firered_pokemon_party_test()
{
    pokemon_party_test_common("FireRed");
}

// LeafGreen

static void leafgreen_pokemon_party_test()
{
    pokemon_party_test_common("LeafGreen");
}

// Colosseum

static void colosseum_pokemon_party_test()
{
    pokemon_party_test_common("Colosseum");
}

// XD

static void xd_pokemon_party_test()
{
    pokemon_party_test_common("XD");
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(red_pokemon_party_test)
    PKMN_C_TEST(blue_pokemon_party_test)
    PKMN_C_TEST(yellow_pokemon_party_test)

    PKMN_C_TEST(gold_pokemon_party_test)
    PKMN_C_TEST(silver_pokemon_party_test)
    PKMN_C_TEST(crystal_pokemon_party_test)

    PKMN_C_TEST(ruby_pokemon_party_test)
    PKMN_C_TEST(sapphire_pokemon_party_test)
    PKMN_C_TEST(emerald_pokemon_party_test)
    PKMN_C_TEST(firered_pokemon_party_test)
    PKMN_C_TEST(leafgreen_pokemon_party_test)

    PKMN_C_TEST(colosseum_pokemon_party_test)
    PKMN_C_TEST(xd_pokemon_party_test)
)
