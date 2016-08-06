/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_BUILD_INFO_HPP
#define INCLUDED_PKMN_BUILD_INFO_HPP

#include <string>

namespace pkmn { namespace build_info {

    std::string get_boost_version();

    std::string get_pksav_version();

    std::string get_sqlite3_version();

    std::string get_sqlitecpp_version();

}}

#endif /* INCLUDED_PKMN_BUILD_INFO_HPP */
