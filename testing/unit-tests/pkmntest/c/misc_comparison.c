/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest-c/misc_comparison.h>
#include <pkmntest-c/util.h>

#include <unity.h>

#include <string.h>

void compare_attribute_names(
    const pkmn_attribute_names_t* attribute_names1_ptr,
    const pkmn_attribute_names_t* attribute_names2_ptr
)
{
    TEST_ASSERT_NOT_NULL(attribute_names1_ptr);
    TEST_ASSERT_NOT_NULL(attribute_names2_ptr);

    TEST_ASSERT_EQUAL(
        attribute_names1_ptr->numeric_attribute_names.length,
        attribute_names2_ptr->numeric_attribute_names.length
    );
    for(size_t name_index = 0;
        name_index < attribute_names1_ptr->numeric_attribute_names.length;
        ++name_index)
    {
        TEST_ASSERT_EQUAL(
            0,
            strcmp(
                attribute_names1_ptr->numeric_attribute_names.strings[name_index],
                attribute_names2_ptr->numeric_attribute_names.strings[name_index]
            )
        );
    }

    TEST_ASSERT_EQUAL(
        attribute_names1_ptr->string_attribute_names.length,
        attribute_names2_ptr->string_attribute_names.length
    );
    for(size_t name_index = 0;
        name_index < attribute_names1_ptr->string_attribute_names.length;
        ++name_index)
    {
        TEST_ASSERT_EQUAL(
            0,
            strcmp(
                attribute_names1_ptr->string_attribute_names.strings[name_index],
                attribute_names2_ptr->string_attribute_names.strings[name_index]
            )
        );
    }

    TEST_ASSERT_EQUAL(
        attribute_names1_ptr->boolean_attribute_names.length,
        attribute_names2_ptr->boolean_attribute_names.length
    );
    for(size_t name_index = 0;
        name_index < attribute_names1_ptr->boolean_attribute_names.length;
        ++name_index)
    {
        TEST_ASSERT_EQUAL(
            0,
            strcmp(
                attribute_names1_ptr->boolean_attribute_names.strings[name_index],
                attribute_names2_ptr->boolean_attribute_names.strings[name_index]
            )
        );
    }
}
