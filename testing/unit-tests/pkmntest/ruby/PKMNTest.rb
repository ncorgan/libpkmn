#!/usr/bin/ruby
#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"

require "minitest/autorun"

class PKMNTest < MiniTest::Test
    @@GAME_GENERATIONS =
    {
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

    @@RNG = Random.new

    def random_bool()
        return (@@RNG.rand(100) < 50)
    end

    def compare_attributes(var1, var2)
        assert_equal(
            var1.numeric_attributes.names,
            var2.numeric_attributes.names
        )
        var1.numeric_attributes.each do |attribute_name, value|
            assert_equal(value, var2.numeric_attributes[attribute_name])
        end

        assert_equal(
            var1.string_attributes.names,
            var2.string_attributes.names
        )
        var1.string_attributes.each do |attribute_name, value|
            assert_equal(value, var2.string_attributes[attribute_name])
        end

        assert_equal(
            var1.boolean_attributes.names,
            var2.boolean_attributes.names
        )
        var1.boolean_attributes.each do |attribute_name, value|
            assert_equal(value, var2.boolean_attributes[attribute_name])
        end
    end

    def get_specific_random_pokemon(game, species, form, move_list, item_list)
        generation = @@GAME_GENERATIONS[game]

        # Level bound accounts for Gen II level met limitation
        pokemon = PKMN::Pokemon.new(species, game, form, (@@RNG.rand(61) + 2))

        (0..3).each do |move_index|
            move = ""
            loop do
                move = move_list[@@RNG.rand(move_list.size)]
                break if !move.start_with?("Shadow")
            end

            pokemon.moves[move_index].move = move
        end

        pokemon.EVs.each do |key|
            pokemon.EVs[key] = @@RNG.rand(256)
        end
        pokemon.IVs.each do |key|
            pokemon.IVs[key] = @@RNG.rand(16)
        end

        if generation >= 2
            # Keep going until one is holdable.
            while pokemon.held_item == "None"
                begin
                    pokemon.held_item = item_list[@@RNG.rand(item_list.size)]
                rescue
                end
            end

            pokemon.pokerus_duration = @@RNG.rand(16)
        end

        if generation >= 3
            pokemon.markings.each do |marking|
                pokemon.markings[marking] = random_bool()
            end
            pokemon.ribbons.each do |ribbon|
                pokemon.ribbons[ribbon] = random_bool()
            end
            pokemon.contest_stats.each do |contest_stat|
                pokemon.contest_stats[contest_stat] = @@RNG.rand(256)
            end
        end

        return pokemon
    end

    def get_random_pokemon(game, pokemon_list, move_list, item_list)
        generation = @@GAME_GENERATIONS[game]

        species = ""
        if generation == 3
            loop do
                species = pokemon_list[@@RNG.rand(pokemon_list.size)]
                break if species != "Deoxys"
            end
        else
            species = pokemon_list[@@RNG.rand(pokemon_list.size)]
        end

        return get_specific_random_pokemon(game, species, "", move_list, item_list)
    end

    def compare_pokemon(pokemon1, pokemon2)
        generation = @@GAME_GENERATIONS[pokemon1.game]

        # There is no way to tell what game an imported Generation I-II
        # Pokemon comes from, so LibPKMN defaults to a default valid game.
        if generation >= 3
            assert_equal(pokemon1.game, pokemon2.game)
        end

        assert_equal(pokemon1.species, pokemon2.species)
        assert_equal(pokemon1.form, pokemon2.form)
        assert_equal(pokemon1.original_trainer_id, pokemon2.original_trainer_id)
        assert_equal(pokemon1.experience, pokemon2.experience)
        assert_equal(pokemon1.level, pokemon2.level)
        assert_equal(pokemon1.nickname, pokemon2.nickname)
        assert_equal(pokemon1.original_trainer_name, pokemon2.original_trainer_name)

        assert_equal(pokemon1.EVs.keys, pokemon2.EVs.keys)
        pokemon1.EVs.each do |key, value|
            assert_equal(value, pokemon2.EVs[key])
        end
        assert_equal(pokemon1.IVs.keys, pokemon2.IVs.keys)
        pokemon1.IVs.each do |key, value|
            assert_equal(value, pokemon2.IVs[key])
        end
        assert_equal(pokemon1.stats.keys, pokemon2.stats.keys)
        pokemon1.stats.each do |key, value|
            assert_equal(value, pokemon2.stats[key])
        end

        if pokemon1.game == pokemon2.game
            assert_equal(pokemon1.icon_filepath, pokemon2.icon_filepath)
            assert_equal(pokemon1.sprite_filepath, pokemon2.sprite_filepath)
        end

        compare_attributes(pokemon1, pokemon2)

        if generation >= 2
            assert_equal(pokemon1.original_trainer_gender, pokemon2.original_trainer_gender)
            assert_equal(pokemon1.current_trainer_friendship, pokemon2.current_trainer_friendship)
            assert_equal(pokemon1.gender, pokemon2.gender)
            assert_equal(pokemon1.is_shiny, pokemon2.is_shiny)
            assert_equal(pokemon1.held_item, pokemon2.held_item)
            assert_equal(pokemon1.level_met, pokemon2.level_met)
            assert_equal(pokemon1.location_met, pokemon2.location_met)
            assert_equal(pokemon1.pokerus_duration, pokemon2.pokerus_duration)
        end

        if generation >= 3
            assert_equal(pokemon1.ability, pokemon2.ability)
            assert_equal(pokemon1.ball, pokemon2.ball)
            assert_equal(pokemon1.personality, pokemon2.personality)

            assert_equal(pokemon1.markings.keys, pokemon2.markings.keys)
            pokemon1.markings.each do |key, value|
                assert_equal(value, pokemon2.markings[key])
            end
            assert_equal(pokemon1.ribbons.keys, pokemon2.ribbons.keys)
            pokemon1.ribbons.each do |key, value|
                assert_equal(value, pokemon2.ribbons[key])
            end
            assert_equal(pokemon1.contest_stats.keys, pokemon2.contest_stats.keys)
            pokemon1.contest_stats.each do |key, value|
                assert_equal(value, pokemon2.contest_stats[key])
            end
        end

        if generation >= 4
            assert_equal(pokemon1.location_met_as_egg, pokemon2.location_met_as_egg)
        end
    end
end
