/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn-c.h>

#include <string.h>

static void about_test()
{
    const char* about = pkmn_buildinfo_about();
    TEST_ASSERT_NOT_NULL(about);
    TEST_ASSERT_TRUE(strlen(about) > 0);
}

static void boost_version_test()
{
    const char* boost_version = pkmn_buildinfo_boost_version();
    TEST_ASSERT_NOT_NULL(boost_version);
    TEST_ASSERT_TRUE(strlen(boost_version) > 0);
}

static void libpkmgc_version_test()
{
    const char* libpkmgc_version = pkmn_buildinfo_libpkmgc_version();
    TEST_ASSERT_NOT_NULL(libpkmgc_version);
    TEST_ASSERT_TRUE(strlen(libpkmgc_version) > 0);
}

static void pksav_version_test()
{
    const char* pksav_version = pkmn_buildinfo_pksav_version();
    TEST_ASSERT_NOT_NULL(pksav_version);
    TEST_ASSERT_TRUE(strlen(pksav_version) > 0);
}

static void qt_version_test()
{
    const char* qt_version = pkmn_buildinfo_qt_version();
    TEST_ASSERT_NOT_NULL(qt_version);
    TEST_ASSERT_TRUE(strlen(qt_version) > 0);
}

static void sqlite3_version_test()
{
    const char* sqlite3_version = pkmn_buildinfo_sqlite3_version();
    TEST_ASSERT_NOT_NULL(sqlite3_version);
    TEST_ASSERT_TRUE(strlen(sqlite3_version) > 0);
}

static void sqlitecpp_version_test()
{
    const char* sqlitecpp_version = pkmn_buildinfo_sqlitecpp_version();
    TEST_ASSERT_NOT_NULL(sqlitecpp_version);
    TEST_ASSERT_TRUE(strlen(sqlitecpp_version) > 0);
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(about_test)
    PKMN_C_TEST(boost_version_test)
    PKMN_C_TEST(libpkmgc_version_test)
    PKMN_C_TEST(pksav_version_test)
    PKMN_C_TEST(qt_version_test)
    PKMN_C_TEST(sqlite3_version_test)
    PKMN_C_TEST(sqlitecpp_version_test)
)
