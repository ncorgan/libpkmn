/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "env.hpp"

#include <pkmn/utils/paths.hpp>

#include <boost/test/unit_test.hpp>
    
/*
 * The example paths don't need to exist (except the database path). These
 * tests just make sure the environment variables override the defaults.
 */

BOOST_AUTO_TEST_CASE(appdata_dir_test) {
#ifdef PKMN_PLATFORM_WIN32
    static const std::string appdata_dir("C:\\libpkmn\\appdata\\dir");
#else
    static const std::string appdata_dir("/libpkmn/appdata/dir");
#endif

    pkmn_setenv("PKMN_APPDATA_DIR", appdata_dir);
    BOOST_CHECK_EQUAL(pkmn::get_appdata_dir(), appdata_dir);
}

BOOST_AUTO_TEST_CASE(database_path_test) {
    /*
     * When this unit test is run, the PKMN_DATABASE_PATH environment variable
     * is set to a valid value, so just make sure the call doesn't fail.
     */
    (void)pkmn::get_database_path();

    /*
     * Make sure a call to an existing file fails if it's not a valid SQLite
     * database. This is admittedly fragile, but this test is never meant to be
     * outside the build system.
     */
    pkmn_setenv("PKMN_DATABASE_PATH", __FILE__);
    BOOST_CHECK_THROW(
        (void)pkmn::get_database_path();
    , std::runtime_error);

    // Now just make sure it fails with a non-existent file.
#ifdef PKMN_PLATFORM_WIN32
    static const std::string database_path("C:\\libpkmn\\database\\path");
#else
    static const std::string database_path("/libpkmn/database/path");
#endif
    pkmn_setenv("PKMN_DATABASE_PATH", database_path);
    BOOST_CHECK_THROW(
        (void)pkmn::get_database_path();
    , std::runtime_error);
}

BOOST_AUTO_TEST_CASE(images_dir_test) {
#ifdef PKMN_PLATFORM_WIN32
    static const std::string images_dir("C:\\libpkmn\\images\\dir");
#else
    static const std::string images_dir("/libpkmn/images/dir");
#endif

    pkmn_setenv("PKMN_IMAGES_DIR", images_dir);
    BOOST_CHECK_EQUAL(pkmn::get_images_dir(), images_dir);    
}

BOOST_AUTO_TEST_CASE(tmp_dir_test) {

}
