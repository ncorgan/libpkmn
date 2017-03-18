--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")

local pokemon_pc_tests = require("pokemon_pc_tests")

---
--- Red
---

function test_red_pokemon_box()
    print("Red box")
    box = pkmn.pokemon_box("Red")
    pokemon_pc_tests.test_pokemon_box(box, "Red")
end

function test_red_pokemon_pc()
    print("Red PC")
    pc = pkmn.pokemon_pc("Red")
    pokemon_pc_tests.test_pokemon_pc(pc, "Red")
end

---
--- Blue
---

function test_blue_pokemon_box()
    print("Blue box")
    box = pkmn.pokemon_box("Blue")
    pokemon_pc_tests.test_pokemon_box(box, "Blue")
end

function test_blue_pokemon_pc()
    print("Blue PC")
    pc = pkmn.pokemon_pc("Blue")
    pokemon_pc_tests.test_pokemon_pc(pc, "Blue")
end

---
--- Yellow
---

function test_yellow_pokemon_box()
    print("Yellow box")
    box = pkmn.pokemon_box("Yellow")
    pokemon_pc_tests.test_pokemon_box(box, "Yellow")
end

function test_yellow_pokemon_pc()
    print("Yellow PC")
    pc = pkmn.pokemon_pc("Yellow")
    pokemon_pc_tests.test_pokemon_pc(pc, "Yellow")
end

---
--- Gold
---

function test_gold_pokemon_box()
    print("Gold box")
    box = pkmn.pokemon_box("Gold")
    pokemon_pc_tests.test_pokemon_box(box, "Gold")
end

function test_gold_pokemon_pc()
    print("Gold PC")
    pc = pkmn.pokemon_pc("Gold")
    pokemon_pc_tests.test_pokemon_pc(pc, "Gold")
end

---
--- Silver
---

function test_silver_pokemon_box()
    print("Silver box")
    box = pkmn.pokemon_box("Silver")
    pokemon_pc_tests.test_pokemon_box(box, "Silver")
end

function test_silver_pokemon_pc()
    print("Silver PC")
    pc = pkmn.pokemon_pc("Silver")
    pokemon_pc_tests.test_pokemon_pc(pc, "Silver")
end

---
--- Crystal
---

function test_crystal_pokemon_box()
    print("Crystal box")
    box = pkmn.pokemon_box("Crystal")
    pokemon_pc_tests.test_pokemon_box(box, "Crystal")
end

function test_crystal_pokemon_pc()
    print("Crystal PC")
    pc = pkmn.pokemon_pc("Crystal")
    pokemon_pc_tests.test_pokemon_pc(pc, "Crystal")
end

---
--- Ruby
---

function test_ruby_pokemon_box()
    print("Ruby box")
    box = pkmn.pokemon_box("Ruby")
    pokemon_pc_tests.test_pokemon_box(box, "Ruby")
end

function test_ruby_pokemon_pc()
    print("Ruby PC")
    pc = pkmn.pokemon_pc("Ruby")
    pokemon_pc_tests.test_pokemon_pc(pc, "Ruby")
end

---
--- Sapphire
---

function test_sapphire_pokemon_box()
    print("Sapphire box")
    box = pkmn.pokemon_box("Sapphire")
    pokemon_pc_tests.test_pokemon_box(box, "Sapphire")
end

function test_sapphire_pokemon_pc()
    print("Sapphire PC")
    pc = pkmn.pokemon_pc("Sapphire")
    pokemon_pc_tests.test_pokemon_pc(pc, "Sapphire")
end

---
--- Emerald
---

function test_emerald_pokemon_box()
    print("Emerald box")
    box = pkmn.pokemon_box("Emerald")
    pokemon_pc_tests.test_pokemon_box(box, "Emerald")
end

function test_emerald_pokemon_pc()
    print("Emerald PC")
    pc = pkmn.pokemon_pc("Emerald")
    pokemon_pc_tests.test_pokemon_pc(pc, "Emerald")
end

---
--- FireRed
---

function test_firered_pokemon_box()
    print("FireRed box")
    box = pkmn.pokemon_box("FireRed")
    pokemon_pc_tests.test_pokemon_box(box, "FireRed")
end

function test_firered_pokemon_pc()
    print("FireRed PC")
    pc = pkmn.pokemon_pc("FireRed")
    pokemon_pc_tests.test_pokemon_pc(pc, "FireRed")
end

---
--- LeafGreen
---

function test_leafgreen_pokemon_box()
    print("LeafGreen box")
    box = pkmn.pokemon_box("LeafGreen")
    pokemon_pc_tests.test_pokemon_box(box, "LeafGreen")
end

function test_leafgreen_pokemon_pc()
    print("LeafGreen PC")
    pc = pkmn.pokemon_pc("LeafGreen")
    pokemon_pc_tests.test_pokemon_pc(pc, "LeafGreen")
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
