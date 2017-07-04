#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"

require "minitest/autorun"
require "os"
require "pathname"

class PokemonTest < MiniTest::Test
    @@GB_EV_MAX = 65535
    @@MODERN_EV_MAX = 255

    @@GB_IV_MAX = 15
    @@MODERN_IV_MAX = 31

    @@STAT_MAX = 65536

    @@GAME_GENERATIONS = {
        "Red" => 1,
        "Blue" => 1,
        "Yellow" => 1,
        "Gold" => 2,
        "Silver" => 2,
        "Crystal" => 2,
        "Ruby" => 3,
        "Sapphire" => 3,
        "Emerald" => 3,
        "FireRed" => 3,
        "LeafGreen" => 3,
        "Colosseum" => 3,
        "XD" => 3,
        "Diamond" => 4,
        "Pearl" => 4,
        "Platinum" => 4,
        "HeartGold" => 4,
        "SoulSilver" => 4,
        "Black" => 5,
        "White" => 5,
        "Black 2" => 5,
        "White 2" => 5,
        "X" => 6,
        "Y" => 6,
        "Omega Ruby" => 6,
        "Alpha Sapphire" => 6
    }

    def check_initial_values(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        assert_equal("Standard", pokemon.form)

        if generation >= 5
            assert_equal(pokemon.species, pokemon.nickname)
        else
            assert_equal(pokemon.species.upcase, pokemon.nickname)
        end

        assert_equal(PKMN::Pokemon::DEFAULT_TRAINER_NAME, pokemon.trainer_name)

        if generation >= 2
            assert_equal("None", pokemon.held_item.name)
        end

        assert_equal("Male", pokemon.trainer_gender)
        assert_equal(PKMN::Pokemon::DEFAULT_TRAINER_ID & 0xFFFF, pokemon.trainer_public_id)

        if generation >= 3
            assert_equal(PKMN::Pokemon::DEFAULT_TRAINER_ID >> 16, pokemon.trainer_secret_id)
            assert_equal(PKMN::Pokemon::DEFAULT_TRAINER_ID, pokemon.trainer_id)
        else
            assert_equal(PKMN::Pokemon::DEFAULT_TRAINER_ID & 0xFFFF, pokemon.trainer_id)
        end

        if generation >= 2
            assert_equal(pokemon.database_entry.base_friendship, pokemon.friendship)
        end

        if generation >= 3
            assert_equal("Premier Ball", pokemon.ball)
            assert_equal(pokemon.game, pokemon.original_game)
        end

        assert_equal(pokemon.database_entry.get_experience_at_level(30), pokemon.experience)
        assert_equal(30, pokemon.level)

        if generation >= 2
            assert_equal(pokemon.level, pokemon.level_met)
        end

        assert_equal(4, pokemon.moves.length)
        pokemon.moves.each do |move|
            assert_equal("None", move.move.name)
            assert_equal(0, move.pp)
        end

        assert(Pathname.new(pokemon.icon_filepath).exists?)
        assert(Pathname.new(pokemon.sprite_filepath).exists?)
    end

    def check_initial_maps(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        [pokemon.EVs, pokemon.IVs, pokemon.stats].each do |map|
            ["HP", "Attack", "Defense", "Speed"].each do |stat|
                assert(map.has_key(stat))
            end
            if generation >= 3
                assert(!map.has_key("Special"))
                assert(map.has_key("Special Attack"))
                assert(map.has_key("Special Defense"))
            else
                assert(map.has_key("Special"))
                assert(!map.has_key("Special Attack"))
                assert(!map.has_key("Special Defense"))
            end
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
                assert(pokemon.contest_stats.has_key(stat))
                assert_equal(0, pokemon.contest_stats[stat])
            end
            if generation == 3
                assert(pokemon.contest_stats.has_key("Feel"))
                assert(!pokemon.contest_stats.has_key("Sheen"))
                assert_equal(0, pokemon.contest_stats["Feel"])
            else
                assert(!pokemon.contest_stats.has_key("Feel"))
                assert(pokemon.contest_stats.has_key("Sheen"))
                assert_equal(0, pokemon.contest_stats["Sheen"])
            end

            ["Circle", "Triangle", "Square", "Heart"].each do |marking|
                assert(pokemon.markings.has_key(marking))
                assert(!pokemon.markings[marking])
            end
            if generation > 3
                ["Star", "Diamond"].each do |marking|
                    assert(pokemon.markings.has_key(marking))
                    assert(!pokemon.markings[marking])
                end
            else
                assert(!pokemon.markings.has_key("Star"))
                assert(!pokemon.markings.has_key("Diamond"))
            end
        else
            assert_raises RuntimeError do
                pokemon.contest_stats
            end
            assert_raises RuntimeError do
                pokemon.markings
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
            assert_raises ArgumentError do
                pokemon.ability
            end
            assert_raises ArgumentError do
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
            assert_raises RuntimeError do
                pokemon.ball
            end
            assert_raises RuntimeError do
                pokemon.ball = "Great Ball"
            end
        end
    end

    def _test_setting_friendship(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        if generation >= 2
            pokemon.friendship = 123
            assert_equal(123, pokemon.friendship)

            assert_raises IndexError do
                pokemon.friendship = -1
            end
            assert_raises IndexError do
                pokemon.friendship = 256
            end
        else
            assert_raises IndexError do
                pokemon.friendship
            end
            assert_raises IndexError do
                pokemon.friendship = 123
            end
        end
    end

    def _test_setting_item(pokemon, item_name, invalid_item_names)
        generation = @@GAME_GENERATIONS[pokemon.game]

        if generation >= 2
            pokemon.set_held_item(item_name)
            assert_equal(item_name, pokemon.get_held_item().name)

            invalid_item_names.each do |invalid_item_name|
                assert_raises ArgumentError do
                    pokemon.set_held_item(invalid_item_name)
                end
            end
        else
        end
    end

    def _test_setting_level(pokemon)
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
        assert(pokemon.database_entry.get_experience_at_level(pokemon.level-1) < experience)
        assert(pokemon.database_entry.get_experience_at_level(pokemon.level) <= experience)

        case generation
        when 1
            assert_raises IndexError do
                pokemon.level = 1
            end
            assert_raises ArgumentError do
                pokemon.level_met
            end
            assert_raises ArgumentError do
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
            assert_raises RuntimeError do
                pokemon.get_location_met(true)
            end
            assert_raises RuntimeError do
                pokemon.set_location_met(valid_locations[0], true)
            end
            assert_raises RuntimeError do
                pokemon.get_location_met(false)
            end
            assert_raises RuntimeError do
                pokemon.set_location_met(valid_locations[0], false)
            end
        when 2..3
            assert_equals(expected_original_location, pokemon.get_location_met(false))
            valid_locations.each do |location|
                pokemon.set_location_met(location, false)
                assert_equals(location, pokemon.get_location_met(false))
            end
            invalid_locations.each do |location|
                assert_raises ArgumentError do
                    pokemon.set_location_met(location, false)
                end
            end

            assert_raises RuntimeError do
                pokemon.get_location_met(true)
            end
            assert_raises RuntimeError do
                pokemon.set_location_met(valid_locations[0], true)
            end
        else
            [true, false].each do |as_egg|
                assert_equals(expected_original_location, pokemon.get_location_met(as_egg))
                valid_locations.each do |location|
                    pokemon.set_location_met(location, as_egg)
                    assert_equals(location, pokemon.get_location_met(as_egg))
                end
                invalid_locations.each do |location|
                    assert_raises ArgumentError do
                        pokemon.set_location_met(location, as_egg)
                    end
                end
            end
        end
    end

    def _test_setting_markings(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        if generation >= 3
            pokemon.markings.each do |marking|
                pokemon.set_marking(marking, false)
                assert(!pokemon.markings[marking])
                pokemon.set_marking(marking, true)
                assert(pokemon.markings[marking])
            end
        else
            assert_raises RuntimeError do
                pokemon.markings["Circle"]
            end
            assert_raises RuntimeError do
                pokemon.set_marking("Circle", false)
            end
        end
    end

    def _test_setting_moves(pokemon, valid_moves, invalid_moves)
        assert_equal(4, pokemon.moves.length)

        (0..3).each do |i|
            pokemon.set_move(valid_moves[i], i)
        end

        assert_raises IndexError do
            pokemon.set_move(valid_moves[0], -1)
        end
        assert_raises IndexError do
            pokemon.set_move(valid_moves[0], 4)
        end

        (0..3).each do |i|
            assert_equal(valid_moves[i], pokemon.moves[i].move.name)
            assert_equal(pokemon.moves[i].move.get_pp(0), pokemon.moves[i].pp)
        end

        invalid_moves.each do |invalid_move|
            assert_raises ArgumentError do
                pokemon.set_move(invalid_move, 0)
            end
        end
    end

    def _test_setting_original_game(pokemon, games, invalid_games)
        assert(games.length > 0)

        generation == @@GAME_GENERATIONS[pokemon.game]

        if generation >= 3
            games.each do |game|
                pokemon.game = game
                assert_equal(game, pokemon.game)
            end

            invalid_games.each do |game|
                assert_raises ArgumentError do
                    pokemon.game = game
                end
            end
        else
            assert_raises RuntimeError do
                pokemon.game
            end
            assert_raises RuntimeError do
                pokemon.game = games[0]
            end
        end
    end

    def _test_setting_personality(pokemon)
        generation = @@GAME_GENERATIONS[pokemon.game]

        if generation >= 3
            pokemon.personality = 0x7F3AB3A8
            assert_equal(0x7F3AB3A8, pokemon.personality)
        else
            assert_raises RuntimeError do
                pokemon.personality
            end
            assert_raises RuntimeError do
                pokemon.personality = 0
            end
        end
    end

    def _test_setting_stats(pokemon)
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
            pokemon.trainer_name = ""
        end
        assert_raises ArgumentError do
            pokemon.trainer_name = "Too long trainer_name"
        end
        pokemon.trainer_name = "foobar"
        assert_equal("foobar", pokemon.trainer_name)

        if generation >= 2
            pokemon.trainer_gender = "Male"
            assert_equal("Male", pokemon.trainer_gender)
            pokemon.trainer_gender = "Female"
            assert_equal("Female", pokemon.trainer_gender)

            assert_raises ArgumentError do
                pokemon.trainer_gender = "Genderless"
            end
        else
            assert_raises RuntimeError do
                pokemon.trainer_gender
            end
            assert_raises RuntimeError do
                pokemon.trainer_gender = "Male"
            end
        end
    end
end
