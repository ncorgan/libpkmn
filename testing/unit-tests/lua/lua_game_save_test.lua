--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local pkmntest_utils = require("pkmntest_utils")
local luaunit = require("luaunit")

math.randomseed(os.time())

local game_save_test = {}

game_save_test.PKSAV_TEST_SAVES = pkmn.paths.getenv("PKSAV_TEST_SAVES")
game_save_test.LIBPKMN_TEST_FILES = pkmn.paths.getenv("LIBPKMN_TEST_FILES")
game_save_test.PKMN_TMP_DIR = pkmn.paths.get_tmp_dir()

game_save_test.MAX_UINT16 = 0xFFFF
game_save_test.MAX_UINT32 = 0xFFFFFFFF

game_save_test.DEFAULT_TRAINER_PID = 1351
game_save_test.DEFAULT_TRAINER_SID = 32123
game_save_test.MONEY_MAX = 999999

game_save_test.MALE_ONLY_GAMES = {
    "Red", "Blue", "Yellow",
    "Gold", "Silver",
    "Colosseum", "XD"
}

game_save_test.RIVAL_NAME_SET_GAMES = {
    "Ruby", "Sapphire", "Emerald", "Colosseum", "XD",
    "Black", "White",
    "X", "Y"
}

function game_save_test.is_gb_game(game)
    return (pkmntest_utils.GAME_TO_GENERATION[game] <= 2)
end

function game_save_test.is_male_only(game)
    for game_index = 1, #game_save_test.MALE_ONLY_GAMES
    do
        if game_save_test.MALE_ONLY_GAMES[game_index] == game
        then
            return true
        end
    end

    return false
end

function game_save_test.is_rival_name_set(game)
    for game_index = 1, #game_save_test.RIVAL_NAME_SET_GAMES
    do
        if game_save_test.RIVAL_NAME_SET_GAMES[game_index] == game
        then
            return true
        end
    end

    return false
end

function game_save_test.test_trainer_id(save, is_gb_game)
    if is_gb_game
    then
        luaunit.assertEquals(save.trainer_id, game_save_test.DEFAULT_TRAINER_PID)
        luaunit.assertEquals(save.trainer_public_id, game_save_test.DEFAULT_TRAINER_PID)
        luaunit.assertError(save.get_trainer_secret_id, save)
    else
        luaunit.assertEquals(save.trainer_id, pkmn.pokemon.DEFAULT_TRAINER_ID)
        luaunit.assertEquals(save.trainer_public_id, game_save_test.DEFAULT_TRAINER_PID)
        luaunit.assertEquals(save.trainer_secret_id, game_save_test.DEFAULT_TRAINER_SID)
    end
end

function game_save_test.test_time_played(save)
    local generation = pkmntest_utils.GAME_TO_GENERATION[save.game]

    -- Test valid values.

    local hours = math.random(0, 255)
    save.time_played.hours = hours
    luaunit.assertEquals(save.time_played.hours, hours)

    local minutes = math.random(0, 59)
    save.time_played.minutes = minutes
    luaunit.assertEquals(save.time_played.minutes, minutes)

    local seconds = math.random(0, 59)
    save.time_played.seconds = seconds
    luaunit.assertEquals(save.time_played.seconds, seconds)

    -- Generation I doesn't have frames, so it won't get back
    -- the correct value.
    if generation > 1
    then
        local frames = math.random(0, 59)
        save.time_played.frames = frames
        luaunit.assertEquals(save.time_played.frames, frames)
    end

    -- Test invalid values. Use these stupid hacky functions because
    -- LuaUnit can't check setting variables.
    function time_played_set_hours(hours)
        save.time_played.hours = hours
    end
    function time_played_set_minutes(minutes)
        save.time_played.minutes = minutes
    end
    function time_played_set_seconds(seconds)
        save.time_played.seconds = seconds
    end
    function time_played_set_frames(frames)
        save.time_played.frames = frames
    end

    -- Too low/high hours
    luaunit.assertError(time_played_set_hours, -1)
    luaunit.assertError(time_played_set_hours, 999999)

    -- Too low/high minutes
    luaunit.assertError(time_played_set_minutes, -1)
    luaunit.assertError(time_played_set_minutes, 999999)

    -- Too low/high seconds
    luaunit.assertError(time_played_set_seconds, -1)
    luaunit.assertError(time_played_set_seconds, 999999)

    if generation > 1
    then
        -- Too low/high frames
        luaunit.assertError(time_played_set_frames, -1)
        luaunit.assertError(time_played_set_frames, 999999)
    end
end

