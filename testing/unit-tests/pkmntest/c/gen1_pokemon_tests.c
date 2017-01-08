/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_tests_common.h"

#include <pkmntest-c/gen1_pokemon_tests.h>

#include <unity.h>

void pkmntest_gen1_invalid_pokemon_test(
    const char* game
) {
    test_invalid_pokemon(game);
}
