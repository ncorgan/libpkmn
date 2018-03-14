/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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
 * The value returned is guaranteed to be non-NULL.
 */
PKMN_C_API const char* pkmn_buildinfo_boost_version();

/*!
 * @brief Returns the version of LibPkmGC used in this build of LibPKMN.
 *
 * The value returned is guaranteed to be non-NULL.
 */
PKMN_C_API const char* pkmn_buildinfo_libpkmgc_version();

/*!
 * @brief Returns the version of PKSav used in this build of LibPKMN.
 *
 * The value returned is guaranteed to be non-NULL.
 */
PKMN_C_API const char* pkmn_buildinfo_pksav_version();

/*!
 * @brief Returns the version of Qt used in this build of LibPKMN.
 *
 * If this build of LibPKMN does not include Qt support, this function will
 * return the string "N/A".
 *
 * The value returned is guaranteed to be non-NULL.
 */
PKMN_C_API const char* pkmn_buildinfo_qt_version();

/*!
 * @brief Returns the version of SQLite3 used in this build of LibPKMN.
 *
 * The value returned is guaranteed to be non-NULL.
 */
PKMN_C_API const char* pkmn_buildinfo_sqlite3_version();

/*!
 * @brief Returns the version of SQLiteCpp used in this build of LibPKMN.
 *
 * The value returned is guaranteed to be non-NULL.
 */
PKMN_C_API const char* pkmn_buildinfo_sqlitecpp_version();

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_BUILD_INFO_H */
