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

    luaunit.assertEquals(entry:get_name(), "Fast Ball")
    luaunit.assertEquals(entry:get_game(), "SoulSilver")
    luaunit.assertEquals(entry:get_category(), "Apricorn balls")
    luaunit.assertEquals(entry:get_pocket(), "Poké Balls")

    -- Just make sure it works.
    entry:get_description()

    luaunit.assertEquals(entry:get_cost(), 300)
    luaunit.assertTrue(entry:holdable())
    luaunit.assertEquals(entry:get_fling_power(), 0)
    luaunit.assertEquals(entry:get_fling_effect(), "None")

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

    luaunit.assertEquals(entry:get_name(), "Octazooka");
    luaunit.assertEquals(entry:get_game(), "Silver")
    luaunit.assertEquals(entry:get_type(), "Water")

    -- Just make sure it works.
    entry:get_description()

    luaunit.assertEquals(entry:get_target(), "Selected Pokémon")
    luaunit.assertEquals(entry:get_damage_class(), "Special")
    luaunit.assertEquals(entry:get_base_power(), 65)

    local pps = {10, 12, 14, 16}
    for i = 1, #pps do
        luaunit.assertEquals(entry:get_pp(i-1), pps[i])
    end

    luaunit.assertAlmostEquals(entry:get_accuracy(), 0.85, 0.0001)
    luaunit.assertEquals(entry:get_priority(), 0)

    -- Just make sure it works.
    entry:get_effect()

    luaunit.assertEquals(entry:get_contest_type(), "None")
    luaunit.assertEquals(entry:get_contest_effect(), "None")
    luaunit.assertEquals(entry:get_super_contest_effect(), "None")

    -- Test equality.
    local entry_same = pkmn.database.move_entry("Octazooka", "Silver")
    local entry_different_move = pkmn.database.move_entry("Hydro Pump", "Silver")
    local entry_different_game = pkmn.database.move_entry("Octazooka", "Gold")

    luaunit.assertEquals(entry, entry_same)
    luaunit.assertNotEquals(entry, entry_different_move)
    luaunit.assertNotEquals(entry, entry_different_game)
end

function test_pokemon_entry()
    -- Make sure trying to create an invalid entry results in an error
    luaunit.assertError(pkmn.database.pokemon_entry, "Invalid", "Black 2, Sunny")
    luaunit.assertError(pkmn.database.pokemon_entry, "Castform", "Not a game", "Sunny")
    luaunit.assertError(pkmn.database.pokemon_entry, "Castform", "Black 2", "Not a form")

    local entry = pkmn.database.pokemon_entry("Stunfisk", "Black 2", "")

    luaunit.assertEquals(entry:get_name(), "Stunfisk")
    luaunit.assertEquals(entry:get_game(), "Black 2")
    luaunit.assertEquals(entry:get_species(), "Trap")
    luaunit.assertEquals(entry:get_form(), "Standard")
    luaunit.assertTrue(#entry:get_pokedex_entry() > 0)
    luaunit.assertAlmostEquals(entry:get_height(), 0.7, 0.0001)
    luaunit.assertAlmostEquals(entry:get_weight(), 11.0, 0.0001)
    luaunit.assertAlmostEquals(entry:get_chance_male(), 0.5, 0.0001)
    luaunit.assertAlmostEquals(entry:get_chance_female(), 0.5, 0.0001)
    luaunit.assertFalse(entry:has_gender_differences())
    luaunit.assertEquals(entry:get_base_happiness(), 70)

    local types = entry:get_types()
    luaunit.assertEquals(types.first, "Ground")
    luaunit.assertEquals(types.second, "Electric")

    local abilities = entry:get_abilities()
    luaunit.assertEquals(abilities.first, "Static")
    luaunit.assertEquals(abilities.second, "Limber")

    luaunit.assertEquals(entry:get_hidden_ability(), "Sand Veil")

    local egg_groups = entry:get_egg_groups()
    luaunit.assertEquals(egg_groups.first, "Water 1")
    luaunit.assertEquals(egg_groups.second, "Amorphous")

    local base_stats = entry:get_base_stats()
    luaunit.assertEquals(base_stats["HP"], 109)
    luaunit.assertEquals(base_stats["Attack"], 66)
    luaunit.assertEquals(base_stats["Defense"], 84)
    luaunit.assertEquals(base_stats["Speed"], 32)
    luaunit.assertEquals(base_stats["Special Attack"], 81)
    luaunit.assertEquals(base_stats["Special Defense"], 99)

    local EV_yields = entry:get_EV_yields()
    luaunit.assertEquals(EV_yields["HP"], 2);
    luaunit.assertEquals(EV_yields["Attack"], 0);
    luaunit.assertEquals(EV_yields["Defense"], 0);
    luaunit.assertEquals(EV_yields["Speed"], 0);
    luaunit.assertEquals(EV_yields["Special Attack"], 0);
    luaunit.assertEquals(EV_yields["Special Defense"], 0);

    luaunit.assertEquals(entry:get_experience_yield(), 165)
    luaunit.assertEquals(entry:get_experience_at_level(50), 125000)
    luaunit.assertEquals(entry:get_level_at_experience(200000), 58)
    luaunit.assertTrue(#entry:get_levelup_moves() > 0)
    luaunit.assertTrue(#entry:get_tm_hm_moves() > 0)
    luaunit.assertTrue(#entry:get_egg_moves() > 0)
    luaunit.assertTrue(#entry:get_tutor_moves() > 0)
    luaunit.assertEquals(#entry:get_evolutions(), 0)

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
