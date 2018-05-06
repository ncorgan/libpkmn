--
-- Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")

local pokemon_pc_tests = require("pokemon_pc_tests")

GAME_TO_GENERATION =
{
    ["Red"] = 1,
    ["Blue"] = 1,
    ["Yellow"] = 1,
    ["Gold"] = 2,
    ["Silver"] = 2,
    ["Crystal"] = 2,
    ["Ruby"] = 3,
    ["Sapphire"] = 3,
    ["Emerald"] = 3,
    ["FireRed"] = 3,
    ["LeafGreen"] = 3
}

function pokedex_test(pokedex)
    local generation = GAME_TO_GENERATION[pokedex.game]

    -- Test initial values.
    luaunit.assertEquals(pokedex.num_seen, 0)
    luaunit.assertEquals(#pokedex.all_seen, 0)
    luaunit.assertEquals(pokedex.num_caught, 0)
    luaunit.assertEquals(#pokedex.all_caught, 0)

    -- Set random numbers of Pokémon to be seen and caught.

    local all_pokemon = pkmn.database.get_pokemon_list(generation, true)
    local num_seen = math.random(1, #all_pokemon)
    local num_caught = math.random(1, #all_pokemon)

    for num = 1, num_seen
    do
        pokedex.seen_pokemon_map[all_pokemon[num]] = true
        luaunit.assertTrue(pokedex.seen_pokemon_map[all_pokemon[num]])
    end
    luaunit.assertEquals(pokedex.num_seen, num_seen)
    luaunit.assertEquals(#pokedex.all_seen, num_seen)

    for num = (#all_pokemon-num_caught+1), #all_pokemon
    do
        pokedex.caught_pokemon_map[all_pokemon[num]] = true
        luaunit.assertTrue(pokedex.caught_pokemon_map[all_pokemon[num]])
    end
    luaunit.assertEquals(pokedex.num_caught, num_caught)
    luaunit.assertEquals(#pokedex.all_caught, num_caught)

    -- Remove all entries.

    local all_seen = pokedex.all_seen
    for num = 1, #all_seen
    do
        pokedex.seen_pokemon_map[all_seen[num]] = false
        luaunit.assertFalse(pokedex.seen_pokemon_map[all_seen[num]])
    end
    luaunit.assertEquals(pokedex.num_seen, 0)
    luaunit.assertEquals(#pokedex.all_seen, 0)

    local all_caught = pokedex.all_caught
    for num = 1, #all_caught
    do
        pokedex.caught_pokemon_map[all_caught[num]] = false
        luaunit.assertFalse(pokedex.caught_pokemon_map[all_caught[num]])
    end
    luaunit.assertEquals(pokedex.num_caught, 0)
    luaunit.assertEquals(#pokedex.all_caught, 0)
end

---
--- Red
---

function test_red_pokedex()
    pokedex = pkmn.pokedex("Red")
    pokedex_test(pokedex)
end

---
--- Blue
---

function test_blue_pokedex()
    pokedex = pkmn.pokedex("Blue")
    pokedex_test(pokedex)
end

---
--- Yellow
---

function test_yellow_pokedex()
    pokedex = pkmn.pokedex("Yellow")
    pokedex_test(pokedex)
end

---
--- Gold
---

function test_gold_pokedex()
    pokedex = pkmn.pokedex("Gold")
    pokedex_test(pokedex)
end

---
--- Silver
---

function test_silver_pokedex()
    pokedex = pkmn.pokedex("Silver")
    pokedex_test(pokedex)
end

---
--- Crystal
---

function test_crystal_pokedex()
    pokedex = pkmn.pokedex("Crystal")
    pokedex_test(pokedex)
end

---
--- Ruby
---

function test_ruby_pokedex()
    pokedex = pkmn.pokedex("Ruby")
    pokedex_test(pokedex)
end

---
--- Sapphire
---

function test_sapphire_pokedex()
    pokedex = pkmn.pokedex("Sapphire")
    pokedex_test(pokedex)
end

---
--- Emerald
---

function test_emerald_pokedex()
    pokedex = pkmn.pokedex("Emerald")
    pokedex_test(pokedex)
end

---
--- FireRed
---

function test_firered_pokedex()
    pokedex = pkmn.pokedex("FireRed")
    pokedex_test(pokedex)
end

---
--- LeafGreen
---

function test_leafgreen_pokedex()
    pokedex = pkmn.pokedex("LeafGreen")
    pokedex_test(pokedex)
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
