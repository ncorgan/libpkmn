/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/types/datetime.hpp>

#include <boost/config.hpp>

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

TEST(cpp_datetime_test, equality_test) {
    BOOST_STATIC_CONSTEXPR pkmn::datetime DATETIME1(1,2,3,4,5,6,7);
    BOOST_STATIC_CONSTEXPR pkmn::datetime DATETIME2(1,2,3,4,5,6,7);
    BOOST_STATIC_CONSTEXPR pkmn::datetime DATETIME3(3,2,1,4,5,6,7);

    EXPECT_EQ(DATETIME1, DATETIME2);
    EXPECT_NE(DATETIME1, DATETIME3);
    EXPECT_NE(DATETIME2, DATETIME3);
}
