/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
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

    PKMN_C_TEST(colosseum_pokemon_test);
    PKMN_C_TEST(xd_pokemon_test);

    /*PKMN_C_TEST(test_3gpkm);

    PKMN_C_TEST(red_friendship_test);
    PKMN_C_TEST(blue_friendship_test);
    PKMN_C_TEST(yellow_friendship_test);

    PKMN_C_TEST(red_form_test);
    PKMN_C_TEST(blue_form_test);
    PKMN_C_TEST(yellow_form_test);
    PKMN_C_TEST(gold_form_test);
    PKMN_C_TEST(silver_form_test);
    PKMN_C_TEST(crystal_form_test);
    PKMN_C_TEST(ruby_form_test);
    PKMN_C_TEST(sapphire_form_test);
    PKMN_C_TEST(emerald_form_test);
    PKMN_C_TEST(firered_form_test);
    PKMN_C_TEST(leafgreen_form_test);
    PKMN_C_TEST(colosseum_form_test);
    PKMN_C_TEST(xd_form_test);

    PKMN_C_TEST(gold_gender_test);
    PKMN_C_TEST(silver_gender_test);
    PKMN_C_TEST(crystal_gender_test);
    PKMN_C_TEST(ruby_gender_test);
    PKMN_C_TEST(sapphire_gender_test);
    PKMN_C_TEST(emerald_gender_test);
    PKMN_C_TEST(firered_gender_test);
    PKMN_C_TEST(leafgreen_gender_test);
    PKMN_C_TEST(colosseum_gender_test);
    PKMN_C_TEST(xd_gender_test);

    PKMN_C_TEST(gold_unown_test);
    PKMN_C_TEST(silver_unown_test);
    PKMN_C_TEST(crystal_unown_test);
    PKMN_C_TEST(ruby_unown_test);
    PKMN_C_TEST(sapphire_unown_test);
    PKMN_C_TEST(emerald_unown_test);
    PKMN_C_TEST(firered_unown_test);
    PKMN_C_TEST(leafgreen_unown_test);
    PKMN_C_TEST(colosseum_unown_test);
    PKMN_C_TEST(xd_unown_test);*/
)
