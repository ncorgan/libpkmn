--
-- Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
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
        pkmn.species.BULBASAUR, "", pkmn.game.RED, pkmn.game.YELLOW
    )

    -- To Generation II
    pokemon_conversions_tests.conversions_test(
        pkmn.species.SQUIRTLE, "", pkmn.game.BLUE, pkmn.game.GOLD
    )
end

function test_pokemon_conversion_from_gen2()
    -- To Generation II
    pokemon_conversions_tests.conversions_test(
        pkmn.species.CYNDAQUIL, "", pkmn.game.GOLD, pkmn.game.CRYSTAL
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.TOTODILE, "", pkmn.game.CRYSTAL, pkmn.game.GOLD
    )

    -- To Generation I
    pokemon_conversions_tests.conversions_test(
        pkmn.species.CHARMANDER, "", pkmn.game.SILVER, pkmn.game.BLUE
    )
end

function test_pokemon_conversion_from_gba()
    -- To GBA
    pokemon_conversions_tests.conversions_test(
        pkmn.species.TORCHIC, "", pkmn.game.RUBY, pkmn.game.SAPPHIRE
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.MUDKIP, "", pkmn.game.RUBY, pkmn.game.EMERALD
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.TREECKO, "", pkmn.game.RUBY, pkmn.game.FIRERED
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.TORCHIC, "", pkmn.game.EMERALD, pkmn.game.SAPPHIRE
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.MUDKIP, "", pkmn.game.EMERALD, pkmn.game.EMERALD
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.TREECKO, "", pkmn.game.EMERALD, pkmn.game.FIRERED
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.CHARMANDER, "", pkmn.game.FIRERED, pkmn.game.RUBY
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.SQUIRTLE, "", pkmn.game.FIRERED, pkmn.game.EMERALD
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.BULBASAUR, "", pkmn.game.FIRERED, pkmn.game.FIRERED
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.BULBASAUR, "", pkmn.game.FIRERED, pkmn.game.FIRERED
    )

    -- To GCN
    pokemon_conversions_tests.conversions_test(
        pkmn.species.EEVEE, "", pkmn.game.RUBY, pkmn.game.COLOSSEUM
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.ESPEON, "", pkmn.game.EMERALD, pkmn.game.COLOSSEUM
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.UMBREON, "", pkmn.game.FIRERED, pkmn.game.COLOSSEUM
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.EEVEE, "", pkmn.game.RUBY, pkmn.game.XD
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.ESPEON, "", pkmn.game.EMERALD, pkmn.game.XD
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.UMBREON, "", pkmn.game.FIRERED, pkmn.game.XD
    )
end

function test_pokemon_conversion_from_gcn()
    -- To GBA
    pokemon_conversions_tests.conversions_test(
        pkmn.species.EEVEE, "", pkmn.game.COLOSSEUM, pkmn.game.SAPPHIRE
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.ESPEON, "", pkmn.game.COLOSSEUM, pkmn.game.EMERALD
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.UMBREON, "", pkmn.game.COLOSSEUM, pkmn.game.LEAFGREEN
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.EEVEE, "", pkmn.game.XD, pkmn.game.SAPPHIRE
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.ESPEON, "", pkmn.game.XD, pkmn.game.EMERALD
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.UMBREON, "", pkmn.game.XD, pkmn.game.LEAFGREEN
    )

    -- To GCN
    pokemon_conversions_tests.conversions_test(
        pkmn.species.VAPOREON, "", pkmn.game.COLOSSEUM, pkmn.game.COLOSSEUM
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.JOLTEON, "", pkmn.game.COLOSSEUM, pkmn.game.XD
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.VAPOREON, "", pkmn.game.XD, pkmn.game.XD
    )
    pokemon_conversions_tests.conversions_test(
        pkmn.species.JOLTEON, "", pkmn.game.XD, pkmn.game.COLOSSEUM
    )
end
