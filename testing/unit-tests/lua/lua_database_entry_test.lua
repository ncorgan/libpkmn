--
-- Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local luaunit = require("luaunit")
local pkmn = require("pkmn")

function test_item_entry()
    -- Make sure trying to create an invalid entry results in an error
    luaunit.assertError(pkmn.database.item_entry, "Not an item", "SoulSilver")
    luaunit.assertError(pkmn.database.item_entry, "Fast Ball", "Not a game")
    luaunit.assertError(pkmn.database.item_entry, "Fast Ball", "Red")

    local entry = pkmn.database.item_entry("Fast Ball", "SoulSilver")

    luaunit.assertEquals(entry.name, "Fast Ball")
    luaunit.assertEquals(entry.game, "SoulSilver")
    luaunit.assertEquals(entry.category, "Apricorn balls")
    luaunit.assertEquals(entry.pocket, "Poké Balls")

    -- Just make sure it works.
    local description = entry.description

    luaunit.assertEquals(entry.cost, 300)
    luaunit.assertTrue(entry.is_holdable)
    luaunit.assertEquals(entry.fling_power, 0)
    luaunit.assertEquals(entry.fling_effect, "None")

    -- Test equality.
    local entry_same = pkmn.database.item_entry("Fast Ball", "SoulSilver")
    local entry_different_item = pkmn.database.item_entry("Potion", "SoulSilver")
    local entry_different_game = pkmn.database.item_entry("Fast Ball", "HeartGold")

    luaunit.assertEquals(entry, entry_same)
    luaunit.assertNotEquals(entry, entry_different_item)
    luaunit.assertNotEquals(entry, entry_different_game)
end

function test_move_entry()
    -- Make sure trying to create an invalid entry results in an error
    luaunit.assertError(pkmn.database.move_entry, "Not a move", "Silver")
    luaunit.assertError(pkmn.database.move_entry, "Octazooka", "Not a game")
    luaunit.assertError(pkmn.database.move_entry, "Octazooka", "Red")

    local entry = pkmn.database.move_entry("Octazooka", "Silver")

    luaunit.assertEquals(entry.name, "Octazooka");
    luaunit.assertEquals(entry.game, "Silver")
    luaunit.assertEquals(entry.type, "Water")

    -- Just make sure it works.
    local description = entry.description

    luaunit.assertEquals(entry.target, "Selected Pokémon")
    luaunit.assertEquals(entry.damage_class, "Special")
    luaunit.assertEquals(entry.base_power, 65)

    local pps = {10, 12, 14, 16}
    for i = 1, #pps do
        luaunit.assertEquals(entry:get_pp(i-1), pps[i])
    end

    luaunit.assertAlmostEquals(entry.accuracy, 0.85, 0.0001)
    luaunit.assertEquals(entry.priority, 0)

    -- Just make sure it works.
    local effect = entry.effect

    luaunit.assertEquals(entry.contest_type, "None")
    luaunit.assertEquals(entry.contest_effect, "None")
    luaunit.assertEquals(entry.super_contest_effect, "None")

    -- Test equality.
    local entry_same = pkmn.database.move_entry("Octazooka", "Silver")
    local entry_different_move = pkmn.database.move_entry("Hydro Pump", "Silver")
    local entry_different_game = pkmn.database.move_entry("Octazooka", "Gold")

    luaunit.assertEquals(entry, entry_same)
    luaunit.assertNotEquals(entry, entry_different_move)
    luaunit.assertNotEquals(entry, entry_different_game)
end

-- http://stackoverflow.com/a/4991602
function file_exists(name)
    local f=io.open(name,"r")
    if f~=nil then io.close(f) return true else return false end
end

