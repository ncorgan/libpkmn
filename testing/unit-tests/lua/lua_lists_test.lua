--
-- Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local luaunit = require("luaunit")
local pkmn = require("pkmn")

function test_ability_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_ability_list, 7)

    local ability_list = pkmn.database.get_ability_list(6)
    luaunit.assertEquals(#ability_list, 191)
    luaunit.assertEquals(ability_list[1], "Adaptability")
    luaunit.assertEquals(ability_list[191], "Zen Mode")
end

function test_game_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_game_list, 7, false)

    local game_list = pkmn.database.get_game_list(6, true)
    luaunit.assertEquals(#game_list, 26)
    luaunit.assertEquals(game_list[1], "Red")
    luaunit.assertEquals(game_list[26], "Alpha Sapphire")
end

function test_gamecube_shadow_pokemon_list()
    local colosseum_shadow_pokemon_list = pkmn.database.get_gamecube_shadow_pokemon_list(true)
    luaunit.assertEquals(#colosseum_shadow_pokemon_list, 48)

    local xd_shadow_pokemon_list = pkmn.database.get_gamecube_shadow_pokemon_list(false)
    luaunit.assertEquals(#xd_shadow_pokemon_list, 83)
end

function test_hm_move_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_hm_move_list, "Not a game")

    local hm_move_list = pkmn.database.get_hm_move_list("Red")
    luaunit.assertEquals(#hm_move_list, 5)
    luaunit.assertEquals(hm_move_list[1], "Cut")
    luaunit.assertEquals(hm_move_list[5], "Flash")
end

function test_item_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_item_list, "Not a game")

    local item_list = pkmn.database.get_item_list("HeartGold")
    luaunit.assertEquals(#item_list, 513)
    luaunit.assertEquals(item_list[1], "Adamant Orb")
    luaunit.assertEquals(item_list[513], "Zoom Lens")
end

function test_location_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_location_list, "Not a game", false)

    local location_list = pkmn.database.get_location_list("Emerald", false)
    luaunit.assertEquals(#location_list, 106)
    luaunit.assertEquals(location_list[1], "Abandoned Ship")
    luaunit.assertEquals(location_list[106], "Victory Road")
end

function test_move_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_move_list, "Not a game")

    local move_list = pkmn.database.get_move_list("Red")
    luaunit.assertEquals(#move_list, 165)
    luaunit.assertEquals(move_list[1], "Pound")
    luaunit.assertEquals(move_list[165], "Struggle")
end

function test_nature_list()
    local nature_list = pkmn.database.get_nature_list()
    luaunit.assertEquals(#nature_list, 25)
    luaunit.assertEquals(nature_list[1], "Hardy")
    luaunit.assertEquals(nature_list[25], "Quirky")
end

function test_pokemon_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_pokemon_list, 0, true)

    local pokemon_list = pkmn.database.get_pokemon_list(1, true)
    luaunit.assertEquals(#pokemon_list, 151)
    luaunit.assertEquals(pokemon_list[1], "Bulbasaur")
    luaunit.assertEquals(pokemon_list[151], "Mew")
end

function test_region_list()
    local region_list = pkmn.database.get_region_list()
    luaunit.assertEquals(#region_list, 7)
    luaunit.assertEquals(region_list[1], "Kanto")
    luaunit.assertEquals(region_list[7], "Kalos")
end

-- Unimplemented
-- function test_ribbon_list()
-- end

function test_super_training_medal_list()
    local super_training_medal_list = pkmn.database.get_super_training_medal_list()
    luaunit.assertEquals(#super_training_medal_list, 30)
    luaunit.assertEquals(super_training_medal_list[1], "Sp. Atk Level 1")
    luaunit.assertEquals(super_training_medal_list[30], "The Battle for the Best!")
end

function test_tm_move_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_tm_move_list, "Not a game")

    local tm_move_list = pkmn.database.get_tm_move_list("Red")
    luaunit.assertEquals(#tm_move_list, 50)
    luaunit.assertEquals(tm_move_list[1], "Mega Punch")
    luaunit.assertEquals(tm_move_list[50], "Substitute")
end

function test_type_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_type_list, "Not a game")

    local type_list = pkmn.database.get_type_list("Alpha Sapphire")
    luaunit.assertEquals(#type_list, 18)
    luaunit.assertEquals(type_list[1], "Normal")
    luaunit.assertEquals(type_list[18], "Fairy")
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
