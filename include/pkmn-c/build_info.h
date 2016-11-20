/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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

PKMN_API pkmn_error_t pkmn_get_boost_version(
    char* boost_version_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_get_pksav_version(
    char* pksav_version_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_get_sqlite3_version(
    char* sqlite3_version_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_get_sqlitecpp_version(
    char* sqlitecpp_version_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_BUILD_INFO_H */
