--
-- Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local luaunit = require("luaunit")
local pkmn = require("pkmn")

function test_gen2_unown_form()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.gen2_unown_form, -1, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_unown_form, 16, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_unown_form, 0, -1, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_unown_form, 0, 16, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_unown_form, 0, 0, -1, 0)
    luaunit.assertError(pkmn.calculations.gen2_unown_form, 0, 0, 16, 0)
    luaunit.assertError(pkmn.calculations.gen2_unown_form, 0, 0, 0, -1)
    luaunit.assertError(pkmn.calculations.gen2_unown_form, 0, 0, 0, 16)

    --
    -- Make sure known inputs result in expected results.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Unown.27s_letter
    --
    luaunit.assertEquals(pkmn.calculations.gen2_unown_form(10, 9, 1, 14), "G")
    luaunit.assertEquals(pkmn.calculations.gen2_unown_form(5, 15, 10, 5), "S")
end

function test_gen3_unown_form()
    --
    -- Make sure known good inputs result in expected results.
    --
    -- Source: http://www.smogon.com/ingame/rng/pid_iv_creation#unown_shape
    --
    luaunit.assertError(pkmn.calculations.gen3_unown_form(0x4C07DE71), "B")
end

function test_wurmple_becomes_silcoon()
    --
    -- Make sure known good inputs result in expected results.
    --
    -- Source: http://www.smogon.com/ingame/rng/pid_iv_creation#silcoon_cascoon
    --
    luaunit.assertTrue(pkmn.calculations.wurmple_becomes_silcoon(0x5CF4091C, true))
    luaunit.assertFalse(pkmn.calculations.wurmple_becomes_silcoon(0x091C5CF4, true))
    luaunit.assertTrue(pkmn.calculations.wurmple_becomes_silcoon(0x091C5CF4, false))
    luaunit.assertFalse(pkmn.calculations.wurmple_becomes_silcoon(0x5CF4091C, false))
end

function test_gen2_gender()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.gen2_pokemon_gender, "Not a species", 0)
    luaunit.assertError(pkmn.calculations.gen2_pokemon_gender, "Bulbasaur", -1)
    luaunit.assertError(pkmn.calculations.gen2_pokemon_gender, "Bulbasaur", 16)

    -- Make sure known good inputs result in expected results.

    -- All male
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Nidorino", 0), "Male")
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Nidorino", 15), "Male")

    -- 25% male, 75% female
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Vulpix", 11), "Female")
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Vulpix", 12), "Male")

    -- All female
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Nidorina", 0), "Female")
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Nidorina", 15), "Female")

    -- Genderless
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Magnemite", 0), "Genderless")
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Magnemite", 15), "Genderless")
end

function test_modern_gender()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.modern_pokemon_gender, "Not a species", 0)

    -- All male
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Nidorino", 0), "Male")
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Nidorino", 0xFFFFFFFF), "Male")

    -- 25% male, 75% female
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Vulpix", 190), "Female")
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Vulpix", 191), "Male")

    -- All female
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Nidorina", 0), "Female")
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Nidorina", 0xFFFFFFFF), "Female")

    -- Genderless
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Magnemite", 0), "Genderless")
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Magnemite", 0xFFFFFFFF), "Genderless")
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
