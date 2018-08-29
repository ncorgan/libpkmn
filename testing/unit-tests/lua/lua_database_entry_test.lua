--
-- Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local pkmntest_utils = require("pkmntest_utils")

local luaunit = require("luaunit")

function test_item_entry()
    -- Make sure trying to create an invalid entry results in an error
    luaunit.assertError(pkmn.database.item_entry, pkmn.item.FAST_BALL, pkmn.game.NONE)
    luaunit.assertError(pkmn.database.item_entry, pkmn.item.FAST_BALL, pkmn.game.RED)

    local entry = pkmn.database.item_entry(pkmn.item.FAST_BALL, pkmn.game.SOULSILVER)

    luaunit.assertEquals(entry.item, pkmn.item.FAST_BALL)
    luaunit.assertEquals(entry.name, "Fast Ball")
    luaunit.assertEquals(entry.game, pkmn.game.SOULSILVER)
    luaunit.assertEquals(entry.category, "Apricorn balls")
    luaunit.assertEquals(entry.pocket, "Poké Balls")

    -- Just make sure it works.
    local description = entry.description

    luaunit.assertEquals(entry.cost, 300)
    luaunit.assertTrue(entry.is_holdable)
    luaunit.assertEquals(entry.fling_power, 0)
    luaunit.assertEquals(entry.fling_effect, "None")
end

function test_move_entry()
    -- Make sure trying to create an invalid entry results in an error
    luaunit.assertError(pkmn.database.move_entry, pkmn.move.OCTAZOOKA, pkmn.game.NONE)
    luaunit.assertError(pkmn.database.move_entry, pkmn.move.OCTAZOOKA, pkmn.game.RED)

    local entry = pkmn.database.move_entry(pkmn.move.OCTAZOOKA, pkmn.game.SILVER)

    luaunit.assertEquals(entry.move, pkmn.move.OCTAZOOKA)
    luaunit.assertEquals(entry.name, "Octazooka")
    luaunit.assertEquals(entry.game, pkmn.game.SILVER)
    luaunit.assertEquals(entry.type, pkmn.type.WATER)

    -- Just make sure it works.
    local description = entry.description

    luaunit.assertEquals(entry.target, pkmn.move_target.SELECTED_POKEMON)
    luaunit.assertEquals(entry.damage_class, pkmn.move_damage_class.SPECIAL)
    luaunit.assertEquals(entry.base_power, 65)

    local pps = {10, 12, 14, 16}
    for i = 1, #pps do
        luaunit.assertEquals(entry:get_pp(i-1), pps[i])
    end

    luaunit.assertAlmostEquals(entry.accuracy, 0.85, 0.0001)
    luaunit.assertEquals(entry.priority, 0)

    -- Just make sure it works.
    local effect = entry.effect

    luaunit.assertEquals(entry.contest_type, pkmn.contest_stat.NONE)
    luaunit.assertEquals(entry.contest_effect, "None")
    luaunit.assertEquals(entry.super_contest_effect, "None")
end

function test_pokemon_entry()
    -- Make sure trying to create an invalid entry results in an error
    luaunit.assertError(pkmn.database.pokemon_entry, pkmn.species.CASTFORM, pkmn.game.NONE, "Sunny")
    luaunit.assertError(pkmn.database.pokemon_entry, pkmn.species.CASTFORM, pkmn.game.BLACK_2, "Not a form")

    local entry = pkmn.database.pokemon_entry(pkmn.species.STUNFISK, pkmn.game.BLACK2, "")

    luaunit.assertEquals(entry.species, pkmn.species.STUNFISK)
    luaunit.assertEquals(entry.species_name, "Stunfisk")
    luaunit.assertEquals(entry.game, pkmn.game.BLACK2)
    luaunit.assertEquals(entry.category, "Trap")
    luaunit.assertEquals(entry.form, "Standard")
    luaunit.assertTrue(#entry.pokedex_entry > 0)
    luaunit.assertAlmostEquals(entry.height, 0.7, 0.0001)
    luaunit.assertAlmostEquals(entry.weight, 11.0, 0.0001)
    luaunit.assertAlmostEquals(entry.chance_male, 0.5, 0.0001)
    luaunit.assertAlmostEquals(entry.chance_female, 0.5, 0.0001)
    luaunit.assertFalse(entry.has_gender_differences)
    luaunit.assertEquals(entry.base_friendship, 70)

    luaunit.assertEquals(entry.types.first, pkmn.type.GROUND)
    luaunit.assertEquals(entry.types.second, pkmn.type.ELECTRIC)

    luaunit.assertEquals(entry.abilities.first, pkmn.ability.STATIC)
    luaunit.assertEquals(entry.abilities.second, pkmn.ability.LIMBER)

    luaunit.assertEquals(entry.hidden_ability, pkmn.ability.SAND_VEIL)

    luaunit.assertEquals(entry.egg_groups.first, pkmn.egg_group.WATER1)
    luaunit.assertEquals(entry.egg_groups.second, pkmn.egg_group.INDETERMINATE)

    luaunit.assertEquals(entry.base_stats[pkmn.stat.HP], 109)
    luaunit.assertEquals(entry.base_stats[pkmn.stat.ATTACK], 66)
    luaunit.assertEquals(entry.base_stats[pkmn.stat.DEFENSE], 84)
    luaunit.assertEquals(entry.base_stats[pkmn.stat.SPEED], 32)
    luaunit.assertEquals(entry.base_stats[pkmn.stat.SPECIAL_ATTACK], 81)
    luaunit.assertEquals(entry.base_stats[pkmn.stat.SPECIAL_DEFENSE], 99)

    luaunit.assertEquals(entry.EV_yields[pkmn.stat.HP], 2);
    luaunit.assertEquals(entry.EV_yields[pkmn.stat.ATTACK], 0);
    luaunit.assertEquals(entry.EV_yields[pkmn.stat.DEFENSE], 0);
    luaunit.assertEquals(entry.EV_yields[pkmn.stat.SPEED], 0);
    luaunit.assertEquals(entry.EV_yields[pkmn.stat.SPECIAL_ATTACK], 0);
    luaunit.assertEquals(entry.EV_yields[pkmn.stat.SPECIAL_DEFENSE], 0);

    luaunit.assertEquals(entry.experience_yield, 165)
    luaunit.assertEquals(entry:get_experience_at_level(50), 125000)
    luaunit.assertEquals(entry:get_level_at_experience(200000), 58)
    luaunit.assertTrue(#entry.levelup_moves > 0)
    luaunit.assertTrue(#entry.tm_hm_moves > 0)
    luaunit.assertTrue(#entry.egg_moves > 0)
    luaunit.assertTrue(#entry.tutor_moves > 0)
    luaunit.assertEquals(#entry.evolutions, 0)

    luaunit.assertTrue(pkmntest_utils.file_exists(entry:get_icon_filepath(false)))
    luaunit.assertTrue(pkmntest_utils.file_exists(entry:get_icon_filepath(true)))
    luaunit.assertTrue(pkmntest_utils.file_exists(entry:get_sprite_filepath(false,false)))
    luaunit.assertTrue(pkmntest_utils.file_exists(entry:get_sprite_filepath(false,true)))
    luaunit.assertTrue(pkmntest_utils.file_exists(entry:get_sprite_filepath(true,false)))
    luaunit.assertTrue(pkmntest_utils.file_exists(entry:get_sprite_filepath(true,true)))
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
