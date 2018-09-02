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
            pkmn.species.NONE
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
                pkmn.species.NONE
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
                                     pkmn.species.VENUSAUR,
                                     daycare.game,
                                     "",
                                     50
                                 )
    luaunit.assertEquals(
        daycare.levelup_pokemon[1].species,
        pkmn.species.VENUSAUR
    )

    if #daycare.levelup_pokemon == 2
    then
        daycare.levelup_pokemon[2] = pkmn.pokemon(
                                         pkmn.species.CHARIZARD,
                                         daycare.game,
                                         "",
                                         50
                                     )
        luaunit.assertEquals(
            daycare.levelup_pokemon[2].species,
            pkmn.species.CHARIZARD
        )
    end

    if daycare.can_breed_pokemon
    then
        -- TODO: validate genders
        daycare.breeding_pokemon[1] = pkmn.pokemon(
                                         pkmn.species.BLASTOISE,
                                         daycare.game,
                                         "",
                                         50
                                     )
        daycare.breeding_pokemon[1].gender = pkmn.gender.FEMALE

        luaunit.assertEquals(
            daycare.breeding_pokemon[1].species,
            pkmn.species.BLASTOISE
        )
        luaunit.assertEquals(
            daycare.breeding_pokemon[1].gender,
            pkmn.gender.FEMALE
        )

        daycare.breeding_pokemon[2] = pkmn.pokemon(
                                         pkmn.species.MAROWAK,
                                         daycare.game,
                                         "",
                                         50
                                     )
        daycare.breeding_pokemon[2].gender = pkmn.gender.MALE

        luaunit.assertEquals(
            daycare.breeding_pokemon[2].species,
            pkmn.species.MAROWAK
        )
        luaunit.assertEquals(
            daycare.breeding_pokemon[2].gender,
            pkmn.gender.MALE
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
    daycare_test.test_daycare(pkmn.game.RED, false)
end

---
--- Blue
---

function test_blue_daycare()
    daycare_test.test_daycare(pkmn.game.BLUE, false)
end

---
--- Yellow
---

function test_yellow_daycare()
    daycare_test.test_daycare(pkmn.game.YELLOW, false)
end

---
--- Gold
---

function test_gold_daycare()
    daycare_test.test_daycare(pkmn.game.GOLD, true)
end

---
--- Silver
---

function test_silver_daycare()
    daycare_test.test_daycare(pkmn.game.SILVER, true)
end

---
--- Crystal
---

function test_crystal_daycare()
    daycare_test.test_daycare(pkmn.game.CRYSTAL, true)
end

--[[

---
--- Ruby
---

function test_ruby_daycare()
    daycare_test.test_daycare(pkmn.game.RUBY, true)
end

---
--- Sapphire
---

function test_sapphire_daycare()
    daycare_test.test_daycare(pkmn.game.SAPPHIRE, true)
end

---
--- Emerald
---

function test_emerald_daycare()
    daycare_test.test_daycare(pkmn.game.EMERALD, true)
end

---
--- FireRed
---

function test_firered_daycare()
    daycare_test.test_daycare(pkmn.game.FIRERED, true)
end

---
--- LeafGreen
---

function test_leafgreen_daycare()
    daycare_test.test_daycare(pkmn.game.LEAFGREEN, true)
end

--]]

---
--- Colosseum
---

function test_colosseum_daycare()
    daycare_test.test_daycare(pkmn.game.COLOSSEUM, false)
end

---
--- XD
---

function test_xd_daycare()
    daycare_test.test_daycare(pkmn.game.XD, false)
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
