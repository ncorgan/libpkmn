/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_BUILD_INFO_H
#define PKMN_C_BUILD_INFO_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Returns the version of Boost used in this build of LibPKMN.
 *
 * \param boost_version_out The buffer in which to return the Boost version
 * \param buffer_len The size of the buffer passed into boost_version_out
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if boost_version_out is NULL
 * \returns ::PKMN_ERROR_BUFFER_TOO_SMALL if the buffer passed into boost_version_out is too short
 */
PKMN_API pkmn_error_t pkmn_get_boost_version(
    char* boost_version_out,
    size_t buffer_len
);

/*!
 * @brief Returns the version of LibPkmGC used in this build of LibPKMN.
 *
 * \param libpkmgc_version_out The buffer in which to return the LibPkmGC version
 * \param buffer_len The size of the buffer passed into libpkmgc_version_out
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if libpkmgc_version_out is NULL
 * \returns ::PKMN_ERROR_BUFFER_TOO_SMALL if the buffer passed into libpkmgc_version_out is too short
 */
PKMN_API pkmn_error_t pkmn_get_libpkmgc_version(
    char* libpkmgc_version_out,
    size_t buffer_len
);

/*!
 * @brief Returns the version of PKSav used in this build of LibPKMN.
 *
 * \param pksav_version_out The buffer in which to return the PKSav version
 * \param buffer_len The size of the buffer passed into pksav_version_out
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if pksav_version_out is NULL
 * \returns ::PKMN_ERROR_BUFFER_TOO_SMALL if the buffer passed into pksav_version_out is too short
 */
PKMN_API pkmn_error_t pkmn_get_pksav_version(
    char* pksav_version_out,
    size_t buffer_len
);

/*!
 * @brief Returns the version of Qt used in this build of LibPKMN, if any.
 *
 * \param qt_version_out The buffer in which to return the Qt version
 * \param buffer_len The size of the buffer passed into qt_version_out
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if qt_version_out is NULL
 * \returns ::PKMN_ERROR_BUFFER_TOO_SMALL if the buffer passed into qt_version_out is too short
 */
PKMN_API pkmn_error_t pkmn_get_qt_version(
    char* qt_version_out,
    size_t buffer_len
);

/*!
 * @brief Returns the version of SQLite3 used in this build of LibPKMN.
 *
 * \param sqlite3_version_out The buffer in which to return the SQLite3 version
 * \param buffer_len The size of the buffer passed into sqlite3_version_out
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if sqlite3_version_out is NULL
 * \returns ::PKMN_ERROR_BUFFER_TOO_SMALL if the buffer passed into sqlite3_version_out is too short
 */
PKMN_API pkmn_error_t pkmn_get_sqlite3_version(
    char* sqlite3_version_out,
    size_t buffer_len
);

/*!
 * @brief Returns the version of SQLiteCpp used in this build of LibPKMN.
 *
 * \param sqlitecpp_version_out The buffer in which to return the SQLiteCpp version
 * \param buffer_len The size of the buffer passed into sqlitecpp_version_out
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if sqlitecpp_version_out is NULL
 * \returns ::PKMN_ERROR_BUFFER_TOO_SMALL if the buffer passed into sqlitecpp_version_out is too short
 */
PKMN_API pkmn_error_t pkmn_get_sqlitecpp_version(
    char* sqlitecpp_version_out,
    size_t buffer_len
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_BUILD_INFO_H */
