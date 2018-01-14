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

//! Returns the version of Boost used in this build of LibPKMN.
PKMN_C_API const char* pkmn_boost_version();

//! Returns the version of LibPkmGC used in this build of LibPKMN.
PKMN_C_API const char* pkmn_libpkmgc_version();

//! Returns the version of PKSav used in this build of LibPKMN.
PKMN_C_API const char* pkmn_pksav_version();

//! Returns the version of Qt used in this build of LibPKMN.
PKMN_C_API const char* pkmn_qt_version();

//! Returns the version of SQLite3 used in this build of LibPKMN.
PKMN_C_API const char* pkmn_sqlite3_version();

//! Returns the version of SQLiteCpp used in this build of LibPKMN.
PKMN_C_API const char* pkmn_sqlitecpp_version();

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_BUILD_INFO_H */
