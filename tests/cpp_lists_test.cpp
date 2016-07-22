/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/database/lists.hpp>

#include <boost/test/unit_test.hpp>

/*
 * TODO: figure out what that parameter actually is...
 */
bool return_true(
    const std::exception &e
) {
    (void)e;
    return true;
}

BOOST_AUTO_TEST_CASE(ability_list_test) {
    std::vector<std::string> abilities;

    // Make sure invalid generations throw an exception
    BOOST_REQUIRE_EXCEPTION(
        pkmn::database::get_ability_list(
            2, abilities
        );
    , std::out_of_range, return_true)
    BOOST_REQUIRE_EXCEPTION(
        pkmn::database::get_ability_list(
            7, abilities
        );
    , std::out_of_range, return_true)

    // TODO: test good cases
}

BOOST_AUTO_TEST_CASE(game_list_test) {
    std::vector<std::string> games;

    // Make sure invalid generations throw an exception
    BOOST_REQUIRE_EXCEPTION(
        pkmn::database::get_game_list(
            0, true, games
        );
    , std::out_of_range, return_true)
    BOOST_REQUIRE_EXCEPTION(
        pkmn::database::get_game_list(
            7, true, games
        );
    , std::out_of_range, return_true)

    // TODO: test good cases, make sure include_previous parameter works
}

BOOST_AUTO_TEST_CASE(item_list_test) {
    // TODO: make sure function checks for bad game input
    //       test good cases
}

BOOST_AUTO_TEST_CASE(location_list_test) {
    // TODO: make sure function checks for bad game input
    //       test good cases, whole_generation parameter
}

BOOST_AUTO_TEST_CASE(move_list_test) {
    // TODO: make sure function checks for bad game input
    //       test good cases
}

BOOST_AUTO_TEST_CASE(nature_list_test) {
    // TODO: test good cases
}

BOOST_AUTO_TEST_CASE(pokemon_list_test) {
    // TODO: make sure function checks for bad generation
    //       test good cases, make sure include_previous parameter works
}

BOOST_AUTO_TEST_CASE(region_list_test) {
    // TODO: confirm length
}

BOOST_AUTO_TEST_CASE(ribbon_list_test) {
    // TODO: make sure function checks for bad generation
    //       test good cases
}

BOOST_AUTO_TEST_CASE(super_training_medal_list_test) {
    // TODO: confirm length
}

BOOST_AUTO_TEST_CASE(type_list_test) {
    // TODO: make sure function checks for bad game
    //       make sure Shadow only appears for Gamecube games
}
