--
-- Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")

local pkmntest_utils = require("pkmntest_utils")
local pokemon_tests = require("pokemon_tests")

local gen2_pokemon_tests = {}

function gen2_pokemon_tests.unown_test(game)
    local unown_entry = pkmn.database.pokemon_entry("Unown", game, "")

    local unown = nil

    local unown_forms = unown_entry.forms
    for form_index = 1, #unown_forms
    do
        unown = pkmn.pokemon("Unown", game, unown_forms[form_index], 5)
        luaunit.assertEquals(unown.form, unown_forms[form_index])

        -- Make sure IVs are properly set.
        local form_from_IVs = pkmn.calculations.gen2_unown_form(
                                  unown.IVs["Attack"],
                                  unown.IVs["Defense"],
                                  unown.IVs["Speed"],
                                  unown.IVs["Special"]
                              )
        luaunit.assertEquals(form_from_IVs, unown.form)

        luaunit.assertTrue(pkmntest_utils.file_exists(unown.icon_filepath))
        luaunit.assertTrue(pkmntest_utils.file_exists(unown.sprite_filepath))
    end

    unown = pkmn.pokemon("Unown", game, "A", 5)

    -- Make sure setting the form properly changes the IVs.
    for form_index = 1, #unown_forms
    do
        unown.form = unown_forms[form_index]
        luaunit.assertEquals(unown.form, unown_forms[form_index])

        local form_from_IVs = pkmn.calculations.gen2_unown_form(
                                  unown.IVs["Attack"],
                                  unown.IVs["Defense"],
                                  unown.IVs["Speed"],
                                  unown.IVs["Special"]
                              )
        luaunit.assertEquals(form_from_IVs, unown.form)

        luaunit.assertTrue(pkmntest_utils.file_exists(unown.icon_filepath))
        luaunit.assertTrue(pkmntest_utils.file_exists(unown.sprite_filepath))
    end

    -- Make sure setting IVs properly sets the form.
    unown.IVs["Attack"] = 10
    unown.IVs["Defense"] = 9
    unown.IVs["Speed"] = 1
    unown.IVs["Special"] = 14
    luaunit.assertEquals(unown.form, "G")
end

function gen2_pokemon_tests.common(game, species)
    local pokemon = pkmn.pokemon(species, game, "", 30)
    local test_params =
    {
        valid_ball = "Great Ball",
        invalid_balls = {"Great Ball"},

        valid_item = "Berry",
        invalid_items = {"Razz Berry", "Bicycle"},

        expected_original_location = "Special",
        valid_locations = {"Sprout Tower", "Tohjo Falls"},
        invalid_locations = {"Littleroot Town", "Petalburg Woods"},

        valid_moves = {"Slash", "Flamethrower", "Return", "Fire Blast"},
        invalid_moves = {"Frenzy Plant", "Roost"},

        valid_original_games = {"Gold"},
        invalid_original_games = {"Gold"}
    }

    pokemon_tests.test_common(pokemon, test_params)

    -- Gender is tied to IVs, so make sure the abstraction reflects that.

    pokemon.gender = "Male"
    luaunit.assertEquals(pokemon.IVs["Attack"], 15)
    pokemon.gender = "Female"
    luaunit.assertTrue(pokemon.IVs["Attack"] < 15)

    pokemon.IVs["Attack"] = 0
    luaunit.assertEquals(pokemon.gender, "Female")
    pokemon.IVs["Attack"] = 15
    luaunit.assertEquals(pokemon.gender, "Male")

    -- Shininess is tied to IVs, so make sure the abstraction reflects that.

    pokemon.is_shiny = false
    luaunit.assertEquals(pokemon.IVs["Attack"], 13)
    pokemon.is_shiny = true
    luaunit.assertEquals(pokemon.IVs["Attack"], 15)
    luaunit.assertEquals(pokemon.IVs["Defense"], 10)
    luaunit.assertEquals(pokemon.IVs["Speed"], 10)
    luaunit.assertEquals(pokemon.IVs["Special"], 10)
end

-- Gold

function test_gold_pokemon()
    gen2_pokemon_tests.common("Gold", "Cyndaquil")
end

function test_gold_forms()
    pokemon_tests.forms_test("Gold")
end

function test_gold_genders()
    pokemon_tests.gender_test("Gold")
end

function test_gold_unown()
    gen2_pokemon_tests.unown_test("Gold")
end

-- Silver

function test_silver_pokemon()
    gen2_pokemon_tests.common("Silver", "Totodile")
end

function test_silver_forms()
    pokemon_tests.forms_test("Silver")
end

function test_silver_genders()
    pokemon_tests.gender_test("Silver")
end

function test_silver_unown()
    gen2_pokemon_tests.unown_test("Silver")
end

-- Crystal

function test_crystal_pokemon()
    gen2_pokemon_tests.common("Crystal", "Chikorita")
end

function test_crystal_forms()
    pokemon_tests.forms_test("Crystal")
end

function test_crystal_genders()
    pokemon_tests.gender_test("Crystal")
end

function test_crystal_unown()
    gen2_pokemon_tests.unown_test("Crystal")
end
