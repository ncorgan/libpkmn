/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/build_info.h>

#include <pkmn/build_info.hpp>

pkmn_error_t pkmn_get_boost_version(
    char* boost_version_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(boost_version_out);

    PKMN_CPP_TO_C(
        return pkmn::std_string_to_c_str(
                   pkmn::build_info::get_boost_version(),
                   boost_version_out,
                   buffer_len
               );
    )
}

pkmn_error_t pkmn_get_pksav_version(
    char* pksav_version_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(pksav_version_out);

    PKMN_CPP_TO_C(
        return pkmn::std_string_to_c_str(
                   pkmn::build_info::get_pksav_version(),
                   pksav_version_out,
                   buffer_len
               );
    )
}

pkmn_error_t pkmn_get_sqlite3_version(
    char* sqlite3_version_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(sqlite3_version_out);

    PKMN_CPP_TO_C(
        return pkmn::std_string_to_c_str(
                   pkmn::build_info::get_sqlite3_version(),
                   sqlite3_version_out,
                   buffer_len
               );
    )
}

pkmn_error_t pkmn_get_sqlitecpp_version(
    char* sqlitecpp_version_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(sqlitecpp_version_out);

    PKMN_CPP_TO_C(
        return pkmn::std_string_to_c_str(
                   pkmn::build_info::get_sqlitecpp_version(),
                   sqlitecpp_version_out,
                   buffer_len
               );
    )
}
