--
-- Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")
local pokemon_tests = require("pokemon_tests")

local gen1_pokemon_tests = {}

function gen1_pokemon_tests.common(game)
end

-- Red

function test_red_forms()
    pokemon_tests.forms_test("Red")
end

-- Blue

function test_blue_forms()
    pokemon_tests.forms_test("Blue")
end

-- Yellow

function test_yellow_forms()
    pokemon_tests.forms_test("Yellow")
end
