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

function gen1_pokemon_tests.common(game, species)
    local pokemon = pkmn.pokemon(species, game, "", 30)
    local test_params =
    {
        valid_ball = "Great Ball",
        invalid_ball = {"Great Ball"},

        valid_item = "Potion",
        invalid_items = {"Potion"},

        expected_original_location = "Special",
        valid_locations = {"Route 1"},
        invalid_locations = {"Route 1"},

        valid_moves = {"Slash", "Flamethrower", "Tail Whip", "Fire Blast"},
        invalid_moves = {"Return"},

        valid_original_games = {"Red"},
        invalid_original_games = {"Red"}
    }

    pokemon_tests.test_common(pokemon, test_params)

    luaunit.assertEquals(pokemon.numeric_attributes["Catch rate"], 45)
end

-- Red

function test_red_pokemon()
    gen1_pokemon_tests.common("Red", "Charmander")
end

function test_red_forms()
    pokemon_tests.forms_test("Red")
end

-- Blue

function test_blue_pokemon()
    gen1_pokemon_tests.common("Blue", "Squirtle")
end

function test_blue_forms()
    pokemon_tests.forms_test("Blue")
end

-- Yellow

function test_yellow_pokemon()
    gen1_pokemon_tests.common("Yellow", "Bulbasaur")
end

function test_yellow_forms()
    pokemon_tests.forms_test("Yellow")
end
