/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen1_pokemon_tests.hpp>
#include <pkmntest/gen2_pokemon_tests.hpp>

#include <boost/test/unit_test.hpp>

typedef void (*pokemon_test_fcn_t)(pkmn::pokemon::sptr, const std::string&, const std::string&);

static void pokemon_test_common(
    const std::string &species,
    const std::string &game,
    pokemon_test_fcn_t test_fcn
) {
    pkmn::pokemon::sptr pokemon = pkmn::pokemon::make(
                                      species,
                                      game,
                                      "",
                                      30
                                  );

    test_fcn(
        pokemon,
        species,
        game
    );
}

/*
 * Red
 */

BOOST_AUTO_TEST_CASE(gen1_red_invalid_pokemon_test) {
    pkmntest::gen1_invalid_pokemon_test("Red");
}

BOOST_AUTO_TEST_CASE(gen1_red_friendship_test) {
    pkmntest::gen1_friendship_test("Red");
}

BOOST_AUTO_TEST_CASE(gen1_red_pokemon_test) {
    pokemon_test_common(
        "Charmander",
        "Red",
        pkmntest::gen1_pokemon_test
    );
}

/*
 * Blue
 */

BOOST_AUTO_TEST_CASE(gen1_blue_invalid_pokemon_test) {
    pkmntest::gen1_invalid_pokemon_test("Blue");
}

BOOST_AUTO_TEST_CASE(gen1_blue_friendship_test) {
    pkmntest::gen1_friendship_test("Blue");
}

BOOST_AUTO_TEST_CASE(gen1_blue_pokemon_test) {
    pokemon_test_common(
        "Charmander",
        "Blue",
        pkmntest::gen1_pokemon_test
    );
}

/*
 * Yellow
 */

BOOST_AUTO_TEST_CASE(gen1_yellow_invalid_pokemon_test) {
    pkmntest::gen1_invalid_pokemon_test("Yellow");
}

BOOST_AUTO_TEST_CASE(gen1_yellow_friendship_test) {
    pkmntest::gen1_friendship_test("Yellow");
}

BOOST_AUTO_TEST_CASE(gen1_yellow_pokemon_test) {
    pokemon_test_common(
        "Charmander",
        "Yellow",
        pkmntest::gen1_pokemon_test
    );
}

/*
 * Gold
 */

BOOST_AUTO_TEST_CASE(gen2_gold_invalid_pokemon_test) {
    pkmntest::gen2_invalid_pokemon_test("Gold");
}

BOOST_AUTO_TEST_CASE(gen2_gold_unown_form_test) {
    pkmntest::gen2_unown_form_test("Gold");
}

BOOST_AUTO_TEST_CASE(gen2_gold_pokemon_test) {
    pokemon_test_common(
        "Cyndaquil",
        "Gold",
        pkmntest::gen2_pokemon_test
    );
}

/*
 * Silver
 */

BOOST_AUTO_TEST_CASE(gen2_silver_invalid_pokemon_test) {
    pkmntest::gen2_invalid_pokemon_test("Silver");
}

BOOST_AUTO_TEST_CASE(gen2_silver_unown_form_test) {
    pkmntest::gen2_unown_form_test("Silver");
}

BOOST_AUTO_TEST_CASE(gen2_silver_pokemon_test) {
    pokemon_test_common(
        "Cyndaquil",
        "Silver",
        pkmntest::gen2_pokemon_test
    );
}

/*
 * Crystal
 */

BOOST_AUTO_TEST_CASE(gen2_crystal_invalid_pokemon_test) {
    pkmntest::gen2_invalid_pokemon_test("Crystal");
}

BOOST_AUTO_TEST_CASE(gen2_crystal_unown_form_test) {
    pkmntest::gen2_unown_form_test("Crystal");
}

BOOST_AUTO_TEST_CASE(gen2_crystal_pokemon_test) {
    pokemon_test_common(
        "Cyndaquil",
        "Crystal",
        pkmntest::gen2_pokemon_test
    );
}
