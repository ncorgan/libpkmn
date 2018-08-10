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
    box = pkmn.pokemon_box(pkmn.game.RED)
    pokemon_pc_tests.test_pokemon_box(box, pkmn.game.RED)
end

function test_red_pokemon_pc()
    pc = pkmn.pokemon_pc(pkmn.game.RED)
    pokemon_pc_tests.test_pokemon_pc(pc, pkmn.game.RED)
end

---
--- Blue
---

function test_blue_pokemon_box()
    box = pkmn.pokemon_box(pkmn.game.BLUE)
    pokemon_pc_tests.test_pokemon_box(box, pkmn.game.BLUE)
end

function test_blue_pokemon_pc()
    pc = pkmn.pokemon_pc(pkmn.game.BLUE)
    pokemon_pc_tests.test_pokemon_pc(pc, pkmn.game.BLUE)
end

---
--- Yellow
---

function test_yellow_pokemon_box()
    box = pkmn.pokemon_box(pkmn.game.YELLOW)
    pokemon_pc_tests.test_pokemon_box(box, pkmn.game.YELLOW)
end

function test_yellow_pokemon_pc()
    pc = pkmn.pokemon_pc(pkmn.game.YELLOW)
    pokemon_pc_tests.test_pokemon_pc(pc, pkmn.game.YELLOW)
end

---
--- Gold
---

function test_gold_pokemon_box()
    box = pkmn.pokemon_box(pkmn.game.GOLD)
    pokemon_pc_tests.test_pokemon_box(box, pkmn.game.GOLD)
end

function test_gold_pokemon_pc()
    pc = pkmn.pokemon_pc(pkmn.game.GOLD)
    pokemon_pc_tests.test_pokemon_pc(pc, pkmn.game.GOLD)
end

---
--- Silver
---

function test_silver_pokemon_box()
    box = pkmn.pokemon_box(pkmn.game.SILVER)
    pokemon_pc_tests.test_pokemon_box(box, pkmn.game.SILVER)
end

function test_silver_pokemon_pc()
    pc = pkmn.pokemon_pc(pkmn.game.SILVER)
    pokemon_pc_tests.test_pokemon_pc(pc, pkmn.game.SILVER)
end

---
--- Crystal
---

function test_crystal_pokemon_box()
    box = pkmn.pokemon_box(pkmn.game.CRYSTAL)
    pokemon_pc_tests.test_pokemon_box(box, pkmn.game.CRYSTAL)
end

function test_crystal_pokemon_pc()
    pc = pkmn.pokemon_pc(pkmn.game.CRYSTAL)
    pokemon_pc_tests.test_pokemon_pc(pc, pkmn.game.CRYSTAL)
end

---
--- Ruby
---

function test_ruby_pokemon_box()
    box = pkmn.pokemon_box(pkmn.game.RUBY)
    pokemon_pc_tests.test_pokemon_box(box, pkmn.game.RUBY)
end

function test_ruby_pokemon_pc()
    pc = pkmn.pokemon_pc(pkmn.game.RUBY)
    pokemon_pc_tests.test_pokemon_pc(pc, pkmn.game.RUBY)
end

---
--- Sapphire
---

function test_sapphire_pokemon_box()
    box = pkmn.pokemon_box(pkmn.game.SAPPHIRE)
    pokemon_pc_tests.test_pokemon_box(box, pkmn.game.SAPPHIRE)
end

function test_sapphire_pokemon_pc()
    pc = pkmn.pokemon_pc(pkmn.game.SAPPHIRE)
    pokemon_pc_tests.test_pokemon_pc(pc, pkmn.game.SAPPHIRE)
end

---
--- Emerald
---

function test_emerald_pokemon_box()
    box = pkmn.pokemon_box(pkmn.game.EMERALD)
    pokemon_pc_tests.test_pokemon_box(box, pkmn.game.EMERALD)
end

function test_emerald_pokemon_pc()
    pc = pkmn.pokemon_pc(pkmn.game.EMERALD)
    pokemon_pc_tests.test_pokemon_pc(pc, pkmn.game.EMERALD)
end

---
--- FireRed
---

function test_firered_pokemon_box()
    box = pkmn.pokemon_box(pkmn.game.FIRERED)
    pokemon_pc_tests.test_pokemon_box(box, pkmn.game.FIRERED)
end

function test_firered_pokemon_pc()
    pc = pkmn.pokemon_pc(pkmn.game.FIRERED)
    pokemon_pc_tests.test_pokemon_pc(pc, pkmn.game.FIRERED)
end

---
--- LeafGreen
---

function test_leafgreen_pokemon_box()
    box = pkmn.pokemon_box(pkmn.game.LEAFGREEN)
    pokemon_pc_tests.test_pokemon_box(box, pkmn.game.LEAFGREEN)
end

function test_leafgreen_pokemon_pc()
    pc = pkmn.pokemon_pc(pkmn.game.LEAFGREEN)
    pokemon_pc_tests.test_pokemon_pc(pc, pkmn.game.LEAFGREEN)
end

---
--- Colosseum
---

function test_colosseum_pokemon_box()
    box = pkmn.pokemon_box(pkmn.game.COLOSSEUM)
    pokemon_pc_tests.test_pokemon_box(box, pkmn.game.COLOSSEUM)
end

function test_colosseum_pokemon_pc()
    pc = pkmn.pokemon_pc(pkmn.game.COLOSSEUM)
    pokemon_pc_tests.test_pokemon_pc(pc, pkmn.game.COLOSSEUM)
end

---
--- XD
---

function test_xd_pokemon_box()
    box = pkmn.pokemon_box(pkmn.game.XD)
    pokemon_pc_tests.test_pokemon_box(box, pkmn.game.XD)
end

function test_xd_pokemon_pc()
    pc = pkmn.pokemon_pc(pkmn.game.XD)
    pokemon_pc_tests.test_pokemon_pc(pc, pkmn.game.XD)
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
