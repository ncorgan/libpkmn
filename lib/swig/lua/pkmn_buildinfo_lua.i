/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <lua/lua_init.i>
PKMN_LUA_INIT

%include <std_string.i>

%{
    #include <pkmn/build_info.hpp>

    static const std::string BOOST_VERSION = pkmn::build_info::get_boost_version();
    static const std::string LIBPKMGC_VERSION = pkmn::build_info::get_libpkmgc_version();
    static const std::string PKSAV_VERSION = pkmn::build_info::get_pksav_version();
    static const std::string QT_VERSION = pkmn::build_info::get_qt_version();
    static const std::string SQLITE3_VERSION = pkmn::build_info::get_sqlite3_version();
    static const std::string SQLITECPP_VERSION = pkmn::build_info::get_sqlitecpp_version();
%}

%immutable;
extern std::string BOOST_VERSION;
extern std::string LIBPKMGC_VERSION;
extern std::string PKSAV_VERSION;
extern std::string QT_VERSION;
extern std::string SQLITE3_VERSION;
extern std::string SQLITECPP_VERSION;
%mutable;
