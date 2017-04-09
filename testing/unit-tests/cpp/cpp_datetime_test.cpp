/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/types/datetime.hpp>

#include <gtest/gtest.h>

TEST(cpp_datetime_test, current_datetime_test) {
    pkmn::datetime current_datetime = pkmn::current_datetime();
    EXPECT_GT(current_datetime.year, 1900);
    EXPECT_GE(current_datetime.month, 1);
    EXPECT_LE(current_datetime.month, 12);
    EXPECT_GE(current_datetime.day, 1);
    EXPECT_LE(current_datetime.day, 31);
    EXPECT_GE(current_datetime.hour, 0);
    EXPECT_LE(current_datetime.hour, 23);
    EXPECT_GE(current_datetime.minute, 0);
    EXPECT_LE(current_datetime.minute, 59);
    EXPECT_GE(current_datetime.second, 0);
    EXPECT_LE(current_datetime.second, 59);
    EXPECT_EQ(0, current_datetime.frames);
}
