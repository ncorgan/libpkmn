#!/usr/bin/ruby
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"
require "PKMNTest"

require "minitest/autorun"
require "os"
require "pathname"

class PokemonTest < PKMNTest
    @@GB_EV_MAX = 65535
    @@MODERN_EV_MAX = 255

    @@GB_IV_MAX = 15
    @@MODERN_IV_MAX = 31

    @@STAT_MAX = 65536

    @@RNG = Random.new

    def _check_initial_values(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        assert_equal("Standard", pokemon.form)

        if generation >= 5
            assert_equal(pokemon.species, pokemon.nickname)
        else
            assert_equal(pokemon.species.upcase, pokemon.nickname)
        end

        assert_equal(PKMN::Pokemon.DEFAULT_TRAINER_NAME, pokemon.original_trainer_name)

        if generation >= 2
            assert_equal("None", pokemon.held_item)
            assert_equal("Male", pokemon.original_trainer_gender)
        else
            assert_equal("", pokemon.held_item)
            assert_equal("", pokemon.original_trainer_gender)
        end

        assert_equal(PKMN::Pokemon.DEFAULT_TRAINER_ID & 0xFFFF, pokemon.original_trainer_public_id)

        if generation >= 3
            assert_equal(PKMN::Pokemon.DEFAULT_TRAINER_ID >> 16, pokemon.original_trainer_secret_id)
            assert_equal(PKMN::Pokemon.DEFAULT_TRAINER_ID, pokemon.original_trainer_id)
        else
            assert_equal(PKMN::Pokemon.DEFAULT_TRAINER_ID & 0xFFFF, pokemon.original_trainer_id)
        end

        if generation >= 2
            assert_equal(
                pokemon.database_entry.base_friendship,
                pokemon.current_trainer_friendship
            )
        end

        if generation >= 3
            assert_equal("Premier Ball", pokemon.ball)
            if pokemon.game == "Colosseum" or pokemon.game == "XD"
                assert_equal("Colosseum/XD", pokemon.original_game)
            else
                assert_equal(pokemon.game, pokemon.original_game)
            end
        end

        assert_equal(pokemon.database_entry.get_experience_at_level(30), pokemon.experience)
        assert_equal(30, pokemon.level)

        if generation >= 2
            assert_equal(pokemon.level, pokemon.level_met)
        end

        assert_equal(4, pokemon.moves.length)
        pokemon.moves.each do |move|
            assert_equal("None", move.move)
            assert_equal(0, move.pp)
        end

        assert(Pathname.new(pokemon.icon_filepath).exist?)
        assert(Pathname.new(pokemon.sprite_filepath).exist?)
    end

    def _check_initial_maps(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        [pokemon.EVs, pokemon.IVs].each do |map|
            ["HP", "Attack", "Defense", "Speed"].each do |stat|
                assert(map.keys.include?(stat))
            end
            if generation >= 3
                assert(!map.keys.include?("Special"))
                assert(map.keys.include?("Special Attack"))
                assert(map.keys.include?("Special Defense"))
            else
                assert(map.keys.include?("Special"))
                assert(!map.keys.include?("Special Attack"))
                assert(!map.keys.include?("Special Defense"))
            end
        end

        ["HP", "Attack", "Defense", "Speed"].each do |stat|
            assert(pokemon.stats.has_key?(stat))
        end
        if generation >= 2
            assert(!pokemon.stats.keys.include?("Special"))
            assert(pokemon.stats.keys.include?("Special Attack"))
            assert(pokemon.stats.keys.include?("Special Defense"))
        else
            assert(pokemon.stats.keys.include?("Special"))
            assert(!pokemon.stats.keys.include?("Special Attack"))
            assert(!pokemon.stats.keys.include?("Special Defense"))
        end

        pokemon.EVs.each do |stat|
            if generation >= 3
                assert(pokemon.EVs[stat] <= @@MODERN_EV_MAX)
            else
                assert(pokemon.EVs[stat] <= @@GB_EV_MAX)
            end
        end
        pokemon.IVs.each do |stat|
            if generation >= 3
                assert(pokemon.IVs[stat] <= @@MODERN_IV_MAX)
            else
                assert(pokemon.IVs[stat] <= @@GB_IV_MAX)
            end
        end
        pokemon.stats.each do |stat|
            assert(pokemon.stats[stat] <= @@STAT_MAX)
        end

        if generation >= 3
            ["Cool", "Beauty", "Cute", "Smart", "Tough"].each do |stat|
                assert(pokemon.contest_stats.keys.include?(stat))
                assert_equal(0, pokemon.contest_stats[stat])
            end
            if generation == 3
                assert(pokemon.contest_stats.keys.include?("Feel"))
                assert(!pokemon.contest_stats.keys.include?("Sheen"))
                assert_equal(0, pokemon.contest_stats["Feel"])
            else
                assert(!pokemon.contest_stats.keys.include?("Feel"))
                assert(pokemon.contest_stats.keys.include?("Sheen"))
                assert_equal(0, pokemon.contest_stats["Sheen"])
            end

            ["Circle", "Triangle", "Square", "Heart"].each do |marking|
                assert(pokemon.markings.keys.include?(marking))
                assert(!pokemon.markings[marking])
            end
            if generation > 3
                ["Star", "Diamond"].each do |marking|
                    assert(pokemon.markings.keys.include?(marking))
                    assert(!pokemon.markings[marking])
                end
            else
                assert(!pokemon.markings.keys.include?("Star"))
                assert(!pokemon.markings.keys.include?("Diamond"))
            end
        else
            assert_raises RuntimeError do
                pokemon.contest_stats["Cool"]
            end
            assert_raises RuntimeError do
                pokemon.markings["Circle"]
            end
        end
    end

    def _test_image_filepaths(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        assert(Pathname.new(pokemon.icon_filepath).exist?)

        if generation >= 2
            ["Male", "Female"].each do |gender|
                pokemon.gender = gender
                [true, false].each do |shininess|
                    pokemon.is_shiny = shininess
                    assert(Pathname.new(pokemon.sprite_filepath).exist?)
                end
            end
        else
            assert(Pathname.new(pokemon.sprite_filepath).exist?)
        end
    end

    def _test_setting_ability(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        if generation >= 3
            abilities = pokemon.database_entry.abilities
            assert(abilities.first != "None")

            pokemon.ability = abilities.first
            assert_equal(abilities.first, pokemon.ability)
            if abilities.second != "None"
                pokemon.ability = abilities.second
                assert_equal(abilities.second, pokemon.ability)
            end

            if generation >= 5
                pokemon.ability = pokemon.database_entry.hidden_ability
                assert_equal(pokemon.database_entry.hidden_ability, pokemon.ability)
            end

            assert_raises ArgumentError do
                pokemon.ability = "Not an ability"
            end
            assert_raises ArgumentError do
                pokemon.ability = "Wonder Guard"
            end
        else
            assert_equal(pokemon.ability, "")
            assert_raises RuntimeError do
                pokemon.ability = "Wonder Guard"
            end
        end
    end

    def _test_setting_ball(pokemon, ball, invalid_balls)
        generation = @@GAME_GENERATIONS[pokemon.game]

        if generation >= 3
            pokemon.ball = ball
            assert_equal(ball, pokemon.ball)

            invalid_balls.each do |invalid_ball|
                assert_raises ArgumentError do
                    pokemon.ball = invalid_ball
                end
            end
        else
            assert_equal(pokemon.ball, "")
            assert_raises RuntimeError do
                pokemon.ball = "Great Ball"
            end
        end
    end

    def _test_setting_condition(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        conditions = ["None", "Asleep", "Poison", "Burn", "Frozen", "Paralysis"]
        if generation >= 3
            conditions << "Bad Poison"
        end

        conditions.each do |condition|
            pokemon.condition = condition
            assert_equal(condition, pokemon.condition)
        end
    end

    def _test_setting_friendship(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        if generation >= 2
            pokemon.current_trainer_friendship = 123
            assert_equal(123, pokemon.current_trainer_friendship)

            assert_raises IndexError do
                pokemon.current_trainer_friendship = -1
            end
            assert_raises IndexError do
                pokemon.current_trainer_friendship = 256
            end
        else
            assert_equal(pokemon.current_trainer_friendship, 0)
            assert_raises RuntimeError do
                pokemon.current_trainer_friendship = 123
            end
        end
    end

    def _test_setting_item(pokemon, item_name, invalid_item_names)
        generation = @@GAME_GENERATIONS[pokemon.game]

        if generation >= 2
            pokemon.held_item = item_name
            assert_equal(item_name, pokemon.held_item)

            invalid_item_names.each do |invalid_item_name|
                assert_raises ArgumentError do
                    pokemon.held_item = invalid_item_name
                end
            end
        else
        end
    end

    def _test_setting_levels(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        assert_raises IndexError do
            pokemon.level = -1
        end
        assert_raises IndexError do
            pokemon.level = 101
        end

        pokemon.level = 50
        assert_equal(50, pokemon.level)
        assert_equal(50, pokemon.database_entry.get_level_at_experience(pokemon.experience))

        pokemon.experience = 123456
        assert_equal(123456, pokemon.experience)
        assert(pokemon.database_entry.get_experience_at_level(pokemon.level-1) < pokemon.experience)
        assert(pokemon.database_entry.get_experience_at_level(pokemon.level) <= pokemon.experience)

        case generation
        when 1
            assert_equal(pokemon.level_met, 0)
            assert_raises IndexError do
                pokemon.level = 1
            end
            assert_raises RuntimeError do
                pokemon.level_met = 10
            end
        when 2
            assert_raises IndexError do
                pokemon.level = 1
            end
            assert_raises IndexError do
                pokemon.level_met = 1
            end
            assert_raises IndexError do
                pokemon.level_met = 101
            end

            pokemon.level_met = 5
            assert_equal(5, pokemon.level_met)
        else
            assert_raises IndexError do
                pokemon.level = -1
            end
            assert_raises IndexError do
                pokemon.level_met = -1
            end
            assert_raises IndexError do
                pokemon.level_met = 101
            end

            pokemon.level_met = 5
            assert_equal(5, pokemon.level_met)
        end
    end

    def _test_setting_location_met(pokemon, expected_original_location, valid_locations, invalid_locations)
        generation = @@GAME_GENERATIONS[pokemon.game]
        assert(valid_locations.length > 0)

        case generation
        when 1
            assert_equal("", pokemon.location_met)
            assert_equal("", pokemon.location_met_as_egg)

            assert_raises RuntimeError do
                pokemon.location_met = valid_locations[0]
            end
            assert_raises RuntimeError do
                pokemon.location_met_as_egg = valid_locations[0]
            end
        when 2..3
            assert_equal(expected_original_location, pokemon.location_met)
            assert_equal("", pokemon.location_met_as_egg)

            valid_locations.each do |location|
                pokemon.location_met = location
                assert_equal(location, pokemon.location_met)
            end
            invalid_locations.each do |location|
                assert_raises ArgumentError do
                    pokemon.location_met = location
                end
            end
        else
            assert_equal(expected_original_location, pokemon.location_met)
            assert_equal(expected_original_location, pokemon.location_met_as_egg)

            valid_locations.each do |location|
                pokemon.location_met = location
                assert_equal(location, pokemon.location_met)

                pokemon.location_met_as_egg = location
                assert_equal(location, pokemon.location_met_as_egg)
            end
            invalid_locations.each do |location|
                assert_raises ArgumentError do
                    pokemon.location_met = location
                end
                assert_raises ArgumentError do
                    pokemon.location_met_as_egg = location
                end
            end
        end
    end

    def _test_setting_markings(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        if generation >= 3
            pokemon.markings.each do |marking|
                pokemon.markings[marking] = false
                assert(!pokemon.markings[marking])
                pokemon.markings[marking] = true
                assert(pokemon.markings[marking])
            end
        else
            assert_raises RuntimeError do
                pokemon.markings["Circle"]
            end
            assert_raises RuntimeError do
                pokemon.markings["Circle"] = false
            end
        end
    end

    def _test_setting_moves(pokemon, valid_moves, invalid_moves)
        assert_equal(4, pokemon.moves.length)

        (0..3).each do |index|
            pokemon.moves[index].move = valid_moves[index]
        end

        assert_raises IndexError do
            pokemon.moves[-1].move = valid_moves[0]
        end
        assert_raises IndexError do
            pokemon.moves[4].move = valid_moves[0]
        end

        (0..3).each do |index|
            assert_equal(
                valid_moves[index],
                pokemon.moves[index].move
            )
            assert_equal(
                PKMN::Database::MoveEntry.new(valid_moves[index], pokemon.game).get_pp(0),
                pokemon.moves[index].pp
            )
        end

        invalid_moves.each do |invalid_move|
            assert_raises ArgumentError do
                pokemon.moves[0].move = invalid_move
            end
        end
    end

    def _test_setting_original_game(pokemon, games, invalid_games)
        assert(games.length > 0)

        generation = @@GAME_GENERATIONS[pokemon.game]

        if generation >= 3
            games.each do |game|
                pokemon.original_game = game
                if game.include? "Colosseum" or game.include? "XD"
                    assert_equal("Colosseum/XD", pokemon.original_game)
                else
                    assert_equal(game, pokemon.original_game)
                end
            end

            invalid_games.each do |game|
                assert_raises ArgumentError do
                    pokemon.original_game = game
                end
            end
        else
            assert_equal("", pokemon.original_game)
            assert_raises RuntimeError do
                pokemon.original_game = games[0]
            end
        end
    end

    def _test_setting_personality(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        if generation >= 3
            pokemon.personality = 0x7F3AB3A8
            assert_equal(0x7F3AB3A8, pokemon.personality)

            # Make sure SWIG+Ruby catches invalid inputs.
            assert_raises RangeError do
                pokemon.personality = -1
            end
            assert_raises RangeError do
                pokemon.personality = 0xFFFFFFFF+1
            end

            assert_equal(0x7F3AB3A8, pokemon.personality)
        else
            assert_equal(0, pokemon.personality)
            assert_raises RuntimeError do
                pokemon.personality = 0
            end
        end
    end

    def _test_setting_pokerus(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        if generation >= 2
            pokerus_duration = @@RNG.rand(16)

            pokemon.pokerus_duration = pokerus_duration
            assert_equal(pokerus_duration, pokemon.pokerus_duration)

            assert_raises IndexError do
                pokemon.pokerus_duration = -1
            end
            assert_raises IndexError do
                pokemon.pokerus_duration = 16
            end
        else
            assert_equal(0, pokemon.pokerus_duration)

            assert_raises RuntimeError do
                pokemon.pokerus_duration = 1
            end
        end
    end

    def _test_setting_stats(pokemon)
        pokemon.EVs.each do |ev|
            new_value = 0
            if @@GAME_GENERATIONS[pokemon.game] >= 3
                new_value = @@RNG.rand(@@MODERN_EV_MAX+1)
            else
                new_value = @@RNG.rand(@@GB_EV_MAX+1)
            end

            pokemon.EVs[ev] = new_value
            assert_equal(new_value, pokemon.EVs[ev])
        end

        pokemon.IVs.each do |iv|
            new_value = 0
            if @@GAME_GENERATIONS[pokemon.game] >= 3
                new_value = @@RNG.rand(@@MODERN_IV_MAX+1)
            else
                new_value = @@RNG.rand(@@GB_IV_MAX+1)
            end

            pokemon.IVs[iv] = new_value
            assert_equal(new_value, pokemon.IVs[iv])
        end

        if @@GAME_GENERATIONS[pokemon.game] >= 3
            pokemon.contest_stats.each do |contest_stat|
                new_value = @@RNG.rand(256)

                pokemon.contest_stats[contest_stat] = new_value
                assert_equal(new_value, pokemon.contest_stats[contest_stat])
            end
        end
    end

    def _test_setting_trainer_info(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        assert_raises ArgumentError do
            pokemon.nickname = ""
        end
        assert_raises ArgumentError do
            pokemon.nickname = "Too long nickname"
        end
        pokemon.nickname = "foobarbaz"
        assert_equal("foobarbaz", pokemon.nickname)

        assert_raises ArgumentError do
            pokemon.original_trainer_name = ""
        end
        assert_raises ArgumentError do
            pokemon.original_trainer_name = "Too long trainer_name"
        end
        pokemon.original_trainer_name = "foobar"
        assert_equal("foobar", pokemon.original_trainer_name)

        if generation >= 2
            pokemon.original_trainer_gender = "Male"
            assert_equal("Male", pokemon.original_trainer_gender)
            pokemon.original_trainer_gender = "Female"
            assert_equal("Female", pokemon.original_trainer_gender)

            assert_raises ArgumentError do
                pokemon.original_trainer_gender = "Genderless"
            end
        else
            assert_equal("", pokemon.original_trainer_gender)
            assert_raises RuntimeError do
                pokemon.original_trainer_gender = "Male"
            end
        end
    end

    def pokemon_test_common(pokemon, params)
        _check_initial_maps(pokemon)
        _check_initial_values(pokemon)
        _test_setting_ability(pokemon)
        _test_setting_ball(
            pokemon,
            params.valid_ball,
            params.invalid_balls
        )
        _test_setting_condition(pokemon)
        _test_image_filepaths(pokemon)
        _test_setting_friendship(pokemon)
        _test_setting_item(
            pokemon,
            params.valid_item,
            params.invalid_items
        )
        _test_setting_levels(pokemon)
        _test_setting_location_met(
            pokemon,
            params.expected_original_location,
            params.valid_locations,
            params.invalid_locations
        )
        _test_setting_markings(pokemon)
        _test_setting_moves(
            pokemon,
            params.valid_moves,
            params.invalid_moves
        )
        _test_setting_original_game(
            pokemon,
            params.valid_original_games,
            params.invalid_original_games
        )
        _test_setting_personality(pokemon)
        _test_setting_pokerus(pokemon)
        _test_setting_stats(pokemon)
        _test_setting_trainer_info(pokemon)
    end
end
