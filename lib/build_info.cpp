/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/build_info.hpp>
#include <pkmn/version.hpp>

#include <boost/version.hpp>
#include <LibPkmGC/Core/Config.h>
#include <pksav/version.h>
#include <sqlite3.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>

#ifdef PKMN_ENABLE_QT
#include <QtGlobal>
#else
#define QT_VERSION_STR "N/A"
#endif

namespace pkmn { namespace build_info {

    std::string about()
    {
        std::string ret = str(boost::format(
            "LibPKMN %s\n"
            "Built with:\n"
            " * Boost %s\n"
#ifdef PKMN_ENABLE_QT
            " * Qt %s\n"
#endif
            " * SQLite3 %s\n"
            " * SQLiteCpp %s\n"
            " * PKSav %s\n"
            " * LibPkmGC %s"
        ) % PKMN_VERSION
          % get_boost_version()
#ifdef PKMN_ENABLE_QT
          % get_qt_version()
#endif
          % get_sqlite3_version()
          % get_sqlitecpp_version()
          % get_pksav_version()
          % get_libpkmgc_version());

        return ret;
    }

    std::string get_boost_version()
    {
        return boost::algorithm::replace_first_copy<std::string>(
                   std::string(BOOST_LIB_VERSION), "_", "."
               );
    }

    std::string get_libpkmgc_version()
    {
        return str(boost::format("%d.%d.%d")
                   % LIBPKMGC_VERSION_MAJOR
                   % LIBPKMGC_VERSION_MINOR
                   % LIBPKMGC_VERSION_BUILD);
    }

    std::string get_pksav_version()
    {
        return std::string(PKSAV_VERSION);
    }

    std::string get_qt_version()
    {
        return std::string(QT_VERSION_STR);
    }

    std::string get_sqlite3_version()
    {
        return std::string(SQLITE_VERSION);
    }

    std::string get_sqlitecpp_version()
    {
        return std::string(SQLITECPP_VERSION);
    }

}}
