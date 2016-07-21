/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/calculations/form.hpp>
#include <pkmn/calculations/gender.hpp>
#include <pkmn/calculations/hidden_power.hpp>
#include <pkmn/calculations/shininess.hpp>
#include <pkmn/calculations/stats.hpp>

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

BOOST_AUTO_TEST_CASE(gen2_unown_form_test) {
}

BOOST_AUTO_TEST_CASE(gen3_unown_form_test) {
}

BOOST_AUTO_TEST_CASE(wurmple_becomes_silcoon_test) {
}

BOOST_AUTO_TEST_CASE(gen2_gender_test) {
}

BOOST_AUTO_TEST_CASE(modern_gender_test) {
}

BOOST_AUTO_TEST_CASE(gen2_hidden_power_test) {
}

BOOST_AUTO_TEST_CASE(modern_hidden_power_test) {
}

BOOST_AUTO_TEST_CASE(gen2_shiny_test) {
}

BOOST_AUTO_TEST_CASE(modern_shiny_test) {
}

BOOST_AUTO_TEST_CASE(gb_stat_test) {
    /*
     * Make sure exceptions are thrown properly
     */

    // Invalid stat
    BOOST_REQUIRE_EXCEPTION(
        int stat = pkmn::calculations::get_gb_stat(
                       "Not a stat", 1, 1, 1, 1
                   )
    , std::invalid_argument, return_true)

    // Invalid EV
    BOOST_REQUIRE_EXCEPTION(
        int stat = pkmn::calculations::get_gb_stat(
                       "Attack", 1, 1, 123456, 1
                   )
    , std::out_of_range, return_true)

    // Invalid IV
    BOOST_REQUIRE_EXCEPTION(
        int stat = pkmn::calculations::get_gb_stat(
                       "Attack", 1, 1, 1, 12345
                   )
    , std::out_of_range, return_true)
}

BOOST_AUTO_TEST_CASE(modern_stat_test) {
    /*
     * Make sure exceptions are thrown properly
     */

    // Invalid stat
    BOOST_REQUIRE_EXCEPTION(
        int stat = pkmn::calculations::get_modern_stat(
                       "Not a stat", 1, 1.0f, 1, 1, 1
                   )
    , std::invalid_argument, return_true)

    // No Special past Generation I
    BOOST_REQUIRE_EXCEPTION(
        int stat = pkmn::calculations::get_modern_stat(
                       "Special", 1, 1.0f, 1, 1, 1
                   )
    , std::invalid_argument, return_true)

    // Invalid nature modifier
    BOOST_REQUIRE_EXCEPTION(
        int stat = pkmn::calculations::get_modern_stat(
                       "Attack", 1, 0.666f, 1, 1, 1
                   )
    , std::domain_error, return_true)

    // Invalid EV
    BOOST_REQUIRE_EXCEPTION(
        int stat = pkmn::calculations::get_modern_stat(
                       "Attack", 1, 1.0f, 1, 12345, 1
                   )
    , std::out_of_range, return_true)

    // Invalid IV
    BOOST_REQUIRE_EXCEPTION(
        int stat = pkmn::calculations::get_modern_stat(
                       "Attack", 1, 1.0f, 1, 1, 12345
                   )
    , std::out_of_range, return_true)

    // TODO: test cases with known results from external sources
}
