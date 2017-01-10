/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/pokemon_pc_tests.hpp>

#include "pkmn_boost_unit_test.hpp"

// Common functions

static void pokemon_box_test(
    const std::string &game
) {
    pkmn::pokemon_box::sptr box = pkmn::pokemon_box::make(game);

    pkmntest::test_empty_pokemon_box(
        box,
        game
    );

    pkmntest::test_box_name(box);
}

static void pokemon_pc_test(
    const std::string &game
) {
    pkmn::pokemon_pc::sptr pc = pkmn::pokemon_pc::make(game);

    pkmntest::test_empty_pokemon_pc(
        pc,
        game
    );

    pkmntest::test_pc_box_names(pc);
}

#define TEST_BOX_AND_PC_FOR_GAME(test_game, game_name) \
    BOOST_AUTO_TEST_CASE(test_game ## _pokemon_box_test) { \
        pokemon_box_test(game_name); \
    } \
    BOOST_AUTO_TEST_CASE(test_game ## _pokemon_pc_test) { \
        pokemon_pc_test(game_name); \
    }

TEST_BOX_AND_PC_FOR_GAME(red, "Red");
TEST_BOX_AND_PC_FOR_GAME(blue, "Blue");
TEST_BOX_AND_PC_FOR_GAME(yellow, "Yellow");
TEST_BOX_AND_PC_FOR_GAME(gold, "Gold");
TEST_BOX_AND_PC_FOR_GAME(silver, "Silver");
TEST_BOX_AND_PC_FOR_GAME(crystal, "Crystal");
TEST_BOX_AND_PC_FOR_GAME(ruby, "Ruby");
TEST_BOX_AND_PC_FOR_GAME(sapphire, "Sapphire");
TEST_BOX_AND_PC_FOR_GAME(emerald, "Emerald");
TEST_BOX_AND_PC_FOR_GAME(firered, "FireRed");
TEST_BOX_AND_PC_FOR_GAME(leafgreen, "LeafGreen");
