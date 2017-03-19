--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")

local utils = {}

-- http://stackoverflow.com/a/30960054
function utils.is_windows()
    local shared_lib_ext = package.cpath:match("%p[\\|/]?%p(%a+)")
    return (shared_lib_ext == "dll")
end

function utils.concat_paths(path1, path2, path3)
    if utils.is_windows()
    then
        if path3 == nil
        then
            return string.format("%s\\%s", path1, path2)
        else
            return string.format("%s\\%s\\%s", path1, path2, path3)
        end
    else
        if path3 == nil
        then
            return string.format("%s/%s", path1, path2)
        else
            return string.format("%s/%s/%s", path1, path2, path3)
        end
    end
end

local game_save_test = {}

game_save_test.GAME_TO_GENERATION = {
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

game_save_test.PKSAV_TEST_SAVES = pkmn.paths.getenv("PKSAV_TEST_SAVES")
game_save_test.PKMN_TMP_DIR = pkmn.paths.get_tmp_dir()

game_save_test.MAX_UINT16 = 0xFFFF
game_save_test.MAX_UINT32 = 0xFFFFFFFF

game_save_test.LIBPKMN_OT_PID = 1351
game_save_test.LIBPKMN_OT_SID = 32123
game_save_test.MONEY_MAX = 999999

game_save_test.MALE_ONLY_GAMES = {
    "Red", "Blue", "Yellow",
    "Gold", "Silver"
}

game_save_test.RIVAL_NAME_SET_GAMES = {
    "Ruby", "Sapphire", "Emerald",
    "Black", "White",
    "X", "Y"
}

function game_save_test.is_gb_game(game)
    return (game_save_test.GAME_TO_GENERATION[game] <= 2)
end

function game_save_test.is_male_only(game)
    for i = 1, #game_save_test.MALE_ONLY_GAMES
    do
        if game_save_test.MALE_ONLY_GAMES[i] == game
        then
            return true
        end
    end

    return false
end

function game_save_test.is_rival_name_set(game)
    for i = 1, #game_save_test.RIVAL_NAME_SET_GAMES
    do
        if game_save_test.RIVAL_NAME_SET_GAMES[i] == game
        then
            return true
        end
    end

    return false
end

function game_save_test.test_trainer_id(save, is_gb_game)
    if is_gb_game
    then
        luaunit.assertEquals(save:get_trainer_id(), game_save_test.LIBPKMN_OT_PID)
        luaunit.assertEquals(save:get_trainer_public_id(), game_save_test.LIBPKMN_OT_PID)
        luaunit.assertError(save.get_trainer_secret_id, save)
    else
        luaunit.assertEquals(save:get_trainer_id(), pkmn.LIBPKMN_OT_ID)
        luaunit.assertEquals(save:get_trainer_public_id(), game_save_test.LIBPKMN_OT_PID)
        luaunit.assertEquals(save:get_trainer_secret_id(), game_save_test.LIBPKMN_OT_SID)
    end
end

function game_save_test.test_common_fields(save)
    local game = save:get_game()
    local is_gb_game = game_save_test.is_gb_game(game)
    local is_male_only = game_save_test.is_male_only(game)
    local is_rival_name_set = game_save_test.is_rival_name_set(game)

    -- Trainer name
    luaunit.assertError(save.set_trainer_name, save, "")
    luaunit.assertError(save.set_trainer_name, save, "LibPKMNLibPKMN")
    save:set_trainer_name("LibPKMN")
    luaunit.assertEquals(save:get_trainer_name(), "LibPKMN")

    -- Trainer ID
    if is_gb_game
    then
        save:set_trainer_id(game_save_test.LIBPKMN_OT_PID)
    else
        save:set_trainer_id(pkmn.LIBPKMN_OT_ID)
    end
    game_save_test.test_trainer_id(save, is_gb_game)

    save:set_trainer_public_id(game_save_test.LIBPKMN_OT_PID)
    game_save_test.test_trainer_id(save, is_gb_game)

    if is_gb_game
    then
        luaunit.assertError(save.set_trainer_secret_id, save, game_save_test.LIBPKMN_OT_SID)
    else
        save:set_trainer_secret_id(game_save_test.LIBPKMN_OT_SID)
    end
    game_save_test.test_trainer_id(save, is_gb_game)

    -- Make sure Lua+SWIG properly catches out-of-range values.
    luaunit.assertError(save.set_trainer_id, save, -1)
    luaunit.assertError(save.set_trainer_public_id, save, -1)
    luaunit.assertError(save.set_trainer_public_id, save, game_save_test.MAX_UINT16+1)
    if is_gb_game
    then
        luaunit.assertError(save.set_trainer_id, save, game_save_test.MAX_UINT16+1)
    else
        luaunit.assertError(save.set_trainer_id, save, game_save_test.MAX_UINT32+1)
        luaunit.assertError(save.set_trainer_secret_id, save, -1)
        luaunit.assertError(save.set_trainer_secret_id, save, game_save_test.MAX_UINT16+1)
    end

    -- Rival name
    if is_rival_name_set
    then
        luaunit.assertError(save.set_rival_name, save, "LibPKMN")
    else
        luaunit.assertError(save.set_rival_name, save, "")
        luaunit.assertError(save.set_rival_name, save, "LibPKMNLibPKMN")
        save:set_rival_name("LibPKMN")
        luaunit.assertEquals(save:get_rival_name(), "LibPKMN")
    end

    -- Trainer gender
    if is_male_only
    then
        luaunit.assertEquals(save:get_trainer_gender(), "Male")
        luaunit.assertError(save.set_trainer_gender, save, "Male")
        luaunit.assertError(save.set_trainer_gender, save, "Female")
    else
        save:set_trainer_gender("Male")
        luaunit.assertEquals(save:get_trainer_gender(), "Male")
        save:set_trainer_gender("Female")
        luaunit.assertEquals(save:get_trainer_gender(), "Female")
        luaunit.assertError(save.set_trainer_gender, save, "Genderless")
    end

    -- Money
    luaunit.assertError(save.set_money, save, -1)
    luaunit.assertError(save.set_money, save, game_save_test.MONEY_MAX+1)
    save:set_money(123456)
    luaunit.assertEquals(save:get_money(), 123456)

    -- Pokémon Party
    local party = save:get_pokemon_party()
    local num_pokemon = party:get_num_pokemon()
    luaunit.assertEquals(#party, 6)
    luaunit.assertTrue(num_pokemon <= 6)

    for i = 1, 6
    do
        if i <= num_pokemon
        then
            luaunit.assertNotEquals(party[i]:get_species(), "None")
        else
            luaunit.assertEquals(party[i]:get_species(), "None")
        end
    end

    -- Pokémon PC
    local pc = save:get_pokemon_pc()
    luaunit.assertEquals(#pc, #pc:as_list())

    for i = 1, #pc
    do
        local box = pc[i]
        luaunit.assertEquals(#box, box:get_capacity())
        luaunit.assertTrue(box:get_num_pokemon() <= #box)

        -- Boxes are only contiguous in Game Boy games.
        if is_gb_game
        then
            for i = 1, #box
            do
                local num_pokemon = box:get_num_pokemon()
                if i <= num_pokemon
                then
                    luaunit.assertNotEquals(box[i]:get_species(), "None")
                else
                    luaunit.assertEquals(box[i]:get_species(), "None")
                end
            end
        end
    end
end

function game_save_test.get_random_pokemon(game, pokemon_list, move_list, item_list)
    local generation = game_save_test.GAME_TO_GENERATION[game]

    local species = ""
    repeat
        species = pokemon_list[math.random(1, #pokemon_list)]
    until (generation ~= 3 or species ~= "Deoxys")

    ret = pkmn.pokemon(species, game, "", math.random(2, 100))

    for i = 1, 4
    do
        ret:set_move(move_list[math.random(1, #move_list)], i)
    end

    if generation >= 2
    then
        -- Keep going until one is holdable
        repeat
            pcall(ret.set_held_item, ret, item_list[math.random(1, #item_list)])
        until ret:get_held_item():get_name() ~= "None"
    end

    return ret
end

function game_save_test.randomize_pokemon(save, item_list)
    local game = save:get_game()
    local generation = game_save_test.GAME_TO_GENERATION[game]
    local pokemon_list = pkmn.database.get_pokemon_list(generation, true)
    local move_list = pkmn.database.get_move_list(game)

    local party = save:get_pokemon_party()
    for i = 1, #party
    do
        party[i] = game_save_test.get_random_pokemon(game, pokemon_list, move_list, item_list)
    end

    local pc = save:get_pokemon_pc()
    for i = 1, #pc
    do
        local box = pc[i]
        for j = 1, #box
        do
            box[j] = game_save_test.get_random_pokemon(game, pokemon_list, move_list, item_list)
        end
    end
end

function game_save_test.compare_item_lists(item_list1, item_list2)
    luaunit.assertEquals(item_list1:get_game(), item_list2:get_game())
    luaunit.assertEquals(item_list1:get_name(), item_list2:get_name())
    luaunit.assertEquals(#item_list1, #item_list2)
    luaunit.assertEquals(item_list1:get_num_items(), item_list2:get_num_items())

    for i = 1, #item_list1
    do
        luaunit.assertEquals(item_list1[i].item:get_name(), item_list2[i].item:get_name())
        luaunit.assertEquals(item_list1[i].amount, item_list2[i].amount)
    end
end

function game_save_test.compare_pokemon(pokemon1, pokemon2)
    luaunit.assertEquals(pokemon1:get_game(), pokemon2:get_game())
    luaunit.assertEquals(pokemon1:get_species(), pokemon2:get_species())
    luaunit.assertEquals(pokemon1:get_form(), pokemon2:get_form())
    luaunit.assertEquals(pokemon1:get_nickname(), pokemon2:get_nickname())
    luaunit.assertEquals(pokemon1:get_trainer_name(), pokemon2:get_trainer_name())

    local moves1 = pokemon1:get_moves()
    local moves2 = pokemon2:get_moves()
    for i = 1, 4
    do
        luaunit.assertEquals(moves1[i].move:get_name(), moves2[i].move:get_name())
        luaunit.assertEquals(moves1[i].pp, moves2[i].pp)
    end

    if game_save_test.GAME_TO_GENERATION[pokemon1:get_game()] >= 2
    then
        luaunit.assertEquals(pokemon1:get_held_item():get_name(), pokemon2:get_held_item():get_name())
    end
end

function game_save_test.compare_game_saves(save1, save2)
    local game = save1:get_game()
    local generation = game_save_test.GAME_TO_GENERATION[game]
    local is_gb_game = game_save_test.is_gb_game(game)
    local is_male_only = game_save_test.is_male_only(game)
    local is_rival_name_set = game_save_test.is_rival_name_set(game)

    luaunit.assertEquals(save1:get_game(), save2:get_game())
    luaunit.assertEquals(save1:get_trainer_id(), save2:get_trainer_id())
    luaunit.assertEquals(save1:get_trainer_public_id(), save2:get_trainer_public_id())

    if not is_gb_game
    then
        luaunit.assertEquals(save1:get_trainer_secret_id(), save2:get_trainer_secret_id())
    end
    if not is_male_only
    then
        luaunit.assertEquals(save1:get_trainer_gender(), save2:get_trainer_gender())
    end
    if not is_rival_name_set
    then
        luaunit.assertEquals(save1:get_rival_name(), save2:get_rival_name())
    end

    luaunit.assertEquals(save1:get_money(), save2:get_money())

    local bag1 = save1:get_item_bag()
    local bag2 = save2:get_item_bag()
    luaunit.assertEquals(#bag1, #bag2)
    luaunit.assertEquals(#bag1:get_pocket_names(), #bag2:get_pocket_names())

    local pocket_names = bag1:get_pocket_names()
    for i = 1, #pocket_names
    do
        game_save_test.compare_item_lists(bag1[pocket_names[i]], bag2[pocket_names[i]])
    end

    if generation <= 3
    then
        game_save_test.compare_item_lists(save1:get_item_pc(), save2:get_item_pc())
    end

    local party1 = save1:get_pokemon_party()
    local party2 = save2:get_pokemon_party()
    luaunit.assertEquals(party1:get_num_pokemon(), party2:get_num_pokemon())
    for i = 1, 6
    do
        game_save_test.compare_pokemon(party1[i], party2[i])
    end

    local pc1 = save1:get_pokemon_pc()
    local pc2 = save2:get_pokemon_pc()
    if generation >= 2
    then
        local box_names1 = pc1:get_box_names()
        local box_names2 = pc2:get_box_names()
        luaunit.assertEquals(#box_names1, #box_names2)
        for i = 1, #box_names1
        do
            luaunit.assertEquals(box_names1[i], box_names2[i])
        end
    end
    for i = 1, #pc1
    do
        if generation >= 2
        then
            luaunit.assertEquals(pc1[i]:get_name(), pc2[i]:get_name())
            luaunit.assertEquals(#pc1[i], #pc2[i])
            for j = 1, #pc1[i]
            do
                game_save_test.compare_pokemon(pc1[i][j], pc2[i][j])
            end
        end
    end
end

function game_save_test.test_game_save(expected_type, expected_game, subdir, filename)
    local save_filepath = utils.concat_paths(game_save_test.PKSAV_TEST_SAVES, subdir, filename)
    luaunit.assertEquals(pkmn.detect_game_save_type(save_filepath), expected_type)

    local save = pkmn.game_save(save_filepath)
    luaunit.assertEquals(save:get_filepath(), save_filepath)
    luaunit.assertEquals(save:get_game(), expected_game)

    local item_list = pkmn.database.get_item_list(expected_game)

    game_save_test.test_common_fields(save)
    -- TODO: randomize_items
    game_save_test.randomize_pokemon(save, item_list)

    temp_save_filepath = utils.concat_paths(
                             game_save_test.PKMN_TMP_DIR,
                             string.format("%s_%d.sav", expected_game, math.random(0, 0xFFFF))
                         )
    save:save_as(temp_save_filepath)

    local save2 = pkmn.game_save(temp_save_filepath)
    luaunit.assertEquals(save2:get_filepath(), temp_save_filepath)
    luaunit.assertEquals(save2:get_game(), expected_game)
    game_save_test.compare_game_saves(save, save2)

    os.remove(temp_save_filepath)
end

function test_red_game_save()
    game_save_test.test_game_save(
        "Red/Blue/Yellow",
        "Red",
        "red_blue",
        "pokemon_red.sav"
    )
end

function test_yellow_game_save()
    game_save_test.test_game_save(
        "Red/Blue/Yellow",
        "Yellow",
        "yellow",
        "pokemon_yellow.sav"
    )
end

function test_gold_game_save()
    game_save_test.test_game_save(
        "Gold/Silver",
        "Gold",
        "gold_silver",
        "pokemon_gold.sav"
    )
end

function test_crystal_game_save()
    game_save_test.test_game_save(
        "Crystal",
        "Crystal",
        "crystal",
        "pokemon_crystal.sav"
    )
end

function test_ruby_game_save()
    game_save_test.test_game_save(
        "Ruby/Sapphire",
        "Ruby",
        "ruby_sapphire",
        "pokemon_ruby.sav"
    )
end

function test_emerald_game_save()
    game_save_test.test_game_save(
        "Emerald",
        "Emerald",
        "emerald",
        "pokemon_emerald.sav"
    )
end

function test_firered_game_save()
    game_save_test.test_game_save(
        "FireRed/LeafGreen",
        "FireRed",
        "firered_leafgreen",
        "pokemon_firered.sav"
    )
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
