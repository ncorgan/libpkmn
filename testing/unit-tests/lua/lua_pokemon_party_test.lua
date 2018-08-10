--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")

local pokemon_party_tests = require("pokemon_party_tests")

---
--- Red
---

function test_red_pokemon_party()
    party = pkmn.pokemon_party(pkmn.game.RED)
    pokemon_party_tests.test_pokemon_party(party, pkmn.game.RED)
end

---
--- Blue
---

function test_blue_pokemon_party()
    party = pkmn.pokemon_party(pkmn.game.BLUE)
    pokemon_party_tests.test_pokemon_party(party, pkmn.game.BLUE)
end

---
--- Yellow
---

function test_yellow_pokemon_party()
    party = pkmn.pokemon_party(pkmn.game.YELLOW)
    pokemon_party_tests.test_pokemon_party(party, pkmn.game.YELLOW)
end

---
--- Gold
---

function test_gold_pokemon_party()
    party = pkmn.pokemon_party(pkmn.game.GOLD)
    pokemon_party_tests.test_pokemon_party(party, pkmn.game.GOLD)
end

---
--- Silver
---

function test_silver_pokemon_party()
    party = pkmn.pokemon_party(pkmn.game.SILVER)
    pokemon_party_tests.test_pokemon_party(party, pkmn.game.SILVER)
end

---
--- Crystal
---

function test_crystal_pokemon_party()
    party = pkmn.pokemon_party(pkmn.game.CRYSTAL)
    pokemon_party_tests.test_pokemon_party(party, pkmn.game.CRYSTAL)
end

---
--- Ruby
---

function test_ruby_pokemon_party()
    party = pkmn.pokemon_party(pkmn.game.RUBY)
    pokemon_party_tests.test_pokemon_party(party, pkmn.game.RUBY)
end

---
--- Sapphire
---

function test_sapphire_pokemon_party()
    party = pkmn.pokemon_party(pkmn.game.SAPPHIRE)
    pokemon_party_tests.test_pokemon_party(party, pkmn.game.SAPPHIRE)
end

---
--- Emerald
---

function test_emerald_pokemon_party()
    party = pkmn.pokemon_party(pkmn.game.EMERALD)
    pokemon_party_tests.test_pokemon_party(party, pkmn.game.EMERALD)
end

---
--- FireRed
---

function test_firered_pokemon_party()
    party = pkmn.pokemon_party(pkmn.game.FIRERED)
    pokemon_party_tests.test_pokemon_party(party, pkmn.game.FIRERED)
end

---
--- LeafGreen
---

function test_leafgreen_pokemon_party()
    party = pkmn.pokemon_party(pkmn.game.LEAFGREEN)
    pokemon_party_tests.test_pokemon_party(party, pkmn.game.LEAFGREEN)
end

---
--- Colosseum
---

function test_colosseum_pokemon_party()
    party = pkmn.pokemon_party(pkmn.game.COLOSSEUM)
    pokemon_party_tests.test_pokemon_party(party, pkmn.game.COLOSSEUM)
end

---
--- XD
---

function test_xd_pokemon_party()
    party = pkmn.pokemon_party(pkmn.game.XD)
    pokemon_party_tests.test_pokemon_party(party, pkmn.game.XD)
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
