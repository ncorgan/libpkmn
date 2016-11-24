/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen1_pokemon_tests.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

namespace pkmntest {
    void gen1_pokemon_test(
        pkmn::pokemon::sptr pokemon,
        const std::string &species,
        const std::string &game
    ) {
        (void)pokemon;
        (void)species;
        (void)game;
    }
}
