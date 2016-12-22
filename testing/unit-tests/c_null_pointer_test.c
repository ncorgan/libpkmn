/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn.h>

static pkmn_error_t error = PKMN_ERROR_NONE;

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};
static size_t dummy_size_t = 0;

/*
 * <pkmn-c/build_info.h>
 */
static void build_info_null_pointer_test() {
    /*
     * pkmn_get_boost_version
     */

    error = pkmn_get_boost_version(
                NULL, // boost_version_out
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_get_boost_version(
                strbuffer,
                STRBUFFER_LEN,
                NULL // actual_strlen_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(build_info_null_pointer_test)
)
