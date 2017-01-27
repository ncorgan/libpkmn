--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")
local pokemon_tests = require("pokemon_tests")

local gen2_pokemon_tests = {}

-- Gold

function test_gen2_gold_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Gold")
end

-- Silver

function test_gen2_silver_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Silver")
end

-- Crystal

function test_gen2_crystal_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Crystal")
end
