--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")
local pokemon_tests = require("pokemon_tests")

local gba_pokemon_tests = {}

-- Ruby

function test_gba_ruby_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Ruby")
end

-- Sapphire

function test_gba_sapphire_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Sapphire")
end

-- Emerald

function test_gba_emerald_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Emerald")
end

-- FireRed

function test_gba_firered_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("FireRed")
end

-- LeafGreen

function test_gba_leafgreen_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("LeafGreen")
end
