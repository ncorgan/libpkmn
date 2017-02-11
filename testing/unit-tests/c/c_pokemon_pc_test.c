/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmntest-c/pokemon_pc_tests.h>

#include <pkmn.h>

static void pokemon_box_test(
    const char* game
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_box_handle_t pokemon_box = NULL;

    error = pkmn_pokemon_box_make(
                &pokemon_box,
                game
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    pkmntest_test_empty_pokemon_box(
        pokemon_box,
        game
    );

    pkmntest_test_box_name(pokemon_box);

    pkmntest_test_setting_pokemon_in_box(pokemon_box);

    error = pkmn_pokemon_box_free(&pokemon_box);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pokemon_box);
}

static void pokemon_pc_test(
    const char* game
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_pc_handle_t pokemon_pc = NULL;

    error = pkmn_pokemon_pc_make(
                &pokemon_pc,
                game
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    pkmntest_test_empty_pokemon_pc(
        pokemon_pc,
        game
    );

    pkmntest_test_pc_box_names(pokemon_pc);

    pkmntest_test_setting_pokemon_in_boxes(pokemon_pc);

    error = pkmn_pokemon_pc_free(&pokemon_pc);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pokemon_pc);
}

#define BOX_AND_PC_TEST_FCNS(test_game, game_name) \
    static void test_game ## _pokemon_box_test() { \
        pokemon_box_test(game_name); \
    } \
    static void test_game ## _pokemon_pc_test() { \
        pokemon_pc_test(game_name); \
    }

BOX_AND_PC_TEST_FCNS(red, "Red")
BOX_AND_PC_TEST_FCNS(blue, "Blue")
BOX_AND_PC_TEST_FCNS(yellow, "Yellow")
BOX_AND_PC_TEST_FCNS(gold, "Gold")
BOX_AND_PC_TEST_FCNS(silver, "Silver")
BOX_AND_PC_TEST_FCNS(crystal, "Crystal")
BOX_AND_PC_TEST_FCNS(ruby, "Ruby")
BOX_AND_PC_TEST_FCNS(sapphire, "Sapphire")
BOX_AND_PC_TEST_FCNS(emerald, "Emerald")
BOX_AND_PC_TEST_FCNS(firered, "FireRed")
BOX_AND_PC_TEST_FCNS(leafgreen, "LeafGreen")

#define BOX_AND_PC_TESTS(test_game) \
    PKMN_C_TEST(test_game ## _pokemon_box_test) \
    PKMN_C_TEST(test_game ## _pokemon_pc_test)

PKMN_C_TEST_MAIN(
    BOX_AND_PC_TESTS(red)
    BOX_AND_PC_TESTS(blue)
    BOX_AND_PC_TESTS(yellow)
    BOX_AND_PC_TESTS(gold)
    BOX_AND_PC_TESTS(silver)
    BOX_AND_PC_TESTS(crystal)
    BOX_AND_PC_TESTS(ruby)
    BOX_AND_PC_TESTS(sapphire)
    BOX_AND_PC_TESTS(emerald)
    BOX_AND_PC_TESTS(firered)
    BOX_AND_PC_TESTS(leafgreen)
)
