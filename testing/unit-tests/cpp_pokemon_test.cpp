/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen1_pokemon_tests.hpp>

#include <boost/test/unit_test.hpp>

/*
 * Generation I Pokémon
 */

BOOST_AUTO_TEST_CASE(gen1_red_pokemon_test) {
    std::string species = "Charmander";

    pkmn::pokemon::sptr pokemon = pkmn::pokemon::make(
                                      species,
                                      "Red",
                                      "",
                                      30
                                  );

    pkmntest::gen1_pokemon_test(
        pokemon,
        species,
        "Red"
    );
}

BOOST_AUTO_TEST_CASE(gen1_blue_pokemon_test) {
    std::string species = "Charmander";

    pkmn::pokemon::sptr pokemon = pkmn::pokemon::make(
                                      species,
                                      "Blue",
                                      "",
                                      30
                                  );

    pkmntest::gen1_pokemon_test(
        pokemon,
        species,
        "Blue"
    );
}

BOOST_AUTO_TEST_CASE(gen1_yellow_pokemon_test) {
    std::string species = "Charmander";

    pkmn::pokemon::sptr pokemon = pkmn::pokemon::make(
                                      species,
                                      "Yellow",
                                      "",
                                      30
                                  );

    pkmntest::gen1_pokemon_test(
        pokemon,
        species,
        "Yellow"
    );
}

/*
 * Generation I Pokémon Box
 */

BOOST_AUTO_TEST_CASE(gen1_red_pokemon_box_test) {
    pkmn::pokemon_box::sptr pokemon_box = pkmn::pokemon_box::make("Red");
    pkmntest::gen1_pokemon_box_test(
        pokemon_box,
        "Red"
    );
}

BOOST_AUTO_TEST_CASE(gen1_blue_pokemon_box_test) {
    pkmn::pokemon_box::sptr pokemon_box = pkmn::pokemon_box::make("Blue");
    pkmntest::gen1_pokemon_box_test(
        pokemon_box,
        "Blue"
    );
}

BOOST_AUTO_TEST_CASE(gen1_yellow_pokemon_box_test) {
    pkmn::pokemon_box::sptr pokemon_box = pkmn::pokemon_box::make("Yellow");
    pkmntest::gen1_pokemon_box_test(
        pokemon_box,
        "Yellow"
    );
}

/*
 * Generation I Pokémon PC
 */

BOOST_AUTO_TEST_CASE(gen1_red_pokemon_pc_test) {
    pkmn::pokemon_pc::sptr pokemon_pc = pkmn::pokemon_pc::make("Red");
    pkmntest::gen1_pokemon_pc_test(
        pokemon_pc,
        "Red"
    );
}

BOOST_AUTO_TEST_CASE(gen1_blue_pokemon_pc_test) {
    pkmn::pokemon_pc::sptr pokemon_pc = pkmn::pokemon_pc::make("Blue");
    pkmntest::gen1_pokemon_pc_test(
        pokemon_pc,
        "Blue"
    );
}

BOOST_AUTO_TEST_CASE(gen1_yellow_pokemon_pc_test) {
    pkmn::pokemon_pc::sptr pokemon_pc = pkmn::pokemon_pc::make("Yellow");
    pkmntest::gen1_pokemon_pc_test(
        pokemon_pc,
        "Yellow"
    );
}
