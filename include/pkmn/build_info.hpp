/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_BUILD_INFO_HPP
#define INCLUDED_PKMN_BUILD_INFO_HPP

#include <pkmn/config.hpp>

#include <string>

namespace pkmn { namespace build_info {

    /*!
     * @brief Returns the version of Boost used in this build of LibPKMN.
     */
    PKMN_API std::string get_boost_version();

    /*!
     * @brief Returns the version of PKSav used in this build of LibPKMN.
     */
    PKMN_API std::string get_pksav_version();

    /*!
     * @brief Returns the version of Qt used in this build of LibPKMN.
     *
     * If this build has no Qt support, this function will return "N/A".
     */
    PKMN_API std::string get_qt_version();

    /*!
     * @brief Returns the version of SQLite3 used in this build of LibPKMN.
     */
    PKMN_API std::string get_sqlite3_version();

    /*!
     * @brief Returns the version of SQLiteC++ used in this build of LibPKMN.
     */
    PKMN_API std::string get_sqlitecpp_version();

}}

#endif /* INCLUDED_PKMN_BUILD_INFO_HPP */
