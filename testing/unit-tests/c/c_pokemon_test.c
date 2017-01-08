/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmntest-c/gen1_pokemon_tests.h>

#include <pkmn.h>

// Red

static void gen1_red_invalid_pokemon_test() {
    pkmntest_gen1_invalid_pokemon_test("Red");
}

// Blue

static void gen1_blue_invalid_pokemon_test() {
    pkmntest_gen1_invalid_pokemon_test("Blue");
}

// Yellow

static void gen1_yellow_invalid_pokemon_test() {
    pkmntest_gen1_invalid_pokemon_test("Yellow");
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(gen1_red_invalid_pokemon_test)

    PKMN_C_TEST(gen1_blue_invalid_pokemon_test)

    PKMN_C_TEST(gen1_yellow_invalid_pokemon_test)
)
