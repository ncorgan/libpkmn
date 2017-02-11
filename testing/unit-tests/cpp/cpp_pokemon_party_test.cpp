/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/pokemon_party_tests.hpp>

#include "pkmn_boost_unit_test.hpp"

// Common function

static void pokemon_party_test(
    const std::string &game
) {
    pkmn::pokemon_party::sptr party = pkmn::pokemon_party::make(game);

    pkmntest::test_empty_pokemon_party(
        party,
        game
    );

    pkmntest::test_setting_pokemon(party);
}

// Red

BOOST_AUTO_TEST_CASE(red_pokemon_box_test) {
    pokemon_party_test("Red");
}

// Blue

BOOST_AUTO_TEST_CASE(blue_pokemon_box_test) {
    pokemon_party_test("Blue");
}

// Yellow

BOOST_AUTO_TEST_CASE(yellow_pokemon_box_test) {
    pokemon_party_test("Yellow");
}

// Gold

BOOST_AUTO_TEST_CASE(gold_pokemon_box_test) {
    pokemon_party_test("Gold");
}

// Silver

BOOST_AUTO_TEST_CASE(silver_pokemon_box_test) {
    pokemon_party_test("Silver");
}

// Crystal

BOOST_AUTO_TEST_CASE(crystal_pokemon_box_test) {
    pokemon_party_test("Crystal");
}

// Ruby

BOOST_AUTO_TEST_CASE(ruby_pokemon_box_test) {
    pokemon_party_test("Ruby");
}

// Sapphire

BOOST_AUTO_TEST_CASE(sapphire_pokemon_box_test) {
    pokemon_party_test("Sapphire");
}

// Emerald

BOOST_AUTO_TEST_CASE(emerald_pokemon_box_test) {
    pokemon_party_test("Emerald");
}

// FireRed

BOOST_AUTO_TEST_CASE(firered_pokemon_box_test) {
    pokemon_party_test("FireRed");
}

// LeafGreen

BOOST_AUTO_TEST_CASE(leafgreen_pokemon_box_test) {
    pokemon_party_test("LeafGreen");
}
