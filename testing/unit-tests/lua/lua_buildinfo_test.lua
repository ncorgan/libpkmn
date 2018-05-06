--
-- Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local luaunit = require("luaunit")
local pkmn = require("pkmn")

math.randomseed(os.time())

function test_boost_version()
    luaunit.assertTrue(#pkmn.buildinfo.BOOST_VERSION > 0)
end

function test_libpkmgc_version()
    luaunit.assertTrue(#pkmn.buildinfo.LIBPKMGC_VERSION > 0)
end

function test_pksav_version()
    luaunit.assertTrue(#pkmn.buildinfo.PKSAV_VERSION > 0)
end

function test_qt_version()
    luaunit.assertTrue(#pkmn.buildinfo.QT_VERSION > 0)
end

function test_sqlite3_version()
    luaunit.assertTrue(#pkmn.buildinfo.SQLITE3_VERSION > 0)
end

function test_sqlitecpp_version()
    luaunit.assertTrue(#pkmn.buildinfo.SQLITECPP_VERSION > 0)
end

function test_swig_version()
    luaunit.assertTrue(#pkmn.buildinfo.SWIG_VERSION > 0)
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
