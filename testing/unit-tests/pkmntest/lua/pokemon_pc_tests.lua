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
    print(" * test_empty_pokemon_box")
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
    print(" * test_box_name")
    local generation = pokemon_pc_tests.GAME_TO_GENERATION[box:get_game()]

    if generation == 1
    then
        luaunit.assertError(box.set_name, box, "ABCDEFGH")
        luaunit.assertError(box.get_name, box)
    else
        luaunit.assertError(box.set_name, box, "ABCDEFGHI")
        
        box:set_name("ABCDEFGH")
        luaunit.assertEquals(box:get_name(), "ABCDEFGH")
    end
end

function pokemon_pc_tests.test_setting_pokemon(box)
    print(" * test_setting_pokemon")
    local game = box:get_game()
    local generation = pokemon_pc_tests.GAME_TO_GENERATION[game]

    local original_first = box[1]
    local original_second = box[2]

    -- Make sure we can't set Pokémon at invalid indices.
    luaunit.assertError(box.set_pokemon, box, 0, original_first)
    luaunit.assertError(box.set_pokemon, box, (#box+1), original_second)

    -- Create Pokémon and place in box. The original variables should
    -- still have the same underlying Pokémon.
    local bulbasaur = pkmn.pokemon("Bulbasaur", game, "", 5)
    local charmander = pkmn.pokemon("Charmander", game, "", 5)
    local squirtle = pkmn.pokemon("Squirtle", game, "", 5)

    box[1] = bulbasaur
    luaunit.assertEquals(box:get_num_pokemon(), 1)
    box[2] = charmander
    luaunit.assertEquals(box:get_num_pokemon(), 2)

    -- Replace one of the new ones.
    box[1] = squirtle
    luaunit.assertEquals(box:get_num_pokemon(), 2)

    -- Make sure we can't copy a Pokémon to itself.
    luaunit.assertError(box.set_pokemon, box, 2, box[2])
    luaunit.assertEquals(box:get_num_pokemon(), 2)

    -- Copy a Pokémon whose memory is already part of the box.
    box[3] = box[2]
    luaunit.assertEquals(box:get_num_pokemon(), 3)

    -- We should always be able to clear the last contiguous Pokémon.
    box[3] = original_first
    luaunit.assertEquals(box:get_num_pokemon(), 2)
    luaunit.assertEquals(box[3]:get_species(), "None")

    -- Put it back.
    box[3] = box[2]
    luaunit.assertEquals(box:get_num_pokemon(), 3)

    -- Check that Pokémon can be placed non-contiguously in the correct games.
    if generation <= 2
    then
        luaunit.assertError(box.set_pokemon, box, 2, original_first)
        luaunit.assertEquals(box:get_num_pokemon(), 3)
        luaunit.assertEquals(box[2]:get_species(), "Charmander")

        luaunit.assertError(box.set_pokemon, box, 5, bulbasaur)
        luaunit.assertEquals(box:get_num_pokemon(), 3)
        luaunit.assertEquals(box[5]:get_species(), "None")
    else
        box[2] = original_first
        luaunit.assertEquals(box:get_num_pokemon(), 2)
        luaunit.assertEquals(box[2]:get_species(), "None")

        box[5] = bulbasaur
        luaunit.assertEquals(box:get_num_pokemon(), 3)
        luaunit.assertEquals(box[5]:get_species(), "Bulbasaur")

        -- Restore it to how it was.
        box[2] = charmander
        box[5] = original_first
        luaunit.assertEquals(box[2]:get_species(), "Charmander")
        luaunit.assertEquals(box[5]:get_species(), "None")
    end

    -- Now check everything we've created. Each variable should have
    -- the same underlying Pokémon.
    luaunit.assertEquals(box[1]:get_species(), "Squirtle")
    luaunit.assertEquals(box[2]:get_species(), "Charmander")
    luaunit.assertEquals(box[3]:get_species(), "Charmander")
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
    print(" * test_empty_pokemon_pc")
    luaunit.assertEquals(pc:get_game(), game)
    luaunit.assertEquals(pc:get_num_boxes(), #pc)

    for i = 1, #pc
    do
        pokemon_pc_tests.test_empty_pokemon_box(pc[i], game)
    end
end

function pokemon_pc_tests.test_box_names(pc)
    print(" * test_box_names")
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

        local box_names = pc:get_box_names()
        for i = 1, #box_names
        do
            local expected_box_name = string.format("BOX%d", i)
            luaunit.assertEquals(box_names[i], expected_box_name)
        end
    end
end

function pokemon_pc_tests.test_setting_pokemon_in_boxes(pc)
    print(" * test_setting_pokemon_in_boxes")
    for i = 1, #pc
    do
        pokemon_pc_tests.test_setting_pokemon(pc[i])
        luaunit.assertEquals(pc[i][1]:get_species(), "Squirtle")
        luaunit.assertEquals(pc[i][2]:get_species(), "Charmander")
    end
end

function pokemon_pc_tests.test_pokemon_pc(pc, game)
    print(" * test_pokemon_pc")
    pokemon_pc_tests.test_empty_pokemon_pc(pc, game)
    pokemon_pc_tests.test_box_names(pc)
    pokemon_pc_tests.test_setting_pokemon_in_boxes(pc)
end

return pokemon_pc_tests
