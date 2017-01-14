--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")

local pokemon_pc_tests = {}

pokemon_pc_tests.GAME_TO_GENERATION = {
    ["Red"] = 1,
    ["Blue"] = 1,
    ["Yellow"] = 1,
    ["Gold"] = 2,
    ["Silver"] = 2,
    ["Crystal"] = 2,
    ["Ruby"] = 3,
    ["Sapphire"] = 3,
    ["Emerald"] = 3,
    ["FireRed"] = 3,
    ["LeafGreen"] = 3,
    ["Colosseum"] = 3,
    ["XD"] = 3
}

function pokemon_pc_tests.test_empty_pokemon_box(box, game)
    local generation = pokemon_pc_tests.GAME_TO_GENERATION[game]

    luaunit.assertEquals(box:get_game(), game)
    if generation == 1
    then
        luaunit.assertError(box.get_name, box)
    else
        luaunit.assertEquals(box:get_name(), "")
    end

    luaunit.assertEquals(box:get_capacity(), #box)

    -- Make sure trying to get a Pokémon at an invalid index fails.
    luaunit.assertError(box.get_pokemon, box, -1)
    luaunit.assertError(box.get_pokemon, box, #box)

    for i = 1, #box
    do
        luaunit.assertEquals(box[i]:get_species(), "None")
        luaunit.assertEquals(box[i]:get_game(), game)

        local moves = box[i]:get_moves()
        for i = 1, #moves
        do
            luaunit.assertEquals(moves[i].move:get_name(), "None")
            luaunit.assertEquals(moves[i].pp, 0)
        end
    end
end

function pokemon_pc_tests.test_box_name(box)
    local generation = pokemon_pc_tests.GAME_TO_GENERATION[box:get_game()]

    if generation == 1
    then
        luaunit.assertError(box.set_name, box, "ABCDEFGH")
        luaunit.assertError(box.get_name)
    else
        luaunit.assertError(box.set_name, box, "ABCDEFGHI")
        
        box:set_name("ABCDEFGH")
        luaunit.assertEquals(box:get_name(), "ABCDEFGH")
    end
end

function pokemon_pc_tests.test_setting_pokemon(box)
    local game = box:get_game()

    local original_first = box[1]
    local original_second = box[2]

    -- Make sure we can't set Pokémon at invalid indices.
    luaunit.assertError(box.set_pokemon, 0, original_first)
    luaunit.assertError(box.set_pokemon, (#box+1), original_second)

    -- Make sure we can't move these.
    luaunit.assertError(box.set_pokemon, 3, original_first)
    luaunit.assertError(box.set_pokemon, 4, original_second)

    -- Create Pokémon and place in box. The original variables should
    -- still have the same underlying Pokémon.
    local bulbasaur = pkmn.pokemon("Bulbasaur", game, "", 5)
    local charmander = pkmn.pokemon("Charmander", game, "", 5)
    local squirtle = pkmn.pokemon("Squirtle", game, "", 5)

    box[1] = bulbasaur
    box[2] = charmander

    -- Make sure we can't do that again.
    luaunit.assertError(box.set_pokemon, 3, bulbasaur)
    luaunit.assertError(box.set_pokemon, 4, charmander)

    -- Replace one of the new ones.
    box[1] = squirtle

    -- Now check everything we've created. Each variable should have
    -- the same underlying Pokémon.
    luaunit.assertEquals(box[1]:get_species(), "Squirtle")
    luaunit.assertEquals(box[2]:get_species(), "Charmander")
    luaunit.assertEquals(original_first:get_species(), "None")
    luaunit.assertEquals(original_second:get_species(), "None")
    luaunit.assertEquals(bulbasaur:get_species(), "Bulbasaur")
    luaunit.assertEquals(charmander:get_species(), "Charmander")
    luaunit.assertEquals(squirtle:get_species(), "Squirtle")
end

function pokemon_pc_tests.test_pokemon_box(box, game)
    pokemon_pc_tests.test_empty_pokemon_box(box, game)
    pokemon_pc_tests.test_box_name(box)
    pokemon_pc_tests.test_setting_pokemon(box)
end

function pokemon_pc_tests.test_empty_pokemon_pc(pc, game)
    luaunit.assertEquals(pc:get_game(), game)
    luaunit.assertEquals(pc:get_num_boxes(), #pc)

    for i = 1, #pc
    do
        pokemon_pc_tests.test_empty_pokemon_box(pc[i], game)
    end
end

function pokemon_pc_tests.test_box_names(pc)
    local generation = pokemon_pc_tests.GAME_TO_GENERATION[pc:get_game()]

    if generation == 1
    then
        luaunit.assertError(pc[1].set_name, pc[1], "ABCDEFGH")
    else
        for i = 1, #pc
        do
            local box_name = string.format("BOX%d", i)
            pc[i]:set_name(box_name)
        end

        for i = 1, #pc
        do
            local box_name = string.format("BOX%d", i)
            luaunit.assertEquals(pc[i]:get_name(), box_name)
        end
    end
end

function pokemon_pc_tests.test_setting_pokemon_in_boxes(pc)
    for i = 1, #pc
    do
        pokemon_pc_tests.test_setting_pokemon(pc[i])
        luaunit.assertEquals(pc[i][1]:get_species(), "Squirtle")
        luaunit.assertEquals(pc[i][2]:get_species(), "Charmander")
    end
end

function pokemon_pc_tests.test_pokemon_pc(pc, game)
    pokemon_pc_tests.test_empty_pokemon_pc(pc, game)
    pokemon_pc_tests.test_box_names(pc)
    pokemon_pc_tests.test_setting_pokemon_in_boxes(pc)
end

return pokemon_pc_tests
