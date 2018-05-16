--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")
local pkmntest_utils = require("pkmntest_utils")

local pokemon_conversions_tests = {}

function pokemon_conversions_tests.conversions_test(species, form, origin_game, dest_game)
    local first_pokemon = pkmn.pokemon(species, origin_game, form, 50)

    local origin_generation = pkmntest_utils.GAME_TO_GENERATION[origin_game]
    local dest_generation = pkmntest_utils.GAME_TO_GENERATION[dest_game]
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

    local first_pokemon = pkmntest_utils.get_specific_random_pokemon(origin_game, species, form, moves, items)

    -- Convert to the second game and compare.
    local second_pokemon = first_pokemon:to_game(dest_game)

    luaunit.assertEquals(first_pokemon.species, second_pokemon.species)
    luaunit.assertEquals(dest_game, second_pokemon.game)
    luaunit.assertEquals(first_pokemon.form, second_pokemon.form)
    luaunit.assertEquals(first_pokemon.nickname, second_pokemon.nickname)
    luaunit.assertEquals(first_pokemon.original_trainer_name, second_pokemon.original_trainer_name)
    luaunit.assertEquals(first_pokemon.original_trainer_id, second_pokemon.original_trainer_id)
    luaunit.assertEquals(first_pokemon.original_trainer_public_id, second_pokemon.original_trainer_public_id)
    luaunit.assertEquals(first_pokemon.experience, second_pokemon.experience)
    luaunit.assertEquals(first_pokemon.level, second_pokemon.level)

    if origin_generation == dest_generation
    then
        luaunit.assertEquals(first_pokemon.EVs.keys, second_pokemon.EVs.keys)
        for EV_index = 1, #first_pokemon.EVs
        do
            luaunit.assertEquals(
                first_pokemon.EVs[first_pokemon.EVs.keys[EV_index]],
                second_pokemon.EVs[second_pokemon.EVs.keys[EV_index]]
            )
        end

        luaunit.assertEquals(first_pokemon.IVs.keys, second_pokemon.IVs.keys)
        for IV_index = 1, #first_pokemon.IVs
        do
            luaunit.assertEquals(
                first_pokemon.IVs[first_pokemon.IVs.keys[IV_index]],
                second_pokemon.IVs[second_pokemon.IVs.keys[IV_index]]
            )
        end
    end

    for move_index = 1, 4
    do
        luaunit.assertEquals(first_pokemon.moves[move_index].move, second_pokemon.moves[move_index].move)
        luaunit.assertEquals(first_pokemon.moves[move_index].pp, second_pokemon.moves[move_index].pp)
    end

    if min_generation >= 3
    then
        luaunit.assertEquals(first_pokemon.original_trainer_secret_id, second_pokemon.original_trainer_secret_id)
        luaunit.assertEquals(first_pokemon.ability, second_pokemon.ability)
        luaunit.assertEquals(first_pokemon.ball, second_pokemon.ball)
        luaunit.assertEquals(first_pokemon.original_game, second_pokemon.original_game)
        luaunit.assertEquals(first_pokemon.personality, second_pokemon.personality)

        if origin_generation == dest_generation
        then
            luaunit.assertEquals(first_pokemon.markings.keys, second_pokemon.markings.keys)
            for marking_index = 1, #first_pokemon.markings
            do
                luaunit.assertEquals(
                    first_pokemon.markings[first_pokemon.markings.keys[marking_index]],
                    second_pokemon.markings[second_pokemon.markings.keys[marking_index]]
                )
            end

            luaunit.assertEquals(first_pokemon.ribbons.keys, second_pokemon.ribbons.keys)
            for ribbon_index = 1, #first_pokemon.ribbons
            do
                luaunit.assertEquals(
                    first_pokemon.ribbons[first_pokemon.ribbons.keys[ribbon_index]],
                    second_pokemon.ribbons[second_pokemon.ribbons.keys[ribbon_index]]
                )
            end

            luaunit.assertEquals(first_pokemon.contest_stats.keys, second_pokemon.contest_stats.keys)
            for contest_stat_index = 1, #first_pokemon.contest_stats
            do
                luaunit.assertEquals(
                    first_pokemon.contest_stats[first_pokemon.contest_stats.keys[contest_stat_index]],
                    second_pokemon.contest_stats[second_pokemon.contest_stats.keys[contest_stat_index]]
                )
            end
        end
    end

    if min_generation >= 2
    then
        luaunit.assertEquals(first_pokemon.original_trainer_gender, second_pokemon.original_trainer_gender)
        luaunit.assertEquals(first_pokemon.gender, second_pokemon.gender)
        luaunit.assertEquals(first_pokemon.is_shiny, second_pokemon.is_shiny)
        luaunit.assertEquals(first_pokemon.held_item, second_pokemon.held_item)
        luaunit.assertEquals(first_pokemon.current_trainer_friendship, second_pokemon.current_trainer_friendship)
        luaunit.assertEquals(first_pokemon.level, second_pokemon.level_met)
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
