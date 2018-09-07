/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/misc_comparison.hpp>
#include <pkmntest/util.hpp>

#include <gtest/gtest.h>

namespace pkmntest
{
    void compare_attributes(
        const std::shared_ptr<pkmn::class_with_attributes>& class1,
        const std::shared_ptr<pkmn::class_with_attributes>& class2
    )
    {
        ASSERT_EQ(
            class1->get_numeric_attribute_names(),
            class2->get_numeric_attribute_names()
        );
        for(const std::string& attribute_name: class1->get_numeric_attribute_names())
        {
            ASSERT_EQ(
                class1->get_numeric_attribute(attribute_name),
                class2->get_numeric_attribute(attribute_name)
            ) << attribute_name;
        }

        ASSERT_EQ(
            class1->get_string_attribute_names(),
            class2->get_string_attribute_names()
        );
        for(const std::string& attribute_name: class1->get_string_attribute_names())
        {
            ASSERT_EQ(
                class1->get_string_attribute(attribute_name),
                class2->get_string_attribute(attribute_name)
            ) << attribute_name;
        }

        ASSERT_EQ(
            class1->get_boolean_attribute_names(),
            class2->get_boolean_attribute_names()
        );
        for(const std::string& attribute_name: class1->get_boolean_attribute_names())
        {
            ASSERT_EQ(
                class1->get_boolean_attribute(attribute_name),
                class2->get_boolean_attribute(attribute_name)
            ) << attribute_name;
        }
    }
}
