/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn.h>

#include <stdlib.h>

#define BUFFER_LEN 1024

static char strbuffer[BUFFER_LEN];
static pkmn_error_t error_code = PKMN_ERROR_NONE;

/*
 * The example paths don't need to exist (except the database path). These
 * tests just make sure the environment variables override the defaults.
 */

static void appdata_dir_test() {
#ifdef PKMN_PLATFORM_WIN32
    _putenv_s("PKMN_APPDATA_DIR", "C:\\libpkmn\\appdata\\dir");
#else
    setenv("PKMN_APPDATA_DIR", "/libpkmn/appdata/dir", 0);
#endif

    error_code = pkmn_get_appdata_dir(
                     strbuffer,
                     sizeof(strbuffer)
                 );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error_code);
#ifdef PKMN_PLATFORM_WIN32
    TEST_ASSERT_EQUAL_STRING("C:\\libpkmn\\appdata\\dir", strbuffer);
#else
    TEST_ASSERT_EQUAL_STRING("/libpkmn/appdata/dir", strbuffer);
#endif
}

static void database_path_test() {
    /*
     * When this unit test is run, the PKMN_DATABASE_PATH environment variable
     * is set to a valid value, so just make sure the call doesn't fail.
     */
    error_code = pkmn_get_database_path(
                     strbuffer,
                     sizeof(strbuffer)
                 );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error_code);

    /*
     * Make sure a call to an existing file fails if it's not a valid SQLite
     * database. This is admittedly fragile, but this test is never meant to be
     * outside the build system.
     */
#ifdef PKMN_PLATFORM_WIN32
    _putenv_s("PKMN_DATABASE_PATH", __FILE__);
#else
    setenv("PKMN_DATABASE_PATH", __FILE__, 1);
#endif

    error_code = pkmn_get_database_path(
                     strbuffer,
                     sizeof(strbuffer)
                 );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RUNTIME_ERROR, error_code);

    // Now just make sure it fails with a non-existent file.
#ifdef PKMN_PLATFORM_WIN32
    _putenv_s("PKMN_DATABASE_PATH", "C:\\libpkmn\\database\\path");
#else
    setenv("PKMN_DATABASE_PATH", "/libpkmn/database/path", 1);
#endif

    error_code = pkmn_get_database_path(
                     strbuffer,
                     sizeof(strbuffer)
                 );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RUNTIME_ERROR, error_code);
}

static void images_dir_test() {
#ifdef PKMN_PLATFORM_WIN32
    _putenv_s("PKMN_IMAGES_DIR", "C:\\libpkmn\\images\\dir");
#else
    setenv("PKMN_IMAGES_DIR", "/libpkmn/images/dir", 1);
#endif

    error_code = pkmn_get_images_dir(
                     strbuffer,
                     sizeof(strbuffer)
                 );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error_code);
#ifdef PKMN_PLATFORM_WIN32
    TEST_ASSERT_EQUAL_STRING("C:\\libpkmn\\images\\dir", strbuffer);
#else
    TEST_ASSERT_EQUAL_STRING("/libpkmn/images/dir", strbuffer);
#endif
}

static void tmp_dir_test() {
    error_code = pkmn_get_tmp_dir(
                     strbuffer,
                     sizeof(strbuffer)
                 );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error_code);
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(appdata_dir_test)
    PKMN_C_TEST(database_path_test)
    PKMN_C_TEST(images_dir_test)
    PKMN_C_TEST(tmp_dir_test)
)
