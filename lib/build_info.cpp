/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/build_info.hpp>

#include <boost/version.hpp>
#include <pksav/version.h>
#include <sqlite3.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include <boost/algorithm/string/replace.hpp>

#if defined(PKMN_QT4) || defined(PKMN_QT5)
#include <QtGlobal>
#else
#define QT_VERSION_STR "N/A"
#endif

namespace pkmn { namespace build_info {

    std::string get_boost_version() {
        return boost::algorithm::replace_first_copy<std::string>(
                   std::string(BOOST_LIB_VERSION), "_", "."
               );
    }

    std::string get_pksav_version() {
        return std::string(PKSAV_VERSION);
    }

    std::string get_qt_version() {
        return std::string(QT_VERSION_STR);
    }

    std::string get_sqlite3_version() {
        return std::string(SQLITE_VERSION);
    }

    std::string get_sqlitecpp_version() {
        return std::string(SQLITECPP_VERSION);
    }

}}
