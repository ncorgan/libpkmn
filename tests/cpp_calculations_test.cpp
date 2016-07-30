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
    BOOST_CHECK_THROW(
        int stat = pkmn::calculations::get_gb_stat(
                       "Not a stat", 1, 1, 1, 1
                   )
    , std::invalid_argument)

    // Invalid EV
    BOOST_CHECK_THROW(
        int stat = pkmn::calculations::get_gb_stat(
                       "Attack", 1, 1, 123456, 1
                   )
    , std::out_of_range)

    // Invalid IV
    BOOST_CHECK_THROW(
        int stat = pkmn::calculations::get_gb_stat(
                       "Attack", 1, 1, 1, 12345
                   )
    , std::out_of_range)
}

BOOST_AUTO_TEST_CASE(modern_stat_test) {
    /*
     * Make sure exceptions are thrown properly
     */

    // Invalid stat
    BOOST_CHECK_THROW(
        int stat = pkmn::calculations::get_modern_stat(
                       "Not a stat", 1, 1.0f, 1, 1, 1
                   )
    , std::invalid_argument)

    // No Special past Generation I
    BOOST_CHECK_THROW(
        int stat = pkmn::calculations::get_modern_stat(
                       "Special", 1, 1.0f, 1, 1, 1
                   )
    , std::invalid_argument)

    // Invalid nature modifier
    BOOST_CHECK_THROW(
        int stat = pkmn::calculations::get_modern_stat(
                       "Attack", 1, 0.666f, 1, 1, 1
                   )
    , std::domain_error)

    // Invalid EV
    BOOST_CHECK_THROW(
        int stat = pkmn::calculations::get_modern_stat(
                       "Attack", 1, 1.0f, 1, 12345, 1
                   )
    , std::out_of_range)

    // Invalid IV
    BOOST_CHECK_THROW(
        int stat = pkmn::calculations::get_modern_stat(
                       "Attack", 1, 1.0f, 1, 1, 12345
                   )
    , std::out_of_range)

    // TODO: test cases with known results from external sources
}
