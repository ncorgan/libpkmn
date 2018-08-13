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
            assert_equal(PKMN::Species::NONE, pokemon.species)
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
                assert_equal(PKMN::Species::NONE, pokemon.species)
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
                                         PKMN::Species::VENUSAUR,
                                         daycare.game,
                                         "",
                                         50
                                     )
        assert_equal(
            PKMN::Species::VENUSAUR,
            daycare.levelup_pokemon[0].species
        )
        assert_equal(daycare.game, daycare.levelup_pokemon[0].game)

        if daycare.levelup_pokemon.length == 2
        then
            daycare.levelup_pokemon[1] = PKMN::Pokemon.new(
                                             PKMN::Species::CHARIZARD,
                                             daycare.game,
                                             "",
                                             50
                                         )
            assert_equal(
                PKMN::Species::CHARIZARD,
                daycare.levelup_pokemon[1].species
            )
            assert_equal(daycare.game, daycare.levelup_pokemon[1].game)
        end

        # TODO: validate genders
        if daycare.can_breed_pokemon
        then
            daycare.breeding_pokemon[0] = PKMN::Pokemon.new(
                                             PKMN::Species::BLASTOISE,
                                             daycare.game,
                                             "",
                                             50
                                         )
            daycare.breeding_pokemon[0].gender = PKMN::Gender::FEMALE

            assert_equal(daycare.game, daycare.breeding_pokemon[0].game)
            assert_equal(
                PKMN::Species::BLASTOISE,
                daycare.breeding_pokemon[0].species
            )
            assert_equal(
                PKMN::Gender::FEMALE,
                daycare.breeding_pokemon[0].gender
            )

            daycare.breeding_pokemon[1] = PKMN::Pokemon.new(
                                             PKMN::Species::MAROWAK,
                                             daycare.game,
                                             "",
                                             50
                                         )
            daycare.breeding_pokemon[1].gender = PKMN::Gender::MALE

            assert_equal(daycare.game, daycare.breeding_pokemon[1].game)
            assert_equal(
                PKMN::Species::MAROWAK,
                daycare.breeding_pokemon[1].species
            )
            assert_equal(
                PKMN::Gender::MALE,
                daycare.breeding_pokemon[1].gender
            )
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
            _daycare_test(
                PKMN::string_to_game(game),
                expected_can_breed
            )
        end
    end
end
