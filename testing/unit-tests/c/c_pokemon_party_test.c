/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmntest-c/pokemon_party_tests.h>

#include <pkmn.h>

static void pokemon_party_test_common(
    const char* game
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_party_handle_t party = NULL;

    error = pkmn_pokemon_party_make(
                &party,
                game
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(party);

    pkmntest_test_empty_pokemon_party(party, game);
    pkmntest_test_setting_pokemon_in_party(party);

    error = pkmn_pokemon_party_free(&party);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(party);
}

// Red

static void red_pokemon_party_test() {
    pokemon_party_test_common("Red");
}

// Blue

static void blue_pokemon_party_test() {
    pokemon_party_test_common("Blue");
}

// Yellow

static void yellow_pokemon_party_test() {
    pokemon_party_test_common("Yellow");
}

// Gold

static void gold_pokemon_party_test() {
    pokemon_party_test_common("Gold");
}

// Silver

static void silver_pokemon_party_test() {
    pokemon_party_test_common("Silver");
}

// Crystal

static void crystal_pokemon_party_test() {
    pokemon_party_test_common("Crystal");
}

// Ruby

static void ruby_pokemon_party_test() {
    pokemon_party_test_common("Ruby");
}

// Sapphire

static void sapphire_pokemon_party_test() {
    pokemon_party_test_common("Sapphire");
}

// Emerald

static void emerald_pokemon_party_test() {
    pokemon_party_test_common("Emerald");
}

// FireRed

static void firered_pokemon_party_test() {
    pokemon_party_test_common("FireRed");
}

// LeafGreen

static void leafgreen_pokemon_party_test() {
    pokemon_party_test_common("LeafGreen");
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
)
