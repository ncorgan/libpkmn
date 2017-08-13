#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require_relative "PokemonTest"
require_relative "PokemonTestParams"

require "PKMN"

require "minitest/autorun"
require "os"
require "pathname"

class Gen3PokemonTest < PokemonTest
    def _gen3_test_common(pokemon)
        # Gender and personality are tied, so make sure they affect each other.
        pokemon.gender = "Female"
        assert_operator(pokemon.personality & 0xFF, :<, 0xFF)
        pokemon.gender = "Male"
        assert_equal(0xFF, pokemon.personality & 0xFF)

        pokemon.personality = 0x1234AB00
        assert_equal("Female", pokemon.gender)
        pokemon.personality = 0xCD5678FF
        assert_equal("Male", pokemon.gender)

        # Setting shininess should affect personality.
        pokemon.is_shiny = false
        assert(!pokemon.is_shiny?)
        pokemon.is_shiny = true
        assert(pokemon.is_shiny?)
    end

    def _gba_pokemon_test(game_params)
        pokemon = PKMN::Pokemon.new(game_params[1], game_params[0], "", 30)

        test_params = PokemonTestParams.new
        test_params.valid_ball = "Great Ball"
        test_params.invalid_balls = ["Friend Ball", "Heal Ball"]
        test_params.valid_item = "Razz Berry"
        test_params.invalid_items = ["Berry", "Mach Bike"]
        test_params.expected_original_location = "Fateful encounter"
        test_params.valid_locations = ["Petalburg Woods", "Viridian Forest"]
        test_params.invalid_locations = ["New Bark Town", "Twinleaf Town"]
        test_params.valid_moves = ["Swallow", "Flamethrower", "Return", "Fire Blast"]
        test_params.invalid_moves = ["Shadow Sky", "Roost"]
        test_params.valid_original_games = ["Ruby", "Sapphire", "Emerald", "FireRed",
                                            "LeafGreen", "Colosseum", "XD", "Colosseum/XD"]
        test_params.invalid_original_games = ["Gold", "HeartGold"]

        pokemon_test_common(pokemon, test_params)
        _gen3_test_common(pokemon)
    end

    def _gcn_pokemon_test(game_params)
        pokemon = PKMN::Pokemon.new(game_params[1], game_params[0], "", 30)

        test_params = PokemonTestParams.new
        test_params.valid_ball = "Great Ball"
        test_params.invalid_balls = ["Friend Ball", "Heal Ball"]
        test_params.valid_item = "Razz Berry"
        test_params.invalid_items = ["Berry", "Mach Bike"]
        test_params.expected_original_location = "Distant land"
        test_params.valid_locations = ["Phenac City", "Orre Colosseum"]
        test_params.invalid_locations = ["New Bark Town", "Twinleaf Town"]
        test_params.valid_moves = ["Swallow", "Flamethrower", "Return", "Fire Blast"]
        test_params.invalid_moves = ["Roost", "Flame Burst"]
        test_params.valid_original_games = ["Ruby", "Sapphire", "Emerald", "FireRed",
                                            "LeafGreen", "Colosseum", "XD", "Colosseum/XD"]
        test_params.invalid_original_games = ["Gold", "HeartGold"]

        pokemon_test_common(pokemon, test_params)
        _gen3_test_common(pokemon)
    end

    [["Ruby", "Torchic"],
     ["Sapphire", "Mudkip"],
     ["Emerald", "Treecko"],
     ["FireRed", "Charmander"],
     ["LeafGreen", "Bulbasaur"]].each do |game_params|
        define_method("test_gba_pokemon_#{game_params[0]}") do
            _gba_pokemon_test(game_params)
        end
    end

    [["Colosseum", "Espeon"],
     ["XD", "Umbreon"]].each do |game_params|
        define_method("test_gcn_pokemon_#{game_params[0]}") do
            _gcn_pokemon_test(game_params)
        end
    end
end
