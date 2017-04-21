#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require_relative "PokemonTest"

class GenderTest < PokemonTest
    def _test_gender(game)
        # Single-gender
        nidorina = PKMN::Pokemon.new("Nidorina", game, "", 50)
        nidorina.gender = "Female"
        assert_equal("Female", nidorina.gender)

        assert_raises ArgumentError do
            nidorina.gender = "Male"
        end
        assert_raises ArgumentError do
            nidorina.gender = "Genderless"
        end

        nidorino = PKMN::Pokemon.new("Nidorino", game, "", 50)
        nidorino.gender = "Male"
        assert_equal("Male", nidorino.gender)

        assert_raises ArgumentError do
            nidorino.gender = "Female"
        end
        assert_raises ArgumentError do
            nidorino.gender = "Genderless"
        end

        magnemite = PKMN::Pokemon.new("Magnemite", game, "", 50)
        magnemite.gender = "Genderless"
        assert_equal("Genderless", magnemite.gender)

        assert_raises ArgumentError do
            magnemite.gender = "Male"
        end
        assert_raises ArgumentError do
            magnemite.gender = "Female"
        end

        mixed_pokemon = ["Charmander", "Growlithe", "Pidgey", "Vulpix"]
        mixed_pokemon.each do |species|
            pokemon = PKMN::Pokemon.new(species, game, "", 50)
            pokemon.gender = "Male"
            assert_equal("Male", pokemon.gender)
            pokemon.gender = "Female"
            assert_equal("Female", pokemon.gender)
            assert_raises ArgumentError do
                pokemon.gender = "Genderless"
            end
        end
    end

    PARAMS = ["Gold", "Silver", "Crystal", "Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen"]

    PARAMS.each do |game|
        define_method("test_gender_#{game}") do
            _test_gender(game)
        end
    end
end
