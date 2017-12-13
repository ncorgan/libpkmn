/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/types/class_with_attributes.hpp>

#include <gtest/gtest.h>

#include <functional>

class attribute_test_class: public pkmn::class_with_attributes
{
    public:
        attribute_test_class():
            pkmn::class_with_attributes(),
            _int1(1), _int2(2), _int3(3),
            _string1("One"), _string2("Two"), _string3("Three")
        {
            _register_attributes();
        }
        ~attribute_test_class() {}

    private:
        int _int1; // Read/write
        int _int2; // Read-only
        int _int3; // Write-only

        std::string _string1; // Read/write
        std::string _string2; // Read-only
        std::string _string3; // Write-only

        int _get_int1()
        {
            return _int1;
        }

        void _set_int1(int value)
        {
            _int1 = value;
        }

        int _get_int2()
        {
            return _int2;
        }

        void _set_int3(int value)
        {
            _int3 = value;
        }

        std::string _get_string1()
        {
            return _string1;
        }

        void _set_string1(const std::string& value)
        {
            _string1 = value;
        }

        std::string _get_string2()
        {
            return _string2;
        }

        void _set_string3(const std::string& value)
        {
            _string3 = value;
        }

        void _register_attributes()
        {
            using namespace std::placeholders;

            _numeric_attribute_engine.register_attribute_fcns(
                "int1",
                std::bind(&attribute_test_class::_get_int1, this),
                std::bind(&attribute_test_class::_set_int1, this, _1)
            );
            _numeric_attribute_engine.register_attribute_fcns(
                "int2",
                std::bind(&attribute_test_class::_get_int2, this),
                nullptr
            );
            _numeric_attribute_engine.register_attribute_fcns(
                "int3",
                nullptr,
                std::bind(&attribute_test_class::_set_int3, this, _1)
            );

            _string_attribute_engine.register_attribute_fcns(
                "string1",
                std::bind(&attribute_test_class::_get_string1, this),
                std::bind(&attribute_test_class::_set_string1, this, _1)
            );
            _string_attribute_engine.register_attribute_fcns(
                "string2",
                std::bind(&attribute_test_class::_get_string2, this),
                nullptr
            );
            _string_attribute_engine.register_attribute_fcns(
                "string3",
                nullptr,
                std::bind(&attribute_test_class::_set_string3, this, _1)
            );
        }
};

TEST(cpp_attribute_test, test_attribute_engine)
{
    attribute_test_class test_class;

    // Test default values for what we can read.
    EXPECT_EQ(1, test_class.get_numeric_attribute("int1"));
    EXPECT_EQ(2, test_class.get_numeric_attribute("int2"));
    EXPECT_EQ("One", test_class.get_string_attribute("string1"));
    EXPECT_EQ("Two", test_class.get_string_attribute("string2"));

    // Set and get values for read/write attributes.
    test_class.set_numeric_attribute("int1", 101);
    EXPECT_EQ(101, test_class.get_numeric_attribute("int1"));
    test_class.set_string_attribute("string1", "101");
    EXPECT_EQ("101", test_class.get_string_attribute("string1"));

    // Attempt to write read-only attributes.
    EXPECT_THROW(
        test_class.set_numeric_attribute("int2", 102);
    , std::invalid_argument);
    EXPECT_THROW(
        test_class.set_string_attribute("string2", "102");
    , std::invalid_argument);

    // Attempt to read write-only attributes.
    EXPECT_THROW(
        test_class.get_numeric_attribute("int3");
    , std::invalid_argument);
    EXPECT_THROW(
        test_class.get_string_attribute("string3");
    , std::invalid_argument);

    // Attempt to get/set invalid attributes.
    EXPECT_THROW(
        test_class.get_numeric_attribute("Not an attribute");
    , std::invalid_argument);
    EXPECT_THROW(
        test_class.set_numeric_attribute("Not an attribute", 0);
    , std::invalid_argument);
    EXPECT_THROW(
        test_class.get_string_attribute("Not an attribute");
    , std::invalid_argument);
    EXPECT_THROW(
        test_class.set_string_attribute("Not an attribute", "0");
    , std::invalid_argument);

    // Read attribute names.
    const std::vector<std::string> expected_numeric_attribute_names = {"int1", "int2", "int3"};
    const std::vector<std::string> expected_string_attribute_names = {"string1", "string2", "string3"};

    EXPECT_EQ(expected_numeric_attribute_names, test_class.get_numeric_attribute_names());
    EXPECT_EQ(expected_string_attribute_names, test_class.get_string_attribute_names());
}
