#!/usr/bin/ruby
#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"

require "minitest/autorun"

class DaycareTest < MiniTest::Test

    def _test_empty_daycare(daycare)
        daycare.levelup_pokemon.each do |pokemon|
            assert_equal("None", pokemon.species)
        end

        # Test invalid indices.
        assert_raises IndexError do
            pokemon = daycare.levelup_pokemon[-1]
        end
        assert_raises IndexError do
            pokemon = daycare.levelup_pokemon[daycare.levelup_pokemon.length]
        end

        if daycare.can_breed_pokemon
        then
            daycare.breeding_pokemon.each do |pokemon|
                assert_equal("None", pokemon.species)
            end

            # Test invalid indices.
            assert_raises IndexError do
                pokemon = daycare.breeding_pokemon[-1]
            end
            assert_raises IndexError do
                pokemon = daycare.breeding_pokemon[daycare.breeding_pokemon.length]
            end
        end
    end

    def _test_setting_pokemon(daycare)
        daycare.levelup_pokemon[0] = PKMN::Pokemon.new(
                                         "Venusaur",
                                         daycare.game,
                                         "",
                                         50
                                     )
        assert_equal("Venusaur", daycare.levelup_pokemon[0].species)
        assert_equal(daycare.game, daycare.levelup_pokemon[0].game)

        if daycare.levelup_pokemon.length == 2
        then
            daycare.levelup_pokemon[1] = PKMN::Pokemon.new(
                                             "Charizard",
                                             daycare.game,
                                             "",
                                             50
                                         )
            assert_equal("Charizard", daycare.levelup_pokemon[1].species)
            assert_equal(daycare.game, daycare.levelup_pokemon[1].game)
        end

        # TODO: validate genders
        if daycare.can_breed_pokemon
        then
            daycare.breeding_pokemon[0] = PKMN::Pokemon.new(
                                             "Blastoise",
                                             daycare.game,
                                             "",
                                             50
                                         )
            daycare.breeding_pokemon[0].gender = "Female"

            assert_equal(daycare.game, daycare.breeding_pokemon[0].game)
            assert_equal("Blastoise", daycare.breeding_pokemon[0].species)
            assert_equal("Female", daycare.breeding_pokemon[0].gender)

            daycare.breeding_pokemon[1] = PKMN::Pokemon.new(
                                             "Marowak",
                                             daycare.game,
                                             "",
                                             50
                                         )
            daycare.breeding_pokemon[1].gender = "Male"

            assert_equal(daycare.game, daycare.breeding_pokemon[1].game)
            assert_equal("Marowak", daycare.breeding_pokemon[1].species)
            assert_equal("Male", daycare.breeding_pokemon[1].gender)
        end
    end

    def _daycare_test(game, expected_can_breed)
        daycare = PKMN::Daycare.new(game)
        assert_equal(game, daycare.game)
        assert_equal(expected_can_breed, daycare.can_breed_pokemon)

        _test_empty_daycare(daycare)
        _test_setting_pokemon(daycare)
    end

    PARAMS = [
        ["Red", false],
        ["Blue", false],
        ["Yellow", false],

        ["Gold", true],
        ["Silver", true],
        ["Crystal", true],

        ["Colosseum", false],
        ["XD", false]
    ]

     PARAMS.each do |game, expected_can_breed|
        define_method("test_daycare_#{game}") do
            _daycare_test(game, expected_can_breed)
        end
    end
end
