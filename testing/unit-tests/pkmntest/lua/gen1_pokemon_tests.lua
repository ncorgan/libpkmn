--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")
local pokemon_tests = require("pokemon_tests")

local gen1_pokemon_tests = {}

-- Red

function test_gen1_red_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Red")
end

-- Blue

function test_gen1_blue_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Blue")
end

-- Yellow

function test_gen1_yellow_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Yellow")
end