function game_save_test.test_common_fields(save)
    local game = save.game
    local is_gb_game = game_save_test.is_gb_game(game)
    local is_male_only = game_save_test.is_male_only(game)
    local is_rival_name_set = game_save_test.is_rival_name_set(game)
    local is_gamecube_game = (game == "Colosseum") or (game == "XD")

    -- Hacky setter functions because LuaUnit can't check setting variables...
    function game_save_set_money(save, money)
        save.money = money
    end
    function game_save_set_trainer_name(save, name)
        save.trainer_name = name
    end
    function game_save_set_trainer_gender(save, gender)
        save.trainer_gender = gender
    end
    function game_save_set_trainer_id(save, id)
        save.trainer_id = id
    end
    function game_save_set_trainer_public_id(save, public_id)
        save.trainer_public_id = public_id
    end
    function game_save_set_trainer_secret_id(save, secret_id)
        save.trainer_secret_id = secret_id
    end
    function game_save_set_rival_name(save, name)
        save.rival_name = name
    end

    -- Trainer name
    luaunit.assertError(
        game_save_set_trainer_name,
        save,
        ""
    )
    luaunit.assertError(
        game_save_set_trainer_name,
        save,
        "LibPKMNLibPKMN"
    )

    save.trainer_name = "LibPKMN"
    luaunit.assertEquals(save.trainer_name, "LibPKMN")

    -- Trainer ID
    if is_gb_game
    then
        save.trainer_id = game_save_test.DEFAULT_TRAINER_PID
    else
        save.trainer_id = pkmn.pokemon.DEFAULT_TRAINER_ID
    end
    game_save_test.test_trainer_id(save, is_gb_game)

    save.trainer_public_id = game_save_test.DEFAULT_TRAINER_PID
    game_save_test.test_trainer_id(save, is_gb_game)

    if is_gb_game
    then
        luaunit.assertError(
            game_save_set_trainer_secret_id,
            save,
            game_save_test.DEFAULT_TRAINER_SID
        )
    else
        save.trainer_secret_id = game_save_test.DEFAULT_TRAINER_SID
    end
    game_save_test.test_trainer_id(save, is_gb_game)

    -- Make sure Lua+SWIG properly catches out-of-range values.
    pkmntest_utils.enforce_uint16_param(
        game_save_set_trainer_public_id,
        save
    )
    if is_gb_game
    then
        pkmntest_utils.enforce_uint16_param(
            game_save_set_trainer_id,
            save
        )
    else
        pkmntest_utils.enforce_uint16_param(
            game_save_set_trainer_secret_id,
            save
        )
        pkmntest_utils.enforce_uint32_param(
            game_save_set_trainer_id,
            save
        )
    end

    -- Rival name
    if is_rival_name_set
    then
        luaunit.assertError(
            game_save_set_rival_name,
            save,
            "LibPKMN"
        )
    else
        luaunit.assertError(
            game_save_set_rival_name,
            save,
            ""
        )
        luaunit.assertError(
            game_save_set_rival_name,
            save,
            "LibPKMNLibPKMN"
        )
        save.rival_name = "LibPKMN"
        luaunit.assertEquals(save.rival_name, "LibPKMN")
    end

    -- Trainer gender
    if is_male_only
    then
        luaunit.assertEquals(save.trainer_gender, "Male")
        luaunit.assertError(
            game_save_set_trainer_gender,
            save,
            "Male"
        )
        luaunit.assertError(
            game_save_set_trainer_gender,
            save,
            "Female"
        )

        luaunit.assertError(save.set_trainer_gender, save, "Male")
        luaunit.assertError(save.set_trainer_gender, save, "Female")
    else
        save.trainer_gender = "Male"
        luaunit.assertEquals(save.trainer_gender, "Male")
        save.trainer_gender = "Female"
        luaunit.assertEquals(save.trainer_gender, "Female")

        luaunit.assertError(
            game_save_set_trainer_gender,
            save,
            "Genderless"
        )
    end

    -- Money
    luaunit.assertError(game_save_set_money, save, -1)
    luaunit.assertError(game_save_set_money, save, game_save_test.MONEY_MAX+1)
    save.money = 123456
    luaunit.assertEquals(save.money, 123456)

    -- Not how these are typically used, but this test is slow...
    local pokedex_seen_pokemon_map = nil
    local pokedex_caught_pokemon_map = nil
    if not is_gamecube_game
    then
        pokedex_seen_pokemon_map = save.pokedex.seen_pokemon_map
        pokedex_caught_pokemon_map = save.pokedex.caught_pokemon_map
    end

    -- Pokémon Party
    luaunit.assertEquals(#save.pokemon_party, 6)
    luaunit.assertTrue(save.pokemon_party.num_pokemon <= 6)

    local party = save.pokemon_party
    local num_pokemon = party.num_pokemon

    for party_index = 1, 6
    do
        local pokemon = party[party_index]

        if party_index <= num_pokemon
        then
            luaunit.assertNotEquals(pokemon.species, "None")

            if not is_gamecube_game and not pokemon.is_egg
            then
                luaunit.assertTrue(pokedex_seen_pokemon_map[pokemon.species])
                luaunit.assertTrue(pokedex_caught_pokemon_map[pokemon.species])
            end
        else
            luaunit.assertEquals(pokemon.species, "None")
        end
    end

    -- Pokémon PC
    local pokemon_pc = save.pokemon_pc

    for pc_index = 1, #pokemon_pc
    do
        local box = pokemon_pc[pc_index]
        luaunit.assertTrue(box.num_pokemon <= #box)

        -- Boxes are only contiguous in Game Boy games.
        if is_gb_game
        then
            for box_index = 1, #box
            do
                local pokemon = box[box_index]

                if box_index <= box.num_pokemon
                then
                    luaunit.assertNotEquals(pokemon.species, "None")

                    if not is_gamecube_game and not pokemon.is_egg
                    then
                        luaunit.assertTrue(pokedex_seen_pokemon_map[pokemon.species])
                        luaunit.assertTrue(pokedex_caught_pokemon_map[pokemon.species])
                    end
                else
                    luaunit.assertEquals(pokemon.species, "None")
                end
            end
        end
    end

    -- Time played
    if not is_gamecube_game
    then
        game_save_test.test_time_played(save)
    end
end

function game_save_test.test_attributes(save)
    local game = save.game
    local generation = pkmntest_utils.GAME_TO_GENERATION[game]

    if generation == 1
    then
        luaunit.assertTrue(save.numeric_attributes["Casino coins"] >= 0)
        luaunit.assertTrue(save.numeric_attributes["Casino coins"] <= 9999)

        local casino_coins = math.random(0, 9999)
        save.numeric_attributes["Casino coins"] = casino_coins
        luaunit.assertEquals(save.numeric_attributes["Casino coins"], casino_coins)

        if game == "Yellow"
        then
            luaunit.assertTrue(save.numeric_attributes["Pikachu friendship"] >= 0)
            luaunit.assertTrue(save.numeric_attributes["Pikachu friendship"] <= 255)

            local pikachu_friendship = math.random(0, 255)
            save.numeric_attributes["Pikachu friendship"] = pikachu_friendship
            luaunit.assertEquals(save.numeric_attributes["Pikachu friendship"], pikachu_friendship)
        end
    elseif generation == 3
    then
        if game ~= "Colosseum" and game ~= "XD"
        then
            luaunit.assertTrue(save.numeric_attributes["Casino coins"] >= 0)
            luaunit.assertTrue(save.numeric_attributes["Casino coins"] <= 9999)

            local casino_coins = math.random(0, 9999)
            save.numeric_attributes["Casino coins"] = casino_coins
            luaunit.assertEquals(save.numeric_attributes["Casino coins"], casino_coins)
        end
    end
end

function game_save_test.randomize_pokemon(save, item_list)
    local game = save.game
    local generation = pkmntest_utils.GAME_TO_GENERATION[game]
    local pokemon_list = pkmn.database.get_pokemon_list(generation, true)
    local move_list = pkmn.database.get_move_list(game)

    local party = save.pokemon_party
    for party_index = 1, #party
    do
        party[party_index] = pkmntest_utils.get_random_pokemon(game, pokemon_list, move_list, item_list)
    end

    pokemon_pc = save.pokemon_pc
    for pc_index = 1, #pokemon_pc
    do
        local box = pokemon_pc[pc_index]
        for box_index = 1, #box
        do
            box[box_index] = pkmntest_utils.get_random_pokemon(game, pokemon_list, move_list, item_list)
        end
    end
end

function game_save_test.compare_item_lists(item_list1, item_list2)
    luaunit.assertEquals(item_list1.game, item_list2.game)
    luaunit.assertEquals(item_list1.name, item_list2.name)
    luaunit.assertEquals(#item_list1, #item_list2)
    luaunit.assertEquals(item_list1.num_items, item_list2.num_items)

    for item_index = 1, #item_list1
    do
        luaunit.assertEquals(item_list1[item_index].item, item_list2[item_index].item)
        luaunit.assertEquals(item_list1[item_index].amount, item_list2[item_index].amount)
    end
end

function game_save_test.compare_game_saves(save1, save2)
    local game = save1.game
    local generation = pkmntest_utils.GAME_TO_GENERATION[game]
    local is_gb_game = game_save_test.is_gb_game(game)
    local is_male_only = game_save_test.is_male_only(game)
    local is_rival_name_set = game_save_test.is_rival_name_set(game)

    luaunit.assertEquals(save1.game, save2.game)
    luaunit.assertEquals(save1.trainer_id, save2.trainer_id)
    luaunit.assertEquals(save1.trainer_public_id, save2.trainer_public_id)

    if not is_gb_game
    then
        luaunit.assertEquals(save1.trainer_secret_id, save2.trainer_secret_id)
    end
    if not is_male_only
    then
        luaunit.assertEquals(save1.trainer_gender, save2.trainer_gender)
    end
    if not is_rival_name_set
    then
        luaunit.assertEquals(save1.rival_name, save2.rival_name)
    end

    luaunit.assertEquals(save1.money, save2.money)

    local bag1 = save1.item_bag
    local bag2 = save2.item_bag

    luaunit.assertEquals(#bag1, #bag2)
    luaunit.assertEquals(bag1.pocket_names, bag2.pocket_names)

    local pocket_names = bag1.pocket_names
    for pocket_index = 1, #pocket_names
    do
        game_save_test.compare_item_lists(
            bag1[pocket_names[pocket_index]],
            bag2[pocket_names[pocket_index]]
        )
    end

    if generation <= 3
    then
        game_save_test.compare_item_lists(save1.item_pc, save2.item_pc)
    end

    local party1 = save1.pokemon_party
    local party2 = save2.pokemon_party

    luaunit.assertEquals(party1.num_pokemon, party2.num_pokemon)
    for party_index = 1, 6
    do
        pkmntest_utils.compare_pokemon(
            party1[party_index],
            party2[party_index]
        )
    end

    local pc1 = save1.pokemon_pc
    local pc2 = save2.pokemon_pc

    luaunit.assertEquals(#pc1, #pc2)
    if generation >= 2
    then
        luaunit.assertEquals(pc1.box_names, pc2.box_names)

        for pc_index = 1, #pc1
        do
            luaunit.assertEquals(pc1[pc_index].name, pc2[pc_index].name)
        end
    end
    for pc_index = 1, #pc1
    do
        local box1 = pc1[pc_index]
        local box2 = pc2[pc_index]

        luaunit.assertEquals(#box1, #box2)
        for box_index = 1, #pc1[1]
        do
            pkmntest_utils.compare_pokemon(
                box1[box_index],
                box2[box_index]
            )
        end
    end

    if game ~= "Colosseum" and game ~= "XD"
    then
        luaunit.assertEquals(save1.pokedex.all_seen, save2.pokedex.all_seen)
        luaunit.assertEquals(save1.pokedex.all_caught, save2.pokedex.all_caught)
    end

    pkmntest_utils.compare_attributes(save1, save2)
end

function game_save_test.test_game_save(expected_type, expected_game, subdir, filename)
    local save_filepath = ""
    if expected_game == "Colosseum" or expected_game == "XD"
    then
        save_filepath = pkmntest_utils.concat_paths(game_save_test.LIBPKMN_TEST_FILES, subdir, filename)
    else
        save_filepath = pkmntest_utils.concat_paths(game_save_test.PKSAV_TEST_SAVES, subdir, filename)
    end
    luaunit.assertEquals(pkmn.detect_game_save_type(save_filepath), expected_type)

    local save = pkmn.game_save(save_filepath)
    luaunit.assertEquals(save.filepath, save_filepath)
    luaunit.assertEquals(save.game, expected_game)

    local item_list = pkmn.database.get_item_list(expected_game)

    game_save_test.test_common_fields(save)
    game_save_test.test_attributes(save)
    -- TODO: randomize_items
    game_save_test.randomize_pokemon(save, item_list)

    temp_save_filepath = pkmntest_utils.concat_path(
                             game_save_test.PKMN_TMP_DIR,
                             string.format("%s_%d.sav", expected_game, math.random(0, 0xFFFF))
                         )
    save:save_as(temp_save_filepath)

    local save2 = pkmn.game_save(temp_save_filepath)
    luaunit.assertEquals(save2.filepath, temp_save_filepath)
    luaunit.assertEquals(save2.game, expected_game)
    game_save_test.compare_game_saves(save, save2)

    os.remove(temp_save_filepath)
end

function test_red_game_save()
    game_save_test.test_game_save(
        "Red/Blue",
        "Red",
        "red_blue",
        "pokemon_red.sav"
    )
end

function test_yellow_game_save()
    game_save_test.test_game_save(
        "Yellow",
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

function test_colosseum_game_save()
    game_save_test.test_game_save(
        "Colosseum/XD",
        "Colosseum",
        "gamecube_saves",
        "pokemon_colosseum.gci"
    )
end

function test_xd_game_save()
    game_save_test.test_game_save(
        "Colosseum/XD",
        "XD",
        "gamecube_saves",
        "pokemon_xd.gci"
    )
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
