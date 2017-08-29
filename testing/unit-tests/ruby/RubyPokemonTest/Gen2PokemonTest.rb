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

class Gen2PokemonTest < PokemonTest
    def _gen2_pokemon_test(game_params)
        pokemon = PKMN::Pokemon.new(game_params[1], game_params[0], "", 30)

        test_params = PokemonTestParams.new
        test_params.valid_ball = "Great Ball"
        test_params.invalid_balls = ["Great Ball"]
        test_params.valid_item = "Berry"
        test_params.invalid_items = ["Razz Berry", "Bicycle"]
        test_params.expected_original_location = "Special"
        test_params.valid_locations = ["Sprout Tower", "Tohjo Falls"]
        test_params.invalid_locations = ["Littleroot Town", "Petalburg Woods"]
        test_params.valid_moves = ["Slash", "Flamethrower", "Return", "Fire Blast"]
        test_params.invalid_moves = ["Frenzy Plant", "Roost"]
        test_params.valid_original_games = ["Gold"]
        test_params.invalid_original_games = ["Gold"]

        pokemon_test_common(pokemon, test_params)

        # Gender affects IVs, so make sure the abstraction reflects that.
        pokemon.gender = "Male"
        assert_equal(15, pokemon.IVs["Attack"])
        pokemon.gender = "Female"
        assert_operator(pokemon.IVs["Attack"], :<, 15)

        pokemon.IVs["Attack"] = 0
        assert_equal("Female", pokemon.gender)
        pokemon.IVs["Attack"] = 15
        assert_equal("Male", pokemon.gender)

        # Shininess affects IVs, so make sure the abstraction reflects that.
        pokemon.is_shiny = false
        assert(!pokemon.is_shiny?)
        assert_equal(13, pokemon.IVs["Attack"])

        pokemon.is_shiny = true
        assert(pokemon.is_shiny?)
        assert_equal(15, pokemon.IVs["Attack"])
        assert_equal(10, pokemon.IVs["Defense"])
        assert_equal(10, pokemon.IVs["Speed"])
        assert_equal(10, pokemon.IVs["Special"])

        pokemon.IVs["Special"] = 12
        assert(!pokemon.is_shiny?)
    end

    [["Gold", "Cyndaquil"],
     ["Silver", "Totodile"],
     ["Crystal", "Chikorita"]].each do |game_params|
        define_method("test_gen2_pokemon_#{game_params[0]}") do
            _gen2_pokemon_test(game_params)
        end
    end
end
