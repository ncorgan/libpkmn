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
    luaunit.assertError(pkmn.database.get_ability_list, 0)
    luaunit.assertError(pkmn.database.get_ability_name_list, 0)

    local ability_list = pkmn.database.get_ability_list(6)
    luaunit.assertEquals(#ability_list, 191)
    luaunit.assertEquals(ability_list[1], pkmn.ability.STENCH)
    luaunit.assertEquals(ability_list[191], pkmn.ability.DELTA_STREAM)

    local ability_name_list = pkmn.database.get_ability_name_list(6)
    luaunit.assertEquals(#ability_name_list, 191)
    luaunit.assertEquals(ability_name_list[1], "Stench")
    luaunit.assertEquals(ability_name_list[191], "Delta Stream")

    for ability_index = 1, #ability_list
    do
        luaunit.assertEquals(
            pkmn.string_to_ability(ability_name_list[ability_index]),
            ability_list[ability_index]
        )
    end
end

function test_game_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_game_list, 0, false)
    luaunit.assertError(pkmn.database.get_game_name_list, 0, false)

    local game_list = pkmn.database.get_game_list(6, true)
    luaunit.assertEquals(#game_list, 26)
    luaunit.assertEquals(game_list[1], pkmn.game.RED)
    luaunit.assertEquals(game_list[26], pkmn.game.ALPHA_SAPPHIRE)

    local game_name_list = pkmn.database.get_game_name_list(6, true)
    luaunit.assertEquals(#game_name_list, 26)
    luaunit.assertEquals(game_name_list[1], "Red")
    luaunit.assertEquals(game_name_list[26], "Alpha Sapphire")

    for game_index = 1, #game_list
    do
        luaunit.assertEquals(
            pkmn.string_to_game(game_name_list[game_index]),
            game_list[game_index]
        )
    end
end

function test_gamecube_shadow_pokemon_list()
    local colosseum_shadow_pokemon_list = pkmn.database.get_gamecube_shadow_pokemon_list(true)
    luaunit.assertEquals(#colosseum_shadow_pokemon_list, 48)

    local colosseum_shadow_pokemon_name_list = pkmn.database.get_gamecube_shadow_pokemon_name_list(true)
    luaunit.assertEquals(#colosseum_shadow_pokemon_name_list, 48)

    for species_index = 1, #colosseum_shadow_pokemon_list
    do
        luaunit.assertEquals(
            pkmn.string_to_species(colosseum_shadow_pokemon_name_list[species_index]),
            colosseum_shadow_pokemon_list[species_index]
        )
    end

    local xd_shadow_pokemon_list = pkmn.database.get_gamecube_shadow_pokemon_list(false)
    luaunit.assertEquals(#xd_shadow_pokemon_list, 83)

    local xd_shadow_pokemon_name_list = pkmn.database.get_gamecube_shadow_pokemon_name_list(false)
    luaunit.assertEquals(#xd_shadow_pokemon_name_list, 83)

    for species_index = 1, #xd_shadow_pokemon_list
    do
        luaunit.assertEquals(
            pkmn.string_to_species(xd_shadow_pokemon_name_list[species_index]),
            xd_shadow_pokemon_list[species_index]
        )
    end
end

function test_hm_move_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_hm_move_list, pkmn.game.NONE)
    luaunit.assertError(pkmn.database.get_hm_move_name_list, pkmn.game.NONE)

    local hm_move_list = pkmn.database.get_hm_move_list(pkmn.game.RED)
    luaunit.assertEquals(#hm_move_list, 5)
    luaunit.assertEquals(hm_move_list[1], pkmn.move.CUT)
    luaunit.assertEquals(hm_move_list[5], pkmn.move.FLASH)

    local hm_move_name_list = pkmn.database.get_hm_move_name_list(pkmn.game.RED)
    luaunit.assertEquals(#hm_move_name_list, 5)
    luaunit.assertEquals(hm_move_name_list[1], "Cut")
    luaunit.assertEquals(hm_move_name_list[5], "Flash")

    for move_index = 1, #hm_move_list
    do
        luaunit.assertEquals(
            pkmn.string_to_move(hm_move_name_list[move_index]),
            hm_move_list[move_index]
        )
    end
end

function test_item_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_item_list, pkmn.game.NONE)
    luaunit.assertError(pkmn.database.get_item_name_list, pkmn.game.NONE)

    local item_list = pkmn.database.get_item_list(pkmn.game.HEARTGOLD)
    luaunit.assertEquals(#item_list, 513)
    luaunit.assertEquals(item_list[1], pkmn.item.MASTER_BALL)
    luaunit.assertEquals(item_list[513], pkmn.item.PHOTO_ALBUM)

    local item_name_list = pkmn.database.get_item_name_list(pkmn.game.HEARTGOLD)
    luaunit.assertEquals(#item_name_list, 513)
    luaunit.assertEquals(item_name_list[1], "Master Ball")
    luaunit.assertEquals(item_name_list[513], "Photo Album")

    for item_index = 1, #item_list
    do
        luaunit.assertEquals(
            pkmn.string_to_item(item_name_list[item_index]),
            item_list[item_index]
        )
    end
end

function test_location_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_location_name_list, pkmn.game.NONE, false)

    local location_name_list = pkmn.database.get_location_name_list(pkmn.game.EMERALD, false)
    luaunit.assertEquals(#location_name_list, 106)
    luaunit.assertEquals(location_name_list[1], "Abandoned Ship")
    luaunit.assertEquals(location_name_list[106], "Victory Road")
end

function test_move_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_move_list, pkmn.game.NONE)
    luaunit.assertError(pkmn.database.get_move_name_list, pkmn.game.NONE)

    local move_list = pkmn.database.get_move_list(pkmn.game.RED)
    luaunit.assertEquals(#move_list, 165)
    luaunit.assertEquals(move_list[1], pkmn.move.POUND)
    luaunit.assertEquals(move_list[165], pkmn.move.STRUGGLE)

    local move_name_list = pkmn.database.get_move_name_list(pkmn.game.RED)
    luaunit.assertEquals(#move_name_list, 165)
    luaunit.assertEquals(move_name_list[1], "Pound")
    luaunit.assertEquals(move_name_list[165], "Struggle")

    for move_index = 1, #move_list
    do
        luaunit.assertEquals(
            pkmn.string_to_move(move_name_list[move_index]),
            move_list[move_index]
        )
    end
end

function test_nature_list()
    local nature_list = pkmn.database.get_nature_list()
    luaunit.assertEquals(#nature_list, 25)
    luaunit.assertEquals(nature_list[1], pkmn.nature.HARDY)
    luaunit.assertEquals(nature_list[25], pkmn.nature.QUIRKY)

    local nature_name_list = pkmn.database.get_nature_name_list()
    luaunit.assertEquals(#nature_name_list, 25)
    luaunit.assertEquals(nature_name_list[1], "Hardy")
    luaunit.assertEquals(nature_name_list[25], "Quirky")

    for nature_index = 1, #nature_list
    do
        luaunit.assertEquals(
            pkmn.string_to_nature(nature_name_list[nature_index]),
            nature_list[nature_index]
        )
    end
end

function test_pokemon_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_pokemon_list, 0, true)
    luaunit.assertError(pkmn.database.get_pokemon_name_list, 0, true)

    local pokemon_list = pkmn.database.get_pokemon_list(1, true)
    luaunit.assertEquals(#pokemon_list, 151)
    luaunit.assertEquals(pokemon_list[1], pkmn.species.BULBASAUR)
    luaunit.assertEquals(pokemon_list[151], pkmn.species.MEW)

    local pokemon_name_list = pkmn.database.get_pokemon_name_list(1, true)
    luaunit.assertEquals(#pokemon_name_list, 151)
    luaunit.assertEquals(pokemon_name_list[1], "Bulbasaur")
    luaunit.assertEquals(pokemon_name_list[151], "Mew")

    for pokemon_index = 1, #pokemon_list
    do
        luaunit.assertEquals(
            pkmn.string_to_species(pokemon_name_list[pokemon_index]),
            pokemon_list[pokemon_index]
        )
    end
end

function test_region_list()
    local region_name_list = pkmn.database.get_region_name_list()
    luaunit.assertEquals(#region_name_list, 7)
    luaunit.assertEquals(region_name_list[1], "Kanto")
    luaunit.assertEquals(region_name_list[7], "Kalos")
end

function test_ribbon_list()
    -- Make sure invalid generations are caught.
    luaunit.assertError(pkmn.database.get_ribbon_name_list, 2)
    luaunit.assertError(pkmn.database.get_ribbon_name_list, 7)

    local ribbon_name_list = pkmn.database.get_ribbon_name_list(3)
    luaunit.assertEquals(#ribbon_name_list, 32)
end

function test_super_training_medal_list()
    local super_training_medal_name_list = pkmn.database.get_super_training_medal_name_list()
    luaunit.assertEquals(#super_training_medal_name_list, 30)
    luaunit.assertEquals(super_training_medal_name_list[1], "Sp. Atk Level 1")
    luaunit.assertEquals(super_training_medal_name_list[30], "The Battle for the Best!")
end

function test_tm_move_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_tm_move_list, pkmn.game.NONE)
    luaunit.assertError(pkmn.database.get_tm_move_name_list, pkmn.game.NONE)

    local tm_move_list = pkmn.database.get_tm_move_list(pkmn.game.RED)
    luaunit.assertEquals(#tm_move_list, 50)
    luaunit.assertEquals(tm_move_list[1], pkmn.move.MEGA_PUNCH)
    luaunit.assertEquals(tm_move_list[50], pkmn.move.SUBSTITUTE)

    local tm_move_name_list = pkmn.database.get_tm_move_name_list(pkmn.game.RED)
    luaunit.assertEquals(#tm_move_name_list, 50)
    luaunit.assertEquals(tm_move_name_list[1], "Mega Punch")
    luaunit.assertEquals(tm_move_name_list[50], "Substitute")

    for move_index = 1, #tm_move_list
    do
        luaunit.assertEquals(
            pkmn.string_to_move(tm_move_name_list[move_index]),
            tm_move_list[move_index]
        )
    end
end

function test_type_list()
    -- Make sure trying to create an invalid list results in an error
    luaunit.assertError(pkmn.database.get_type_list, pkmn.game.NONE)
    luaunit.assertError(pkmn.database.get_type_name_list, pkmn.game.NONE)

    local type_list = pkmn.database.get_type_list(pkmn.game.ALPHA_SAPPHIRE)
    luaunit.assertEquals(#type_list, 18)
    luaunit.assertEquals(type_list[1], pkmn.type.NORMAL)
    luaunit.assertEquals(type_list[18], pkmn.type.FAIRY)

    local type_name_list = pkmn.database.get_type_name_list(pkmn.game.ALPHA_SAPPHIRE)
    luaunit.assertEquals(#type_name_list, 18)
    luaunit.assertEquals(type_name_list[1], "Normal")
    luaunit.assertEquals(type_name_list[18], "Fairy")

    for type_index = 1, #type_list
    do
        luaunit.assertEquals(
            pkmn.string_to_type(type_name_list[type_index]),
            type_list[type_index]
        )
    end
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
