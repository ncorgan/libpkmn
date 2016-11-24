/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen1_pokemon_tests.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(gen1_red_pokemon_test) {
    std::string species = "Charmander";

    pkmn::pokemon::sptr pokemon = pkmn::pokemon::make(
                                      species,
                                      "Red",
                                      "",
                                      "Ember",
                                      "Fire Spin",
                                      "Slash",
                                      "None",
                                      30
                                  );

    pkmntest::gen1_pokemon_test(
        pokemon,
        species,
        "Red"
    );
}
