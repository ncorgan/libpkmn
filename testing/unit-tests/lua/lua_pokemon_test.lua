--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local luaunit = require("luaunit")

local tests = {
    require("gen1_pokemon_tests"),
    require("gen2_pokemon_tests"),
    require("gba_pokemon_tests")
}

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
