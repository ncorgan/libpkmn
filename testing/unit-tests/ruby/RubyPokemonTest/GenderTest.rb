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
        nidorina.gender = PKMN::Gender::FEMALE
        assert_equal(PKMN::Gender::FEMALE, nidorina.gender)

        assert_raises ArgumentError do
            nidorina.gender = PKMN::Gender::MALE
        end
        assert_raises ArgumentError do
            nidorina.gender = PKMN::Gender::GENDERLESS
        end

        nidorino = PKMN::Pokemon.new("Nidorino", game, "", 50)
        nidorino.gender = PKMN::Gender::MALE
        assert_equal(PKMN::Gender::MALE, nidorino.gender)

        assert_raises ArgumentError do
            nidorino.gender = PKMN::Gender::FEMALE
        end
        assert_raises ArgumentError do
            nidorino.gender = PKMN::Gender::GENDERLESS
        end

        magnemite = PKMN::Pokemon.new("Magnemite", game, "", 50)
        magnemite.gender = PKMN::Gender::GENDERLESS
        assert_equal(PKMN::Gender::GENDERLESS, magnemite.gender)

        assert_raises ArgumentError do
            magnemite.gender = PKMN::Gender::MALE
        end
        assert_raises ArgumentError do
            magnemite.gender = PKMN::Gender::FEMALE
        end

        mixed_pokemon = ["Charmander", "Growlithe", "Pidgey", "Vulpix"]
        mixed_pokemon.each do |species|
            pokemon = PKMN::Pokemon.new(species, game, "", 50)
            pokemon.gender = PKMN::Gender::MALE
            assert_equal(PKMN::Gender::MALE, pokemon.gender)
            pokemon.gender = PKMN::Gender::FEMALE
            assert_equal(PKMN::Gender::FEMALE, pokemon.gender)
            assert_raises ArgumentError do
                pokemon.gender = PKMN::Gender::GENDERLESS
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
