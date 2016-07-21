/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_UTILS_PATHS_HPP
#define PKMN_UTILS_PATHS_HPP

#include <pkmn/config.hpp>

#include <string>

namespace pkmn {

    /*!
     * @brief Returns this filesystem's AppData directory.
     *
     * This function first checks the PKMN_APPDATA_DIR environment variable.
     * If this variable is set, this value is returned. If not, the default
     * behavior is OS-dependent.
     *
     * In Windows systems, the function returns the value of the APPDATA
     * environment variable, followed by the ".pkmn" directory.
     *
     * In UNIX-like systems, the function returns the value of the HOME
     * environment variable, followed by the ".pkmn" directory.
     *
     * \returns AppData directory
     */
    PKMN_API std::string get_appdata_dir();

    /*!
     * @brief Returns the path to LibPKMN's underlying SQLite database.
     *
     * This function first checks the PKMN_DATABASE_PATH environment variable.
     * If this variable is not set or if the path does not exist, it defaults
     * to the default installation path determined at compile-time.
     *
     * In either case, it confirms that the file exists and is a valid
     * SQLite database. If so, this path is returned.
     *
     * \throws std::runtime_error If no valid database was found
     * \returns Path to LibPKMN's database
     */
    PKMN_API std::string get_database_path();

    /*!
     * @brief Returns the path to LibPKMN's images directory.
     *
     * This function first checks the PKMN_IMAGES_DIR environment variable. If
     * this variable is set and the path exists, this value is returned.
     *
     * If not, the function checks the default installation path, determined
     * at compile-time. If this path exists, it is returned.
     *
     * \throws std::runtime_error If neither path exists
     * \returns Path to LibPKMN's images directory
     */
    PKMN_API std::string get_images_dir();

    /*!
     * @brief Returns this system's temp directory.
     *
     * The function checks the following environment variables: TMP, TMPDIR,
     * TEMP. If none of these variables are set, the function returns the value
     * "/tmp".
     *
     * \returns Path to filesystem's temp directory
     */
    PKMN_API std::string get_tmp_dir();

}
#endif /* PKMN_UTILS_PATHS_HPP */
