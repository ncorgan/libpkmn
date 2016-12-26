--
-- Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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
    luaunit.assertEquals(ability_list[0], "Adaptability")
    luaunit.assertEquals(ability_list[190], "Zen Mode")
end

function test_game_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_game_list, 7, false)

    local game_list = pkmn.database.get_game_list(6, true)
    luaunit.assertEquals(#game_list, 26)
    luaunit.assertEquals(game_list[0], "Red")
    luaunit.assertEquals(game_list[25], "Alpha Sapphire")
end

function test_item_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_item_list, "Not a game")

    local item_list = pkmn.database.get_item_list("HeartGold")
    luaunit.assertEquals(#item_list, 513)
    luaunit.assertEquals(item_list[0], "Adamant Orb")
    luaunit.assertEquals(item_list[512], "Zoom Lens")
end

function test_location_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_location_list, "Not a game", false)

    local location_list = pkmn.database.get_location_list("Emerald", false)
    luaunit.assertEquals(#location_list, 106)
    luaunit.assertEquals(location_list[0], "Abandoned Ship")
    luaunit.assertEquals(location_list[105], "Victory Road")
end

function test_move_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_move_list, "Not a game")

    local move_list = pkmn.database.get_move_list("Red")
    luaunit.assertEquals(#move_list, 165)
    luaunit.assertEquals(move_list[0], "Pound")
    luaunit.assertEquals(move_list[164], "Struggle")
end

function test_nature_list()
    local nature_list = pkmn.database.get_nature_list()
    luaunit.assertEquals(#nature_list, 25)
    luaunit.assertEquals(nature_list[0], "Adamant")
    luaunit.assertEquals(nature_list[24], "Timid")
end

function test_pokemon_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_pokemon_list, 0, true)

    local pokemon_list = pkmn.database.get_pokemon_list(1, true)
    luaunit.assertEquals(#pokemon_list, 151)
    luaunit.assertEquals(pokemon_list[0], "Bulbasaur")
    luaunit.assertEquals(pokemon_list[150], "Mew")
end

function test_region_list()
    local region_list = pkmn.database.get_region_list()
    luaunit.assertEquals(#region_list, 7)
    luaunit.assertEquals(region_list[0], "Kanto")
    luaunit.assertEquals(region_list[6], "Kalos")
end

-- Unimplemented
-- function test_ribbon_list()
-- end

function test_super_training_medal_list()
    local super_training_medal_list = pkmn.database.get_super_training_medal_list()
    luaunit.assertEquals(#super_training_medal_list, 30)
    luaunit.assertEquals(super_training_medal_list[0], "Sp. Atk Level 1")
    luaunit.assertEquals(super_training_medal_list[29], "The Battle for the Best!")
end

function test_type_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_type_list, "Not a game")

    local type_list = pkmn.database.get_type_list("Alpha Sapphire")
    luaunit.assertEquals(#type_list, 18)
    luaunit.assertEquals(type_list[0], "Normal")
    luaunit.assertEquals(type_list[17], "Fairy")
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
