--
-- Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local luaunit = require("luaunit")
local pkmn = require("pkmn")

math.randomseed(os.time())

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

function test_gen2_hidden_power()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, -1, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, 16, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, 0, -1, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, 0, 16, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, 0, 0, -1, 0)
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, 0, 0, 16, 0)
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, 0, 0, 0, -1)
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, 0, 0, 0, 16)

    --
    -- Make sure known good inputs result in expected results, and test (in)equality operators.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_II
    --
    expected_hidden_power = pkmn.calculations.hidden_power("Dark", 69)
    hidden_power_different_type = pkmn.calculations.hidden_power("Normal", 69)
    hidden_power_different_base_power = pkmn.calculations.hidden_power("Dark", 50)

    hidden_power = pkmn.calculations.gen2_hidden_power(15, 15, 15, 14)
    luaunit.assertEquals(hidden_power, expected_hidden_power)
    luaunit.assertNotEquals(hidden_power, hidden_power_different_type)
    luaunit.assertNotEquals(hidden_power, hidden_power_different_base_power)
end

function test_modern_hidden_power()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.modern_hidden_power, -1, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 32, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, -1, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 32, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, -1, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, 32, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, 0, -1, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, 0, 32, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, 0, 0, -1, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, 0, 0, 32, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, 0, 0, 0, -1)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, 0, 0, 0, 32)

    --
    -- Make sure known good inputs result in expected results.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_III_to_VI
    --
    expected_hidden_power = pkmn.calculations.hidden_power("Grass", 70)
    hidden_power_different_type = pkmn.calculations.hidden_power("Steel", 70)
    hidden_power_different_base_power = pkmn.calculations.hidden_power("Grass", 10)

    hidden_power = pkmn.calculations.modern_hidden_power(30, 31, 31, 31, 30, 31)
    luaunit.assertEquals(hidden_power, expected_hidden_power)
    luaunit.assertNotEquals(hidden_power, hidden_power_different_type)
    luaunit.assertNotEquals(hidden_power, hidden_power_different_base_power)
end

function test_nature()
    -- Make sure SWIG+Lua catches values outside the uint32_t range.
    luaunit.assertError(pkmn.calculations.nature, -1)
    luaunit.assertError(pkmn.calculations.nature, 0xFFFFFFFF+1)

    local natures = {
        "Hardy", "Lonely", "Brave", "Adamant", "Naughty",
        "Bold", "Docile", "Relaxed", "Impish", "Lax",
        "Timid", "Hasty", "Serious", "Jolly", "Naive",
        "Modest", "Mild", "Quiet", "Bashful", "Rash",
        "Calm", "Gentle", "Sassy", "Careful", "Quirky",
    }

    for i = 1, #natures
    do
        luaunit.assertEquals(
            pkmn.calculations.nature((math.random(0,50000) * 1000) + i-1),
            natures[i]
        )
    end
end

--
-- Given the amount of time the C++ test takes, this will just verify
-- the API wrapper.
--
function test_personality()
    -- Test invalid ability.
    luaunit.assertError(
        pkmn.calculations.generate_personality,
        "Charmander",
        pkmn.DEFAULT_TRAINER_ID,
        true,
        "Torrent",
        "Male",
        "Quiet"
    )

    -- Test invalid gender.
    luaunit.assertError(
        pkmn.calculations.generate_personality,
        "Charmander",
        pkmn.DEFAULT_TRAINER_ID,
        true,
        "Blaze",
        "Not a gender",
        "Quiet"
    )

    -- Test invalid nature.
    luaunit.assertError(
        pkmn.calculations.generate_personality,
        "Charmander",
        pkmn.DEFAULT_TRAINER_ID,
        true,
        "Blaze",
        "Male",
        "Not a nature"
    )

    -- Make sure Lua+SWIG catch invalid trainer IDs.
    luaunit.assertError(
        pkmn.calculations.generate_personality,
        "Charmander",
        -1,
        true,
        "Blaze",
        "Male",
        "Quiet"
    )
    luaunit.assertError(
        pkmn.calculations.generate_personality,
        "Charmander",
        0xFFFFFFFF+1,
        true,
        "Blaze",
        "Male",
        "Quiet"
    )

    -- Test and validate a valid call.
    local personality = pkmn.calculations.generate_personality(
                            "Charmander",
                            pkmn.DEFAULT_TRAINER_ID,
                            true,
                            "Blaze",
                            "Male",
                            "Quiet"
                        )
    luaunit.assertEquals("Male", pkmn.calculations.modern_pokemon_gender("Charmander", personality))
    luaunit.assertTrue(pkmn.calculations.modern_shiny(pkmn.DEFAULT_TRAINER_ID, personality))
end

function test_gen2_shiny()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.gen2_shiny, -1, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_shiny, 16, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_shiny, 0, -1, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_shiny, 0, 16, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_shiny, 0, 0, -1, 0)
    luaunit.assertError(pkmn.calculations.gen2_shiny, 0, 0, 16, 0)
    luaunit.assertError(pkmn.calculations.gen2_shiny, 0, 0, 0, -1)
    luaunit.assertError(pkmn.calculations.gen2_shiny, 0, 0, 0, 16)

    --
    -- Make sure known good inputs result in expected results.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Shininess
    --
    luaunit.assertTrue(pkmn.calculations.gen2_shiny(7, 10, 10, 10))
    luaunit.assertFalse(pkmn.calculations.gen2_shiny(6, 15, 7, 15))
