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
    party = pkmn.pokemon_party("Red")
    pokemon_party_tests.test_pokemon_party(party, "Red")
end

---
--- Blue
---

function test_blue_pokemon_party()
    party = pkmn.pokemon_party("Blue")
    pokemon_party_tests.test_pokemon_party(party, "Blue")
end

---
--- Yellow
---

function test_yellow_pokemon_party()
    party = pkmn.pokemon_party("Yellow")
    pokemon_party_tests.test_pokemon_party(party, "Yellow")
end

---
--- Gold
---

function test_gold_pokemon_party()
    party = pkmn.pokemon_party("Gold")
    pokemon_party_tests.test_pokemon_party(party, "Gold")
end

---
--- Silver
---

function test_silver_pokemon_party()
    party = pkmn.pokemon_party("Silver")
    pokemon_party_tests.test_pokemon_party(party, "Silver")
end

---
--- Crystal
---

function test_crystal_pokemon_party()
    party = pkmn.pokemon_party("Crystal")
    pokemon_party_tests.test_pokemon_party(party, "Crystal")
end

---
--- Ruby
---

function test_ruby_pokemon_party()
    party = pkmn.pokemon_party("Ruby")
    pokemon_party_tests.test_pokemon_party(party, "Ruby")
end

---
--- Sapphire
---

function test_sapphire_pokemon_party()
    party = pkmn.pokemon_party("Sapphire")
    pokemon_party_tests.test_pokemon_party(party, "Sapphire")
end

---
--- Emerald
---

function test_emerald_pokemon_party()
    party = pkmn.pokemon_party("Emerald")
    pokemon_party_tests.test_pokemon_party(party, "Emerald")
end

---
--- FireRed
---

function test_firered_pokemon_party()
    party = pkmn.pokemon_party("FireRed")
    pokemon_party_tests.test_pokemon_party(party, "FireRed")
end

---
--- LeafGreen
---

function test_leafgreen_pokemon_party()
    party = pkmn.pokemon_party("LeafGreen")
    pokemon_party_tests.test_pokemon_party(party, "LeafGreen")
end

---
--- Colosseum
---

function test_colosseum_pokemon_party()
    party = pkmn.pokemon_party("Colosseum")
    pokemon_party_tests.test_pokemon_party(party, "Colosseum")
end

---
--- XD
---

function test_xd_pokemon_party()
    party = pkmn.pokemon_party("XD")
    pokemon_party_tests.test_pokemon_party(party, "XD")
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
