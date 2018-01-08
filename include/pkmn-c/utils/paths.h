/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_UTILS_PATHS_H
#define PKMN_C_UTILS_PATHS_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

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
 * \param appdata_dir_out The buffer in which to return the AppData directory
 * \param buffer_len The size of the buffer passed into appdata_dir_out
 * \param appdata_dir_length_out The actual string length (can be NULL)
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if appdata_dir_out is NULL
 */
PKMN_C_API pkmn_error_t pkmn_get_appdata_dir(
    char* appdata_dir_out,
    size_t buffer_len,
    size_t* appdata_dir_length_out
);

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
 * \param database_path_out The buffer in which to return the database path
 * \param buffer_len The size of the buffer passed into database_path_out
 * \param database_path_length_out The actual string length (can be NULL)
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_RUNTIME_ERROR if no valid database is found
 * \returns ::PKMN_ERROR_NULL_POINTER if database_path_out is NULL
 */
PKMN_C_API pkmn_error_t pkmn_get_database_path(
    char* database_path_out,
    size_t buffer_len,
    size_t* database_path_length_out
);

/*!
 * @brief Returns the path to LibPKMN's images directory.
 *
 * This function first checks the PKMN_IMAGES_DIR environment variable. If
 * this variable is set and the path exists, this value is returned.
 *
 * If not, the function checks the default installation path, determined
 * at compile-time. If this path exists, it is returned.
 *
 * \param images_dir_out The buffer in which to return the images directory
 * \param buffer_len The size of the buffer passed into images_dir_out
 * \param images_dir_length_out The actual string length (can be NULL)
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_RUNTIME_ERROR If neither path exists
 * \returns ::PKMN_ERROR_NULL_POINTER if images_dir_out is NULL
 */
PKMN_C_API pkmn_error_t pkmn_get_images_dir(
    char* images_dir_out,
    size_t buffer_len,
    size_t* images_dir_length_out
);

/*!
 * @brief Returns this system's temp directory.
 *
 * The function checks the following environment variables: TMP, TMPDIR,
 * TEMP. If none of these variables are set, the function returns the value
 * "/tmp".
 *
 * \param tmp_dir_out The buffer in which to return the filesystem's temp directory
 * \param buffer_len The size of the buffer passed into tmp_dir_out
 * \param tmp_dir_length_out The actual string length (can be NULL)
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if tmp_dir_out is NULL
 */
PKMN_C_API pkmn_error_t pkmn_get_tmp_dir(
    char* tmp_dir_out,
    size_t buffer_len,
    size_t* tmp_dir_length_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_UTILS_PATHS_H */