end

function test_modern_shiny()
    --
    -- Make sure known good inputs result in expected results.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Personality_value#Shininess
    --         http://www.smogon.com/ingame/rng/pid_iv_creation#how_shiny
    --
    luaunit.assertTrue(pkmn.calculations.modern_shiny(2814471828, 2545049318))
    luaunit.assertTrue(pkmn.calculations.modern_shiny(0xB58F0B2A, 398174488))
end

function test_pokemon_size()
    -- Test input validation.
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", -1, 0, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0xFFFFFFFF+1, 0, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, -1, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 32, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, -1, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 32, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, -1, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, 32, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, 0, -1, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, 0, 32, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, 0, 0, -1, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, 0, 0, 32, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, 0, 0, 0, -1)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, 0, 0, 0, 32)

    --
    -- There are no known good calculations, so just check for reasonable values
    -- for each relevant Pokémon.
    --
    local pokemon_with_size_checks = {
        pkmn.database.pokemon_entry("Barboach", "Ruby", ""),
        pkmn.database.pokemon_entry("Shroomish", "Ruby", ""),
        pkmn.database.pokemon_entry("Seedot", "Emerald", ""),
        pkmn.database.pokemon_entry("Lotad", "Emerald", ""),
        pkmn.database.pokemon_entry("Magikarp", "FireRed", ""),
        pkmn.database.pokemon_entry("Heracross", "FireRed", "")
    }

    for i = 1, #pokemon_with_size_checks
    do
        local height = pokemon_with_size_checks[i]:get_height()
        local species = pokemon_with_size_checks[i]:get_name()

        for j = 1, 10
        do
            local size = pkmn.calculations.pokemon_size(
                             species,
                             math.random(0, 2147483646),
                             math.random(0, 31),
                             math.random(0, 31),
                             math.random(0, 31),
                             math.random(0, 31),
                             math.random(0, 31),
                             math.random(0, 31)
                         )
            luaunit.assertTrue(math.abs(height-size) <= height)
        end
    end
end

function test_spinda_coords()
    -- Check (in)equality.
    spinda_coords1 = pkmn.calculations.spinda_coords(123, 456)
    spinda_coords2 = pkmn.calculations.spinda_coords(123, 456)
    spinda_coords3 = pkmn.calculations.spinda_coords(456, 123)

    luaunit.assertEquals(spinda_coords1, spinda_coords2)
    luaunit.assertNotEquals(spinda_coords1, spinda_coords3)
end

function test_spinda_spots()
    -- Check (in)equality.
    spots1 = pkmn.calculations.spinda_spots(
                 pkmn.calculations.spinda_coords(7, 5),
                 pkmn.calculations.spinda_coords(10, 0),
                 pkmn.calculations.spinda_coords(1, 4),
                 pkmn.calculations.spinda_coords(2, 15)
             )
    spots2 = pkmn.calculations.spinda_spots(
                 pkmn.calculations.spinda_coords(7, 5),
                 pkmn.calculations.spinda_coords(10, 0),
                 pkmn.calculations.spinda_coords(1, 4),
                 pkmn.calculations.spinda_coords(2, 15)
             )
    spots3 = pkmn.calculations.spinda_spots(
                 pkmn.calculations.spinda_coords(7, 5),
                 pkmn.calculations.spinda_coords(10, 0),
                 pkmn.calculations.spinda_coords(2, 15),
                 pkmn.calculations.spinda_coords(1, 4)
             )
    luaunit.assertEquals(spots1, spots2)
    luaunit.assertNotEquals(spots1, spots3)

    --
    -- Make sure known good inputs result in expected results, and test (in)equality operators.
    --
    -- Source: https://github.com/magical/spinda
    --
    spots = pkmn.calculations.spinda_spot_offset(4064348759)
    luaunit.assertEquals(spots, spots1)
end

function test_gb_stats()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.get_gb_stat, "Attack", 1, 1, 123456, 1)
    luaunit.assertError(pkmn.calculations.get_gb_stat, "Attack", 1, 1, 1, 12345)

    --
    -- Test with known good inputs.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generations_I_and_II
    --
    luaunit.assertAlmostEquals(pkmn.calculations.get_gb_stat("HP", 81, 35, 22850, 7), 189, 1)
    luaunit.assertAlmostEquals(pkmn.calculations.get_gb_stat("Attack", 81, 55, 23140, 8), 137, 1)
end

function test_modern_stats()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.get_modern_stat, "Not a stat", 1, 1.0, 1, 1, 1)
    luaunit.assertError(pkmn.calculations.get_modern_stat, "Special", 1, 1.0, 1, 1, 1)
    luaunit.assertError(pkmn.calculations.get_modern_stat, "Attack", 1, 0.666, 1, 1, 1)
    luaunit.assertError(pkmn.calculations.get_modern_stat, "Attack", 1, 1.0, 1, 12345, 1)
    luaunit.assertError(pkmn.calculations.get_modern_stat, "Attack", 1, 1.0, 1, 1, 12345)

    --
    -- Test with known good inputs.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generation_III_onward
    --
    luaunit.assertAlmostEquals(pkmn.calculations.get_modern_stat("HP", 78, 1.0, 108, 74, 24), 289, 1)
    luaunit.assertAlmostEquals(pkmn.calculations.get_modern_stat("Attack", 78, 1.1, 130, 195, 12), 280, 1)
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
