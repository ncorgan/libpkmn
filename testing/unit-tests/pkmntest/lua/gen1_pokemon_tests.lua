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
        invalid_balls = {"Great Ball"},

        valid_item = pkmn.item.POTION,
        invalid_items = {pkmn.item.POTION},

        expected_original_location = pkmn.stat.SPECIAL,
        valid_locations = {"Route 1"},
        invalid_locations = {"Route 1"},

        valid_moves = {"Slash", "Flamethrower", "Tail Whip", "Fire Blast"},
        invalid_moves = {"Return"},

        valid_original_games = {pkmn.game.RED},
        invalid_original_games = {pkmn.game.RED}
    }

    pokemon_tests.test_common(pokemon, test_params)

    luaunit.assertEquals(pokemon.numeric_attributes["Catch rate"], 45)
end

-- Red

function test_red_pokemon()
    gen1_pokemon_tests.common(pkmn.game.RED, pkmn.species.CHARMANDER)
end

function test_red_forms()
    pokemon_tests.forms_test(pkmn.game.RED)
end

-- Blue

function test_blue_pokemon()
    gen1_pokemon_tests.common(pkmn.game.BLUE, pkmn.species.SQUIRTLE)
end

function test_blue_forms()
    pokemon_tests.forms_test(pkmn.game.BLUE)
end

-- Yellow

function test_yellow_pokemon()
    gen1_pokemon_tests.common(pkmn.game.YELLOW, pkmn.species.BULBASAUR)
end

function test_yellow_forms()
    pokemon_tests.forms_test(pkmn.game.YELLOW)
end
