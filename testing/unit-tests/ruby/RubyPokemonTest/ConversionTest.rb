#!/usr/bin/ruby
#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require_relative "PokemonTest"

require "pathname"

class ConversionTest < PokemonTest

    def _test_conversion(species, form, origin_game, dest_game)
        origin_generation = @@GAME_GENERATIONS[origin_game]
        dest_generation = @@GAME_GENERATIONS[dest_game]
        min_generation = [origin_generation, dest_generation].min
        game_for_lists = (min_generation == origin_generation) ? origin_game : dest_game

        items = PKMN::Database::get_item_list(game_for_lists)
        moves = PKMN::Database::get_move_list(game_for_lists)

        first_pokemon = get_specific_random_pokemon(
                            origin_game,
                            species,
                            form,
                            moves,
                            items
                        )

        # Convert to the second game and compare.
        second_pokemon = first_pokemon.to_game(dest_game)

        assert_equal(first_pokemon.species, second_pokemon.species)
        assert_equal(dest_game, second_pokemon.game)
        assert_equal(first_pokemon.form, second_pokemon.form)
        assert_equal(first_pokemon.nickname, second_pokemon.nickname)
        assert_equal(first_pokemon.original_trainer_name, second_pokemon.original_trainer_name)
        assert_equal(first_pokemon.original_trainer_id, second_pokemon.original_trainer_id)
        assert_equal(first_pokemon.original_trainer_public_id, second_pokemon.original_trainer_public_id)
        assert_equal(first_pokemon.experience, second_pokemon.experience)
        assert_equal(first_pokemon.level, second_pokemon.level)

        if origin_generation == dest_generation
            assert_equal(first_pokemon.EVs.keys, second_pokemon.EVs.keys)
            first_pokemon.EVs.each do |key, value|
                assert_equal(value, second_pokemon.EVs[key])
            end

            assert_equal(first_pokemon.IVs.keys, second_pokemon.IVs.keys)
            first_pokemon.IVs.each do |key, value|
                assert_equal(value, second_pokemon.IVs[key])
            end
        end

        (0..3).each do |move_index|
            assert_equal(
                first_pokemon.moves[move_index].move,
                second_pokemon.moves[move_index].move
            )
            assert_equal(
                first_pokemon.moves[move_index].pp,
                second_pokemon.moves[move_index].pp
            )
        end

        if min_generation >= 2
            assert_equal(first_pokemon.original_trainer_gender, second_pokemon.original_trainer_gender)
            assert_equal(first_pokemon.gender, second_pokemon.gender)
            assert_equal(first_pokemon.is_shiny, second_pokemon.is_shiny)
            assert_equal(first_pokemon.held_item, second_pokemon.held_item)
            assert_equal(first_pokemon.current_trainer_friendship, second_pokemon.current_trainer_friendship)
            assert_equal(first_pokemon.level, second_pokemon.level_met)
        end

        if min_generation >= 3
            assert_equal(first_pokemon.original_trainer_secret_id, second_pokemon.original_trainer_secret_id)
            assert_equal(first_pokemon.ability, second_pokemon.ability)
            assert_equal(first_pokemon.ball, second_pokemon.ball)
            assert_equal(first_pokemon.original_game, second_pokemon.original_game)
            assert_equal(first_pokemon.personality, second_pokemon.personality)

            if origin_generation == dest_generation
                assert_equal(first_pokemon.markings.keys, second_pokemon.markings.keys)
                first_pokemon.markings.each do |key, value|
                    assert_equal(value, second_pokemon.markings[key])
                end

                assert_equal(first_pokemon.ribbons.keys, second_pokemon.ribbons.keys)
                first_pokemon.ribbons.each do |key, value|
                    assert_equal(value, second_pokemon.ribbons[key])
                end

                assert_equal(first_pokemon.contest_stats.keys, second_pokemon.contest_stats.keys)
                first_pokemon.contest_stats.each do |key, value|
                    assert_equal(value, second_pokemon.contest_stats[key])
                end
            end
        end
    end

    CONVERSION_TEST_PARAMS = [
        # Generation I -> Generation I
        ["Bulbasaur", "", "Red", "Yellow"],

        # Generation I -> Generation II
        ["Squirtle", "", "Blue", "Gold"],

        # Generation II -> Generation II
        ["Cyndaquil", "", "Gold", "Crystal"],
        ["Totodile", "", "Crystal", "Gold"],

        # Generation II -> Generation I
        ["Charmander", "", "Silver", "Blue"],

        # GBA -> GBA
        ["Torchic", "", "Ruby", "Sapphire"],
        ["Mudkip", "", "Ruby", "Emerald"],
        ["Treecko", "", "Ruby", "FireRed"],
        ["Torchic", "", "Emerald", "Sapphire"],
        ["Mudkip", "", "Emerald", "Emerald"],
        ["Treecko", "", "Emerald", "FireRed"],
        ["Charmander", "", "FireRed", "Sapphire"],
        ["Squirtle", "", "FireRed", "Emerald"],
        ["Bulbasaur", "", "FireRed", "FireRed"],

        # GBA -> GCN
        ["Eevee", "", "Ruby", "Colosseum"],
        ["Espeon", "", "Emerald", "Colosseum"],
        ["Umbreon", "", "FireRed", "Colosseum"],
        ["Eevee", "", "Ruby", "XD"],
        ["Espeon", "", "Emerald", "XD"],
        ["Umbreon", "", "FireRed", "XD"],

        # GCN -> GBA
        ["Eevee", "", "Colosseum", "Sapphire"],
        ["Espeon", "", "Colosseum", "Emerald"],
        ["Umbreon", "", "Colosseum", "LeafGreen"],
        ["Eevee", "", "XD", "Sapphire"],
        ["Espeon", "", "XD", "Emerald"],
        ["Umbreon", "", "XD", "LeafGreen"],

        # GCN -> GCN
        ["Vaporeon", "", "Colosseum", "Colosseum"],
        ["Jolteon", "", "Colosseum", "XD"],
        ["Vaporeon", "", "XD", "XD"],
        ["Jolteon", "", "XD", "Colosseum"]
    ]

    CONVERSION_TEST_PARAMS.each do |params|
        species = params[0]
        form = params[1]
        origin_game = params[2]
        dest_game = params[3]

        define_method("test_conversion_from_#{origin_game}_to_#{dest_game}") do
            _test_conversion(species, form, origin_game, dest_game)
        end
    end
end
