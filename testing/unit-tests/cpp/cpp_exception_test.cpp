/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/exception.hpp>

#include <pksav/error.h>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

#include <gtest/gtest.h>

#include <map>
#include <unordered_map>

TEST(cpp_exception_test, test_feature_not_in_build_error)
{
    const std::string expected_msg = "This feature is not part of this build of LibPKMN: Qt support";

    try
    {
        throw pkmn::feature_not_in_build_error("Qt support");
        FAIL() << "Did not throw";
    }
    catch(const pkmn::feature_not_in_build_error& e)
    {
        EXPECT_EQ(expected_msg, std::string(e.what()));
    }
    catch(...)
    {
        FAIL() << "Did not throw pkmn::feature_not_in_build_error";
    }
}

TEST(cpp_exception_test, test_feature_not_in_game_error)
{
    const std::string expected_msg = "Ribbons not in Generation II";

    try {
        throw pkmn::feature_not_in_game_error("Ribbons", "Generation II");
        FAIL() << "Did not throw";
    } catch(const pkmn::feature_not_in_game_error& e) {
        EXPECT_EQ(expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw pkmn::feature_not_in_game_error";
    }
}

TEST(cpp_exception_test, test_pksav_error)
{
    // Source: https://github.com/ncorgan/pksav/blob/master/lib/error.c
    const std::string expected_msg = "PKSav returned the following error: \"File I/O error\"";

    try {
        throw pkmn::pksav_error(PKSAV_ERROR_FILE_IO);
        FAIL() << "Did not throw";
    } catch(const pkmn::pksav_error& e) {
        EXPECT_EQ(expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw pkmn::pksav_error";
    }
}

TEST(cpp_exception_test, test_unimplemented_error)
{
    const std::string expected_msg = "Currently unimplemented.";

    try {
        throw pkmn::unimplemented_error();
        FAIL() << "Did not throw.";
    } catch(const pkmn::unimplemented_error& e) {
        EXPECT_EQ(expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw pkmn::unimplemented_error";
    }
}

TEST(cpp_exception_test, test_string_length)
{
    const std::string test_string = "I want to be the very best.";

    pkmn::enforce_string_length(
        "Test string",
        test_string,
        0,
        100
    );

    EXPECT_THROW(
        pkmn::enforce_string_length(
            "Test string",
            test_string,
            0,
            10
        );
    , std::invalid_argument);

    EXPECT_THROW(
        pkmn::enforce_string_length(
            "Test string",
            test_string,
            100,
            200
        );
    , std::invalid_argument);
}

// Don't check error message, as map key orders cannot be guaranteed.
TEST(cpp_exception_test, test_value_in_map_keys)
{
    // std::map

    static const std::map<std::string, int> test_map =
    boost::assign::map_list_of
        ("Key1", 1351)
        ("Key2", 254)
        ("Key3", 846)
    ;

    pkmn::enforce_value_in_map_keys(
        "std::map param",
        "Key1",
        test_map
    );

    EXPECT_THROW(
        pkmn::enforce_value_in_map_keys(
            "std::map param",
            "Key4",
            test_map
        );
    , std::invalid_argument);

    // std::unordered_map

    static const std::unordered_map<std::string, int> test_unordered_map =
    boost::assign::map_list_of
        ("Key4", 1351)
        ("Key5", 254)
        ("Key6", 846)
    ;

    pkmn::enforce_value_in_map_keys(
        "std::unordered_map param",
        "Key4",
        test_unordered_map
    );

    EXPECT_THROW(
        pkmn::enforce_value_in_map_keys(
            "std::unordered_map param",
            "Key7",
            test_unordered_map
        );
    , std::invalid_argument);

    // boost::bimap

    typedef boost::bimap<std::string, int> string_int_bimap_t;
    static const string_int_bimap_t test_bimap =
    boost::assign::list_of<string_int_bimap_t::relation>
        ("Key7", 1351)
        ("Key8", 254)
        ("Key9", 846)
    ;

    pkmn::enforce_value_in_map_keys(
        "boost::bimap param",
        "Key7",
        test_bimap.left
    );

    EXPECT_THROW(
        pkmn::enforce_value_in_map_keys(
            "boost::bimap param",
            "Key10",
            test_bimap.left
        );
    , std::invalid_argument);
}

TEST(cpp_exception_test, test_value_in_vector)
{
    static const std::vector<int> int_values = boost::assign::list_of
        (0)(5)(10)(15)
    ;
    const std::string int_expected_msg = "int_param: valid values 0, 5, 10, 15.";

    pkmn::enforce_value_in_vector(
        "int_param",
        0,
        int_values
    );

    try {
        pkmn::enforce_value_in_vector(
            "int_param",
            -5,
            int_values
        );
        FAIL() << "Did not throw.";
    } catch(const std::invalid_argument& e) {
        EXPECT_EQ(int_expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw std::invalid_argument";
    }

    static const std::vector<std::string> string_values = boost::assign::list_of
        ("A")("B")("C")("D")
    ;
    const std::string string_expected_msg = "string_param: valid values A, B, C, D.";

    pkmn::enforce_value_in_vector<std::string>(
        "string_param",
        "A",
        string_values
    );

    try {
        pkmn::enforce_value_in_vector<std::string>(
            "string_param",
            "E",
            string_values
        );
        FAIL() << "Did not throw.";
    } catch(const std::invalid_argument& e) {
        EXPECT_EQ(string_expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw std::invalid_argument";
    }
}

TEST(cpp_exception_test, test_out_of_range)
{
    const int int_min = -100;
    const int int_max = 2000;
    const std::string int_expected_msg = "int_param: valid values -100-2000.";

    try {
        pkmn::enforce_bounds("int_param", int_min-1, int_min, int_max);
        FAIL() << "Did not throw";
    } catch(const std::out_of_range& e) {
        EXPECT_EQ(int_expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw std::out_of_range";
    }

    const unsigned short ushort_min = 3;
    const unsigned short ushort_max = 1234;
    const std::string ushort_expected_msg = "ushort_param: valid values 3-1234.";

    try {
        pkmn::enforce_bounds(
            "ushort_param",
            static_cast<unsigned short>(ushort_max+1),
            ushort_min,
            ushort_max
        );
        FAIL() << "Did not throw";
    } catch(const std::out_of_range& e) {
        EXPECT_EQ(ushort_expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw std::out_of_range";
    }

    // This can happen when converting a number passed in from a SWIG wrapper.
    const float float_min = 12.230000005f;
    const float float_max = 45.670000009f;
    const std::string float_expected_msg = "float_param: valid values 12.23-45.67.";

    try {
        pkmn::enforce_bounds("float_param", float_min-0.5f, float_min, float_max);
        FAIL() << "Did not throw";
    } catch(const std::out_of_range& e) {
        EXPECT_EQ(float_expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw std::out_of_range";
    }

    // This can happen when converting a number passed in from a SWIG wrapper.
    const double double_min = 12.230000005;
    const double double_max = 45.670000009;
    const std::string double_expected_msg = "double_param: valid values 12.23-45.67.";

    try {
        pkmn::enforce_bounds("double_param", double_max+0.5, double_min, double_max);
        FAIL() << "Did not throw";
    } catch(const std::out_of_range& e) {
        EXPECT_EQ(double_expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw std::out_of_range";
    }
}
