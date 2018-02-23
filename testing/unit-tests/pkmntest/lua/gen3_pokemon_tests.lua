--
-- Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")
local pokemon_tests = require("pokemon_tests")

local gen3_pokemon_tests = {}

gen3_pokemon_tests.MARKINGS = {"Circle", "Triangle", "Square", "Heart"}
gen3_pokemon_tests.CONTEST_TYPES = {"Cool", "Beauty", "Cute", "Smart", "Tough"}
gen3_pokemon_tests.CONTEST_LEVELS = {"", " Super", " Hyper", " Master"}
gen3_pokemon_tests.RIBBONS = {"Champion", "Winning", "Victory", "Artist",
                             "Effort", "Marine", "Land", "Sky",
                             "Country", "National", "Earth", "World"}
gen3_pokemon_tests.STATS = {"HP", "Attack", "Defense", "Speed",
                           "Special Attack", "Special Defense"}
