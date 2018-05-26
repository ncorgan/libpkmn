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
    const struct pkmn_attribute_names* p_attribute_names1,
    const struct pkmn_attribute_names* p_attribute_names2
)
{
    TEST_ASSERT_NOT_NULL(p_attribute_names1);
    TEST_ASSERT_NOT_NULL(p_attribute_names2);

    TEST_ASSERT_EQUAL(
        p_attribute_names1->numeric_attribute_names.length,
        p_attribute_names2->numeric_attribute_names.length
    );
    for(size_t name_index = 0;
        name_index < p_attribute_names1->numeric_attribute_names.length;
        ++name_index)
    {
        TEST_ASSERT_EQUAL(
            0,
            strcmp(
                p_attribute_names1->numeric_attribute_names.pp_strings[name_index],
                p_attribute_names2->numeric_attribute_names.pp_strings[name_index]
            )
        );
    }

    TEST_ASSERT_EQUAL(
        p_attribute_names1->string_attribute_names.length,
        p_attribute_names2->string_attribute_names.length
    );
    for(size_t name_index = 0;
        name_index < p_attribute_names1->string_attribute_names.length;
        ++name_index)
    {
        TEST_ASSERT_EQUAL(
            0,
            strcmp(
                p_attribute_names1->string_attribute_names.pp_strings[name_index],
                p_attribute_names2->string_attribute_names.pp_strings[name_index]
            )
        );
    }

    TEST_ASSERT_EQUAL(
        p_attribute_names1->boolean_attribute_names.length,
        p_attribute_names2->boolean_attribute_names.length
    );
    for(size_t name_index = 0;
        name_index < p_attribute_names1->boolean_attribute_names.length;
        ++name_index)
    {
        TEST_ASSERT_EQUAL(
            0,
            strcmp(
                p_attribute_names1->boolean_attribute_names.pp_strings[name_index],
                p_attribute_names2->boolean_attribute_names.pp_strings[name_index]
            )
        );
    }
}
