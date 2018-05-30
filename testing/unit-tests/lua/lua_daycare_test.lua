--
-- Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")

local pokemon_party_tests = require("pokemon_party_tests")

local daycare_test = {}
daycare_test.daycare = {}

-- Stupid hacky functions to be able to test indexing and attributes

function daycare_test.daycare.get_levelup_pokemon(daycare, index)
    local pokemon = daycare.levelup_pokemon[index]
end

function daycare_test.test_empty_daycare(daycare)
    -- Levelup Pokémon

    for levelup_pokemon_index = 1, #daycare.levelup_pokemon
    do
        luaunit.assertEquals(
            daycare.levelup_pokemon[levelup_pokemon_index].species,
            "None"
        )
    end

    -- Test invalid indices.
    luaunit.assertError(
        daycare_test.daycare.get_levelup_pokemon,
        daycare,
        0
    )
    luaunit.assertError(
        daycare_test.daycare.get_levelup_pokemon,
        daycare,
        (#daycare.levelup_pokemon + 1)
    )

    -- Breeding Pokémon
    -- TODO: egg

    if daycare_test.can_breed_pokemon
    then
        for breeding_pokemon_index = 1, #daycare.breeding_pokemon
        do
            luaunit.assertEquals(
                daycare.breeding_pokemon[breeding_pokemon_index].species,
                "None"
            )
        end

        -- Test invalid indices.
        luaunit.assertError(
            daycare_test.daycare.get_breeding_pokemon,
            daycare,
            0
        )
        luaunit.assertError(
            daycare_test.daycare.get_breeding_pokemon,
            daycare,
            (#daycare.breeding_pokemon + 1)
        )
    end
end

function daycare_test.test_setting_pokemon(daycare)
    daycare.levelup_pokemon[1] = pkmn.pokemon(
                                     "Venusaur",
                                     daycare.game,
                                     "",
                                     50
                                 )
    luaunit.assertEquals(
        daycare.levelup_pokemon[1].species,
        "Venusaur"
    )

    if #daycare.levelup_pokemon == 2
    then
        daycare.levelup_pokemon[2] = pkmn.pokemon(
                                         "Charizard",
                                         daycare.game,
                                         "",
                                         50
                                     )
        luaunit.assertEquals(
            daycare.levelup_pokemon[2].species,
            "Charizard"
        )
    end

    if daycare.can_breed_pokemon
    then
        -- TODO: validate genders
        daycare.breeding_pokemon[1] = pkmn.pokemon(
                                         "Blastoise",
                                         daycare.game,
                                         "",
                                         50
                                     )
        daycare.breeding_pokemon[1].gender = "Female"

        luaunit.assertEquals(
            daycare.breeding_pokemon[1].species,
            "Blastoise"
        )
        luaunit.assertEquals(
            daycare.breeding_pokemon[1].gender,
            "Female"
        )

        daycare.breeding_pokemon[2] = pkmn.pokemon(
                                         "Marowak",
                                         daycare.game,
                                         "",
                                         50
                                     )
        daycare.breeding_pokemon[2].gender = "Male"

        luaunit.assertEquals(
            daycare.breeding_pokemon[2].species,
            "Marowak"
        )
        luaunit.assertEquals(
            daycare.breeding_pokemon[2].gender,
            "Male"
        )
    end
end

function daycare_test.test_daycare(game, expected_can_breed)
    daycare = pkmn.daycare(game)
    luaunit.assertEquals(daycare.game, game)
    luaunit.assertEquals(
        daycare.can_breed_pokemon,
        expected_can_breed
    )

    daycare_test.test_empty_daycare(daycare)
    daycare_test.test_setting_pokemon(daycare)
end

---
--- Red
---

function test_red_daycare()
    daycare_test.test_daycare("Red", false)
end

---
--- Blue
---

function test_blue_daycare()
    daycare_test.test_daycare("Blue", false)
end

---
--- Yellow
---

function test_yellow_daycare()
    daycare_test.test_daycare("Yellow", false)
end

---
--- Gold
---

function test_gold_daycare()
    daycare_test.test_daycare("Gold", true)
end

---
--- Silver
---

function test_silver_daycare()
    daycare_test.test_daycare("Silver", true)
end

---
--- Crystal
---

function test_crystal_daycare()
    daycare_test.test_daycare("Crystal", true)
end

--[[

---
--- Ruby
---

function test_ruby_daycare()
    daycare_test.test_daycare("Ruby", true)
end

---
--- Sapphire
---

function test_sapphire_daycare()
    daycare_test.test_daycare("Sapphire", true)
end

---
--- Emerald
---

function test_emerald_daycare()
    daycare_test.test_daycare("Emerald", true)
end

---
--- FireRed
---

function test_firered_daycare()
    daycare_test.test_daycare("FireRed", true)
end

---
--- LeafGreen
---

function test_leafgreen_daycare()
    daycare_test.test_daycare("LeafGreen", true)
end

--]]

---
--- Colosseum
---

function test_colosseum_daycare()
    daycare_test.test_daycare("Colosseum", false)
end

---
--- XD
---

function test_xd_daycare()
    daycare_test.test_daycare("XD", false)
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
