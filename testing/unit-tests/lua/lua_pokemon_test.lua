--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

-- Seed it once to avoid repeating values.
math.randomseed(os.time()*10000)

local luaunit = require("luaunit")

local tests = {
    require("gen1_pokemon_tests"),
    require("gen2_pokemon_tests"),
    require("gen3_pokemon_tests"),
    require("pokemon_io_tests")
}

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