function test_pokemon_entry()
    -- Make sure trying to create an invalid entry results in an error
    luaunit.assertError(pkmn.database.pokemon_entry, "Invalid", "Black 2, Sunny")
    luaunit.assertError(pkmn.database.pokemon_entry, "Castform", "Not a game", "Sunny")
    luaunit.assertError(pkmn.database.pokemon_entry, "Castform", "Black 2", "Not a form")

    local entry = pkmn.database.pokemon_entry("Stunfisk", "Black 2", "")

    luaunit.assertEquals(entry.name, "Stunfisk")
    luaunit.assertEquals(entry.game, "Black 2")
    luaunit.assertEquals(entry.species, "Trap")
    luaunit.assertEquals(entry.form, "Standard")
    luaunit.assertTrue(#entry.pokedex_entry > 0)
    luaunit.assertAlmostEquals(entry.height, 0.7, 0.0001)
    luaunit.assertAlmostEquals(entry.weight, 11.0, 0.0001)
    luaunit.assertAlmostEquals(entry.chance_male, 0.5, 0.0001)
    luaunit.assertAlmostEquals(entry.chance_female, 0.5, 0.0001)
    luaunit.assertFalse(entry.has_gender_differences)
    luaunit.assertEquals(entry.base_friendship, 70)

    luaunit.assertEquals(entry.types.first, "Ground")
    luaunit.assertEquals(entry.types.second, "Electric")

    luaunit.assertEquals(entry.abilities.first, "Static")
    luaunit.assertEquals(entry.abilities.second, "Limber")

    luaunit.assertEquals(entry.hidden_ability, "Sand Veil")

    luaunit.assertEquals(entry.egg_groups.first, "Water 1")
    luaunit.assertEquals(entry.egg_groups.second, "Amorphous")

    luaunit.assertEquals(entry.base_stats["HP"], 109)
    luaunit.assertEquals(entry.base_stats["Attack"], 66)
    luaunit.assertEquals(entry.base_stats["Defense"], 84)
    luaunit.assertEquals(entry.base_stats["Speed"], 32)
    luaunit.assertEquals(entry.base_stats["Special Attack"], 81)
    luaunit.assertEquals(entry.base_stats["Special Defense"], 99)

    luaunit.assertEquals(entry.EV_yields["HP"], 2);
    luaunit.assertEquals(entry.EV_yields["Attack"], 0);
    luaunit.assertEquals(entry.EV_yields["Defense"], 0);
    luaunit.assertEquals(entry.EV_yields["Speed"], 0);
    luaunit.assertEquals(entry.EV_yields["Special Attack"], 0);
    luaunit.assertEquals(entry.EV_yields["Special Defense"], 0);

    luaunit.assertEquals(entry.experience_yield, 165)
    luaunit.assertEquals(entry:get_experience_at_level(50), 125000)
    luaunit.assertEquals(entry:get_level_at_experience(200000), 58)
    luaunit.assertTrue(#entry.levelup_moves > 0)
    luaunit.assertTrue(#entry.tm_hm_moves > 0)
    luaunit.assertTrue(#entry.egg_moves > 0)
    luaunit.assertTrue(#entry.tutor_moves > 0)
    luaunit.assertEquals(#entry.evolutions, 0)

    luaunit.assertTrue(file_exists(entry:get_icon_filepath(false)))
    luaunit.assertTrue(file_exists(entry:get_icon_filepath(true)))
    luaunit.assertTrue(file_exists(entry:get_sprite_filepath(false,false)))
    luaunit.assertTrue(file_exists(entry:get_sprite_filepath(false,true)))
    luaunit.assertTrue(file_exists(entry:get_sprite_filepath(true,false)))
    luaunit.assertTrue(file_exists(entry:get_sprite_filepath(true,true)))

    -- Use different Pokémon for testing (in)equality.
    local entry_first = pkmn.database.pokemon_entry("Pikachu", "Omega Ruby", "Standard")
    local entry_same = pkmn.database.pokemon_entry("Pikachu", "Omega Ruby", "Standard")
    local entry_different_pokemon = pkmn.database.pokemon_entry("Pichu", "Omega Ruby", "Standard")
    local entry_different_game = pkmn.database.pokemon_entry("Pikachu", "Alpha Sapphire", "Standard")
    local entry_different_form = pkmn.database.pokemon_entry("Pikachu", "Omega Ruby", "Belle")

    luaunit.assertEquals(entry_first, entry_same)
    luaunit.assertNotEquals(entry_first, entry_different_pokemon)
    luaunit.assertNotEquals(entry_first, entry_different_game)
    luaunit.assertNotEquals(entry_first, entry_different_form)
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
