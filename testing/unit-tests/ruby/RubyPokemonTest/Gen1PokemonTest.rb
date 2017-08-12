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

class Gen1PokemonTest < PokemonTest
    def _gen1_pokemon_test(game_params)
        pokemon = PKMN::Pokemon.new(game_params[1], game_params[0], "", 30)

        test_params = PokemonTestParams.new
        test_params.valid_ball = "Great Ball"
        test_params.invalid_balls = ["Great Ball"]
        test_params.valid_item = "Potion"
        test_params.invalid_items = ["Potion"]
        test_params.expected_original_location = "Special"
        test_params.valid_locations = ["Route 1"]
        test_params.invalid_locations = ["Route 1"]
        test_params.valid_moves = ["Slash", "Flamethrower", "Tail Whip", "Fire Blast"]
        test_params.invalid_moves = ["Return"]
        test_params.valid_original_games = ["Red"]
        test_params.invalid_original_games = ["Red"]

        pokemon_test_common(pokemon, test_params)
    end

    [["Red", "Charmander"],
     ["Blue", "Squirtle"],
     ["Yellow", "Bulbasaur"]].each do |game_params|
        define_method("test_gen1_pokemon_#{game_params[0]}") do
            _gen1_pokemon_test(game_params)
        end
    end
end
