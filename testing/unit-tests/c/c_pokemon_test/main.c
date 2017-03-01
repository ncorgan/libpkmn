/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include "pokemon_test.h"

#include <pkmn.h>

#include <stdlib.h>
#include <time.h>

PKMN_C_TEST_MAIN(
    srand((unsigned int)time(NULL));

    PKMN_C_TEST(red_pokemon_test);
    PKMN_C_TEST(blue_pokemon_test);
    PKMN_C_TEST(yellow_pokemon_test);

    PKMN_C_TEST(gold_pokemon_test);
    PKMN_C_TEST(silver_pokemon_test);
    PKMN_C_TEST(crystal_pokemon_test);

    PKMN_C_TEST(ruby_pokemon_test);
    PKMN_C_TEST(sapphire_pokemon_test);
    PKMN_C_TEST(emerald_pokemon_test);
    PKMN_C_TEST(firered_pokemon_test);
    PKMN_C_TEST(leafgreen_pokemon_test);

    PKMN_C_TEST(test_3gpkm);

    PKMN_C_TEST(red_friendship_test);
    PKMN_C_TEST(blue_friendship_test);
    PKMN_C_TEST(yellow_friendship_test);
)
