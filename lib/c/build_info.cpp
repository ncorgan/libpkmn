/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/build_info.h>

#include <pkmn/build_info.hpp>

const char* pkmn_boost_version()
{
    static const std::string boost_version = pkmn::build_info::get_boost_version();
    return boost_version.c_str();
}

const char* pkmn_libpkmgc_version()
{
    static const std::string libpkmgc_version = pkmn::build_info::get_libpkmgc_version();
    return libpkmgc_version.c_str();
}

const char* pkmn_pksav_version()
{
    static const std::string pksav_version = pkmn::build_info::get_pksav_version();
    return pksav_version.c_str();
}

const char* pkmn_qt_version()
{
    static const std::string qt_version = pkmn::build_info::get_qt_version();
    return qt_version.c_str();
}

const char* pkmn_sqlite3_version()
{
    static const std::string sqlite3_version = pkmn::build_info::get_sqlite3_version();
    return sqlite3_version.c_str();
}

const char* pkmn_sqlitecpp_version()
{
    static const std::string sqlitecpp_version = pkmn::build_info::get_sqlitecpp_version();
    return sqlitecpp_version.c_str();
}
