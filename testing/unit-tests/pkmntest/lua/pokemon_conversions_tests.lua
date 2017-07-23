--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")
local pokemon_tests = require("pokemon_tests")

local utils = {}
utils.string = {}

-- http://lua-users.org/wiki/StringRecipes
function utils.string.starts(String,Start)
    return string.sub(String,1,string.len(Start))==Start
end

-- http://lua-users.org/wiki/StringRecipes
function utils.string.ends(String,End)
    return End=='' or string.sub(String,-string.len(End))==End
end

function utils.random_bool()
    return (math.random(0, 255) > 127)
end

function utils.random_string(len)
    ret = ""
    for i = 1, len
    do
        n = math.random(65, 90)
        if utils.random_bool()
        then
            n = math.random(97, 122)
        end

        ret = ret .. string.char(n)
    end

    return ret
end

local pokemon_conversions_tests = {}

function pokemon_conversions_tests.conversions_test(species, form, origin_game, dest_game)
    local first_pokemon = pkmn.pokemon(species, origin_game, form, 50)

    local origin_generation = pokemon_tests.GAME_TO_GENERATION[origin_game]
    local dest_generation = pokemon_tests.GAME_TO_GENERATION[dest_game]
    local min_generation = math.min(origin_generation, dest_generation)
    local game_for_lists = ""
    if min_generation == origin_generation
    then
        game_for_lists = origin_game
    else
        game_for_lists = dest_game
    end

    local items = pkmn.database.get_item_list(game_for_lists)
    local moves = pkmn.database.get_move_list(game_for_lists)

    for i = 1, 4
    do
        -- This will get rid of legitimate moves like Shadow Ball,
        -- but not enough to cause an issue.
        local move_name = ""
        repeat
            move_name = moves[math.random(1, #moves)]
        until not utils.string.starts(move_name, "Shadow")

        first_pokemon:set_move(move_name, i)
    end

    if origin_generation >= 3
    then
        first_pokemon:set_trainer_secret_id(math.random(0, 0xFFFF))

        abilities = first_pokemon:get_database_entry():get_abilities()
        if abilities.second ~= "None"
        then
            if utils.random_bool()
            then
                first_pokemon:set_ability(abilities.first)
            else
                first_pokemon:set_ability(abilities.second)
            end
        end
    end
    first_pokemon:set_trainer_public_id(math.random(0, 0xFFFF))

    if min_generation >= 2
    then
        local item_name = ""
        repeat
            item_name = items[math.random(1, #items)]
        until not utils.string.ends(item_name, "Scent") and pkmn.database.item_entry(item_name, game_for_lists):holdable()

        first_pokemon:set_held_item(item_name)
    end
    if origin_generation >= 2
    then
        if utils.random_bool()
        then
            first_pokemon:set_gender("Male")
        else
            first_pokemon:set_gender("Female")
        end

        first_pokemon:set_shininess(utils.random_bool())
        first_pokemon:set_friendship(math.random(0, 255))

        if origin_game ~= "Gold" and origin_game ~= "Silver"
        then
            if utils.random_bool()
            then
                first_pokemon:set_trainer_gender("Male")
            else
                first_pokemon:set_trainer_gender("Female")
            end
        end

        -- The max level met in Generation II is 63.
        if origin_generation >= 3
        then
            first_pokemon:set_level_met(math.random(0, 100))
        else
            first_pokemon:set_level_met(math.random(2, 63))
        end
    end

    if origin_generation >= 3
    then
        markings = first_pokemon:get_markings():keys()
        for i = 1, #markings
        do
            first_pokemon:set_marking(markings[i], utils.random_bool())
        end

        ribbons = first_pokemon:get_ribbons():keys()
        for i = 1, #ribbons
        do
            first_pokemon:set_ribbon(ribbons[i], utils.random_bool())
        end

        contest_stats = first_pokemon:get_contest_stats():keys()
        for i = 1, #contest_stats
        do
            first_pokemon:set_contest_stat(contest_stats[i], math.random(0, 255))
        end
    end

    first_pokemon:set_nickname(utils.random_string(10))
    first_pokemon:set_trainer_name(utils.random_string(7))

    -- The max level met in Generation II is 63, which restricts this as well.
    if origin_generation >= 3
    then
        first_pokemon:set_level(math.random(1, 100))
    else
        first_pokemon:set_level(math.random(2, 63))
    end

    -- Convert to the second game and compare.
    local second_pokemon = first_pokemon:to_game(dest_game)

    luaunit.assertEquals(first_pokemon:get_species(), second_pokemon:get_species())
    luaunit.assertEquals(dest_game, second_pokemon:get_game())
    luaunit.assertEquals(first_pokemon:get_form(), second_pokemon:get_form())
    luaunit.assertEquals(first_pokemon:get_nickname(), second_pokemon:get_nickname())
    luaunit.assertEquals(first_pokemon:get_trainer_name(), second_pokemon:get_trainer_name())
    luaunit.assertEquals(first_pokemon:get_trainer_id(), second_pokemon:get_trainer_id())
    luaunit.assertEquals(first_pokemon:get_trainer_public_id(), second_pokemon:get_trainer_public_id())
    luaunit.assertEquals(first_pokemon:get_experience(), second_pokemon:get_experience())
    luaunit.assertEquals(first_pokemon:get_level(), second_pokemon:get_level())

    for i = 1, 4
    do
        luaunit.assertEquals(first_pokemon:get_moves()[i].move, second_pokemon:get_moves()[i].move)
        luaunit.assertEquals(first_pokemon:get_moves()[i].pp, second_pokemon:get_moves()[i].pp)
    end

    if min_generation >= 3
    then
        luaunit.assertEquals(first_pokemon:get_trainer_secret_id(), second_pokemon:get_trainer_secret_id())
        luaunit.assertEquals(first_pokemon:get_ability(), second_pokemon:get_ability())
        luaunit.assertEquals(first_pokemon:get_ball(), second_pokemon:get_ball())
        luaunit.assertEquals(first_pokemon:get_original_game(), second_pokemon:get_original_game())
        luaunit.assertEquals(first_pokemon:get_personality(), second_pokemon:get_personality())

        if origin_generation == dest_generation
        then
            luaunit.assertEquals(first_pokemon:get_markings(), second_pokemon:get_markings())
            luaunit.assertEquals(first_pokemon:get_contest_stats(), second_pokemon:get_contest_stats())
            luaunit.assertEquals(first_pokemon:get_ribbons(), second_pokemon:get_ribbons())
            luaunit.assertEquals(first_pokemon:get_EVs(), second_pokemon:get_EVs())
            luaunit.assertEquals(first_pokemon:get_IVs(), second_pokemon:get_IVs())
        end
    end

    if min_generation >= 2
    then
        luaunit.assertEquals(first_pokemon:get_trainer_gender(), second_pokemon:get_trainer_gender())
        luaunit.assertEquals(first_pokemon:get_gender(), second_pokemon:get_gender())
        luaunit.assertEquals(first_pokemon:is_shiny(), second_pokemon:is_shiny())
        luaunit.assertEquals(first_pokemon:get_held_item(), second_pokemon:get_held_item())
        luaunit.assertEquals(first_pokemon:get_friendship(), second_pokemon:get_friendship())
        luaunit.assertEquals(first_pokemon:get_level(), second_pokemon:get_level_met())
    end
end

-- Luaunit doesn't support parameterized tests, so this will have to do.

function test_pokemon_conversion_from_gen1()
    -- To Generation I
    pokemon_conversions_tests.conversions_test(
        "Bulbasaur", "", "Red", "Yellow"
    )

    -- To Generation II
    pokemon_conversions_tests.conversions_test(
        "Squirtle", "", "Blue", "Gold"
    )
end

function test_pokemon_conversion_from_gen2()
    -- To Generation II
    pokemon_conversions_tests.conversions_test(
        "Cyndaquil", "", "Gold", "Crystal"
    )
    pokemon_conversions_tests.conversions_test(
        "Totodile", "", "Crystal", "Gold"
    )

    -- To Generation I
    pokemon_conversions_tests.conversions_test(
        "Charmander", "", "Silver", "Blue"
    )
end

function test_pokemon_conversion_from_gba()
    -- To GBA
    pokemon_conversions_tests.conversions_test(
        "Torchic", "", "Ruby", "Sapphire"
    )
    pokemon_conversions_tests.conversions_test(
        "Mudkip", "", "Ruby", "Emerald"
    )
    pokemon_conversions_tests.conversions_test(
        "Treecko", "", "Ruby", "FireRed"
    )
    pokemon_conversions_tests.conversions_test(
        "Torchic", "", "Emerald", "Sapphire"
    )
    pokemon_conversions_tests.conversions_test(
        "Mudkip", "", "Emerald", "Emerald"
    )
    pokemon_conversions_tests.conversions_test(
        "Treecko", "", "Emerald", "FireRed"
    )
    pokemon_conversions_tests.conversions_test(
        "Charmander", "", "FireRed", "Ruby"
    )
    pokemon_conversions_tests.conversions_test(
        "Squirtle", "", "FireRed", "Emerald"
    )
    pokemon_conversions_tests.conversions_test(
        "Bulbasaur", "", "FireRed", "FireRed"
    )
    pokemon_conversions_tests.conversions_test(
        "Bulbasaur", "", "FireRed", "FireRed"
    )

    -- To GCN
    pokemon_conversions_tests.conversions_test(
        "Eevee", "", "Ruby", "Colosseum"
    )
    pokemon_conversions_tests.conversions_test(
        "Espeon", "", "Emerald", "Colosseum"
    )
    pokemon_conversions_tests.conversions_test(
        "Umbreon", "", "FireRed", "Colosseum"
    )
    pokemon_conversions_tests.conversions_test(
        "Eevee", "", "Ruby", "XD"
    )
    pokemon_conversions_tests.conversions_test(
        "Espeon", "", "Emerald", "XD"
    )
    pokemon_conversions_tests.conversions_test(
        "Umbreon", "", "FireRed", "XD"
    )
end

function test_pokemon_conversion_from_gcn()
    -- To GBA
    pokemon_conversions_tests.conversions_test(
        "Eevee", "", "Colosseum", "Sapphire"
    )
    pokemon_conversions_tests.conversions_test(
        "Espeon", "", "Colosseum", "Emerald"
    )
    pokemon_conversions_tests.conversions_test(
        "Umbreon", "", "Colosseum", "LeafGreen"
    )
    pokemon_conversions_tests.conversions_test(
        "Eevee", "", "XD", "Sapphire"
    )
    pokemon_conversions_tests.conversions_test(
        "Espeon", "", "XD", "Emerald"
    )
    pokemon_conversions_tests.conversions_test(
        "Umbreon", "", "XD", "LeafGreen"
    )

    -- To GCN
    pokemon_conversions_tests.conversions_test(
        "Vaporeon", "", "Colosseum", "Colosseum"
    )
    pokemon_conversions_tests.conversions_test(
        "Jolteon", "", "Colosseum", "XD"
    )
    pokemon_conversions_tests.conversions_test(
        "Vaporeon", "", "XD", "XD"
    )
    pokemon_conversions_tests.conversions_test(
        "Jolteon", "", "XD", "Colosseum"
    )
end
