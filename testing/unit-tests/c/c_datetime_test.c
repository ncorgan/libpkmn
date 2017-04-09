/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn.h>

static pkmn_error_t error = PKMN_ERROR_NONE;

static PKMN_INLINE void TEST_ASSERT_IN_RANGE(
    int num,
    int min,
    int max
) {
    TEST_ASSERT_TRUE(num >= min);
    TEST_ASSERT_TRUE(num <= max);
}

static void current_datetime_test() {
    pkmn_datetime_t now = {
        .year = 0,
        .month = 0,
        .day = 0,
        .hour = 0,
        .minute = 0,
        .second = 0,
        .frames = 0,
    };

    error = pkmn_current_datetime(&now);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_TRUE(now.year > 1900);
    TEST_ASSERT_IN_RANGE(now.month, 1, 12);
    TEST_ASSERT_IN_RANGE(now.day, 1, 31);
    TEST_ASSERT_IN_RANGE(now.hour, 0, 23);
    TEST_ASSERT_IN_RANGE(now.minute, 0, 59);
    TEST_ASSERT_IN_RANGE(now.second, 0, 59);
    TEST_ASSERT_EQUAL(0, now.frames);
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(current_datetime_test)
)
