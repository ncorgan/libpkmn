/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen1_pokemon_tests.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN

#include <boost/algorithm/string.hpp>
#include <boost/test/unit_test.hpp>

namespace pkmntest {
    void gen1_pokemon_test(
        pkmn::pokemon::sptr pokemon,
        const std::string &species,
        const std::string &game
    ) {
        BOOST_CHECK_EQUAL(pokemon->get_game(), game);
        BOOST_CHECK_EQUAL(
            pokemon->get_nickname(),
            boost::algorithm::to_upper_copy(species)
        );
        BOOST_CHECK_EQUAL(pokemon->get_trainer_name(), "LibPKMN");
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_public_id(),
            (pkmn::pokemon::LIBPKMN_OT_ID && 0xFFFF)
        );
        BOOST_CHECK_THROW(
            pokemon->get_trainer_secret_id();
        , std::runtime_error);
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_id(),
            (pkmn::pokemon::LIBPKMN_OT_ID && 0xFFFF)
        );
        BOOST_CHECK_THROW(
            pokemon->get_location_caught();
        , std::runtime_error);
        BOOST_CHECK_EQUAL(pokemon->get_level(), 30);
    }
}
