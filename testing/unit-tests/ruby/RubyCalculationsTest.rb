#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"

require "test/unit"

class RubyListsTest < Test::Unit::TestCase
    def test_gen2_unown_forms
        # Make sure expected errors are raised.
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(-1, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(16, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(0, -1, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(0, 16, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(0, 0, -1, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(0, 0, 16, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(0, 0, 0, -1)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(0, 0, 0, 16)
        end

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Unown.27s_letter
        #
        assert_equal("G", PKMN::Calculations::gen2_unown_form(10, 9, 1, 14))
        assert_equal("S", PKMN::Calculations::gen2_unown_form(5, 15, 10, 5))
    end

    def test_gen3_unown_forms
        # Make sure SWIG+Ruby catches values outside the uint32_t bounds.
        assert_raise RangeError do
            PKMN::Calculations::gen3_unown_form(-1)
        end
        assert_raise RangeError do
            PKMN::Calculations::gen3_unown_form(0xFFFFFFFF+1)
        end

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://www.smogon.com/ingame/rng/pid_iv_creation#unown_shape
        #
        assert_equal("B", PKMN::Calculations::gen3_unown_form(0x4C07DE71))
    end

    def test_wurmple_becomes_silcoon
        # Make sure SWIG+Ruby catches values outside the uint32_t bounds.
        assert_raise RangeError do
            PKMN::Calculations::wurmple_becomes_silcoon(-1, false)
        end
        assert_raise RangeError do
            PKMN::Calculations::wurmple_becomes_silcoon(0xFFFFFFFF+1, false)
        end

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://www.smogon.com/ingame/rng/pid_iv_creation#silcoon_cascoon
        #
        assert(PKMN::Calculations::wurmple_becomes_silcoon(0x5CF4091C, true))
        assert_equal(false, PKMN::Calculations::wurmple_becomes_silcoon(0x091C5CF4, true))
        assert(PKMN::Calculations::wurmple_becomes_silcoon(0x091C5CF4, false))
        assert_equal(false, PKMN::Calculations::wurmple_becomes_silcoon(0x5CF4091C, false))
    end

    def test_gen2_gender
        # Make sure expected errors are raised.
        assert_raise ArgumentError do
            PKMN::Calculations::gen2_pokemon_gender("Not a species", 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_pokemon_gender("Bulbasaur", -1)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_pokemon_gender("Bulbasaur", 16)
        end

        # Make sure known good inputs result in expected results.

        # All male
        assert_equal("Male", PKMN::Calculations::gen2_pokemon_gender("Nidorino", 0))
        assert_equal("Male", PKMN::Calculations::gen2_pokemon_gender("Nidorino", 15))

        # 25% male, 75% female
        assert_equal("Female", PKMN::Calculations::gen2_pokemon_gender("Vulpix", 11))
        assert_equal("Male", PKMN::Calculations::gen2_pokemon_gender("Vulpix", 12))

        # All female
        assert_equal("Female", PKMN::Calculations::gen2_pokemon_gender("Nidorina", 0))
        assert_equal("Female", PKMN::Calculations::gen2_pokemon_gender("Nidorina", 15))

        # Genderless
        assert_equal("Genderless", PKMN::Calculations::gen2_pokemon_gender("Magnemite", 0))
        assert_equal("Genderless", PKMN::Calculations::gen2_pokemon_gender("Magnemite", 15))
    end

    def test_modern_gender
        # Make sure expected errors are raised.
        assert_raise ArgumentError do
            PKMN::Calculations::modern_pokemon_gender("Not a species", 0)
        end

        # Make sure SWIG+Ruby catches values outside the uint32_t bounds.
        assert_raise RangeError do
            PKMN::Calculations::modern_pokemon_gender("Bulbasaur", -1)
        end
        assert_raise RangeError do
            PKMN::Calculations::modern_pokemon_gender("Bulbasaur", 0xFFFFFFFF+1)
        end

        # All male
        assert_equal("Male", PKMN::Calculations::modern_pokemon_gender("Nidorino", 0))
        assert_equal("Male", PKMN::Calculations::modern_pokemon_gender("Nidorino", 0xFFFFFFFF))

        # 25% male, 75% female
        assert_equal("Female", PKMN::Calculations::modern_pokemon_gender("Vulpix", 190))
        assert_equal("Male", PKMN::Calculations::modern_pokemon_gender("Vulpix", 191))

        # All female
        assert_equal("Female", PKMN::Calculations::modern_pokemon_gender("Nidorina", 0))
        assert_equal("Female", PKMN::Calculations::modern_pokemon_gender("Nidorina", 0xFFFFFFFF))

        # Genderless
        assert_equal("Genderless", PKMN::Calculations::modern_pokemon_gender("Magnemite", 0))
        assert_equal("Genderless", PKMN::Calculations::modern_pokemon_gender("Magnemite", 0xFFFFFFFF))
    end

    def test_gen2_hidden_power
        # Make sure expected errors are raised.
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(-1, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(16, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(0, -1, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(0, 16, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(0, 0, -1, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(0, 0, 16, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(0, 0, 0, -1)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(0, 0, 0, 16)
        end

        #
        # Make sure known good inputs result in expected results, and test (in)equality operators.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_II
        #
        expected_hidden_power = PKMN::Calculations::HiddenPower.new("Dark", 69)
        hidden_power_different_type = PKMN::Calculations::HiddenPower.new("Normal", 69)
        hidden_power_different_base_power = PKMN::Calculations::HiddenPower.new("Dark", 50)

        hidden_power = PKMN::Calculations::gen2_hidden_power(15, 15, 15, 14)
        assert_equal(expected_hidden_power, hidden_power)
        assert_not_equal(hidden_power_different_type, hidden_power)
        assert_not_equal(hidden_power_different_base_power, hidden_power)
    end

    def test_modern_hidden_power
        # Make sure expected errors are raised.
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(-1, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(32, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, -1, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 32, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, -1, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, 32, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, 0, -1, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, 0, 32, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, 0, 0, -1, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, 0, 0, 32, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, 0, 0, 0, -1)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, 0, 0, 0, 32)
        end

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_III_to_VI
        #
        expected_hidden_power = PKMN::Calculations::HiddenPower.new("Grass", 70)
        hidden_power_different_type = PKMN::Calculations::HiddenPower.new("Steel", 70)
        hidden_power_different_base_power = PKMN::Calculations::HiddenPower.new("Grass", 10)

        hidden_power = PKMN::Calculations::modern_hidden_power(30, 31, 31, 31, 30, 31)
        assert_equal(expected_hidden_power, hidden_power)
        assert_not_equal(hidden_power_different_type, hidden_power)
        assert_not_equal(hidden_power_different_base_power, hidden_power)
    end

    def test_nature
        natures = [
            "Hardy", "Lonely", "Brave", "Adamant", "Naughty",
            "Bold", "Docile", "Relaxed", "Impish", "Lax",
            "Timid", "Hasty", "Serious", "Jolly", "Naive",
            "Modest", "Mild", "Quiet", "Bashful", "Rash",
            "Calm", "Gentle", "Sassy", "Careful", "Quirky",
        ]

        # Make sure SWIG+Ruby catches values outside the uint32_t bounds.
        assert_raise RangeError do
            PKMN::Calculations::nature(-1)
        end
        assert_raise RangeError do
            PKMN::Calculations::nature(0xFFFFFFFF+1)
        end

        (0..natures.length-1).each do |i|
            assert_equal(
                natures[i],
                PKMN::Calculations::nature((rand(0...50000) * 1000) + i)
            )
        end
    end

    #
    # Given the amount of time the C++ test takes, this will just verify
    # the API wrapper.
    #
    def test_personality
        # Test invalid ability.
        assert_raise ArgumentError do
            PKMN::Calculations::generate_personality(
                "Charmander",
                PKMN::Pokemon::DEFAULT_TRAINER_ID,
                true,
                "Torrent",
                "Male",
                "Quiet"
            )
        end

        # Test invalid gender.
        assert_raise ArgumentError do
            PKMN::Calculations::generate_personality(
                "Charmander",
                PKMN::Pokemon::DEFAULT_TRAINER_ID,
                true,
                "Blaze",
                "Not a gender",
                "Quiet"
            )
        end

        # Test invalid nature.
        assert_raise ArgumentError do
            PKMN::Calculations::generate_personality(
                "Charmander",
                PKMN::Pokemon::DEFAULT_TRAINER_ID,
                true,
                "Blaze",
                "Male",
                "Not a nature"
            )
        end

        # Make sure SWIG+Ruby catches values outside the uint32_t bounds.
        assert_raise RangeError do
            PKMN::Calculations::generate_personality(
                "Charmander",
                -1,
                true,
                "Blaze",
                "Male",
                "Quiet"
            )
        end
        assert_raise RangeError do
            PKMN::Calculations::generate_personality(
                "Charmander",
                0xFFFFFFFF+1,
                true,
                "Blaze",
                "Male",
                "Quiet"
            )
        end

        # Test and validate a valid call.
        personality = PKMN::Calculations::generate_personality(
                          "Charmander",
                          PKMN::Pokemon::DEFAULT_TRAINER_ID,
                          true,
                          "Blaze",
                          "Male",
                          "Quiet"
                      )
        assert(
            PKMN::Calculations::modern_shiny(
                personality, PKMN::Pokemon::DEFAULT_TRAINER_ID
            )
        )
        assert_equal(
            "Male",
            PKMN::Calculations::modern_pokemon_gender(
                "Charmander",
                personality
            )
        )
    end

    def test_gen2_shiny
        # Make sure expected errors are raised.
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(-1, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(16, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(0, -1, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(0, 16, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(0, 0, -1, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(0, 0, 16, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(0, 0, 0, -1)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(0, 0, 0, 16)
        end
        
        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Shininess
        #
        assert(PKMN::Calculations::gen2_shiny(7, 10, 10, 10))
        assert_equal(false, PKMN::Calculations::gen2_shiny(6, 15, 7, 15))
    end

    def test_modern_shiny
        # Make sure SWIG+Ruby catches values outside the uint32_t bounds.
        assert_raise RangeError do
            PKMN::Calculations::modern_shiny(-1, 0)
        end
        assert_raise RangeError do
            PKMN::Calculations::modern_shiny(0xFFFFFFFF+1, 0)
        end
        assert_raise RangeError do
            PKMN::Calculations::modern_shiny(0, -1)
        end
        assert_raise RangeError do
            PKMN::Calculations::modern_shiny(0, 0xFFFFFFFF+1)
        end
        
        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Personality_value#Shininess
        #         http://www.smogon.com/ingame/rng/pid_iv_creation#how_shiny
        #
        assert(PKMN::Calculations::modern_shiny(2814471828, 2545049318))
        assert(PKMN::Calculations::modern_shiny(0xB58F0B2A, 398174488))
    end

    def test_pokemon_size
        # Make sure SWIG+Ruby catches values outside the uint32_t bounds.
        assert_raise RangeError do
            PKMN::Calculations::pokemon_size(
                "Magikarp", -1, 0, 0, 0, 0, 0, 0
            )
        end
        assert_raise RangeError do
            PKMN::Calculations::pokemon_size(
                "Magikarp", 0xFFFFFFFF+1, 0, 0, 0, 0, 0, 0
            )
        end

        # Test input validation.
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                "Magikarp", 0, -1, 0, 0, 0, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                "Magikarp", 0, 32, 0, 0, 0, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                "Magikarp", 0, 0, -1, 0, 0, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                "Magikarp", 0, 0, 32, 0, 0, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                "Magikarp", 0, 0, 0, -1, 0, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                "Magikarp", 0, 0, 0, 32, 0, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                "Magikarp", 0, 0, 0, 0, -1, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                "Magikarp", 0, 0, 0, 0, 32, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                "Magikarp", 0, 0, 0, 0, 0, -1, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                "Magikarp", 0, 0, 0, 0, 0, 32, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                "Magikarp", 0, 0, 0, 0, 0, 0, -1
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                "Magikarp", 0, 0, 0, 0, 0, 0, 32
            )
        end

        # There are no known good calculations, so just check for reasonable values
        # for each relevant Pokemon.
        pokemon_with_size_checks = [
            PKMN::Database::PokemonEntry.new("Barboach", "Ruby", ""),
            PKMN::Database::PokemonEntry.new("Shroomish", "Ruby", ""),
            PKMN::Database::PokemonEntry.new("Seedot", "Emerald", ""),
            PKMN::Database::PokemonEntry.new("Lotad", "Emerald", ""),
            PKMN::Database::PokemonEntry.new("Magikarp", "FireRed", ""),
            PKMN::Database::PokemonEntry.new("Heracross", "FireRed", "")
        ]

        (0..pokemon_with_size_checks.length-1).each do |i|
            height = pokemon_with_size_checks[i].height
            species = pokemon_with_size_checks[i].name

            (0..9).each do
                size = PKMN::Calculations::pokemon_size(
                           species,
                           rand(0...0xFFFFFF),
                           rand(0...31),
                           rand(0...31),
                           rand(0...31),
                           rand(0...31),
                           rand(0...31),
                           rand(0...31)
                       )
                assert((size-height).abs <= height)
            end
        end
    end

    def test_spinda_coords
        # Check (in)equality operators.
        spinda_coords1 = PKMN::Calculations::SpindaCoords.new(123, 456)
        spinda_coords2 = PKMN::Calculations::SpindaCoords.new(123, 456)
        spinda_coords3 = PKMN::Calculations::SpindaCoords.new(456, 123)

        assert_equal(spinda_coords1, spinda_coords2)
        assert_not_equal(spinda_coords1, spinda_coords3)
    end

    def test_spinda_spots
        # Check comparisons.
        spots1 = PKMN::Calculations::SpindaSpots.new(
                     PKMN::Calculations::SpindaCoords.new(7, 5),
                     PKMN::Calculations::SpindaCoords.new(10, 0),
                     PKMN::Calculations::SpindaCoords.new(1, 4),
                     PKMN::Calculations::SpindaCoords.new(2, 15)
                 )
        spots2 = PKMN::Calculations::SpindaSpots.new(
                     PKMN::Calculations::SpindaCoords.new(7, 5),
                     PKMN::Calculations::SpindaCoords.new(10, 0),
                     PKMN::Calculations::SpindaCoords.new(1, 4),
                     PKMN::Calculations::SpindaCoords.new(2, 15)
                 )
        spots3 = PKMN::Calculations::SpindaSpots.new(
                     PKMN::Calculations::SpindaCoords.new(7, 5),
                     PKMN::Calculations::SpindaCoords.new(10, 0),
                     PKMN::Calculations::SpindaCoords.new(2, 15),
                     PKMN::Calculations::SpindaCoords.new(1, 4)
                 )
        assert_equal(spots1, spots2)
        assert_not_equal(spots1, spots3)

        #
        # Make sure known good inputs result in expected results, and test comparisons.
        #
        # Source: https://github.com/magical/spinda
        #
        spots = PKMN::Calculations::spinda_spot_offset(4064348759)
        assert_equal(spots1, spots)
    end

    def assert_ints_almost_equal(a,b)
        return (a == b or (a-1) == b or (a+1) == b)
    end

    def test_gb_stats
        # Make sure expected errors are raised.
        assert_raise ArgumentError do
            PKMN::Calculations::get_gb_stat("Not a stat", 1, 1, 1, 1)
        end
        assert_raise IndexError do
            PKMN::Calculations::get_gb_stat("Attack", 1, 1, 123456, 1)
        end
        assert_raise IndexError do
            PKMN::Calculations::get_gb_stat("Attack", 1, 1, 1, 12345)
        end

        #
        # Test with known good inputs.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generations_I_and_II
        #
        stat = PKMN::Calculations::get_gb_stat("HP", 81, 35, 22850, 7)
        assert_ints_almost_equal(stat, 189)
        stat = PKMN::Calculations::get_gb_stat("Attack", 81, 55, 23140, 8)
        assert_ints_almost_equal(stat, 137)
    end

    def test_modern_stats
        # Make sure expected errors are raised.
        assert_raise ArgumentError do
            PKMN::Calculations::get_modern_stat("Not a stat", 1, 1.0, 1, 1, 1)
        end
        assert_raise ArgumentError do
            PKMN::Calculations::get_modern_stat("Special", 1, 1.0, 1, 1, 1)
        end
        assert_raise ArgumentError do
            PKMN::Calculations::get_modern_stat("Attack", 1, 0.666, 1, 1, 1)
        end
        assert_raise IndexError do
            PKMN::Calculations::get_modern_stat("Attack", 1, 1.0, 1, 12345, 1)
        end
        assert_raise IndexError do
            PKMN::Calculations::get_modern_stat("Attack", 1, 1.0, 1, 1, 12345)
        end

        #
        # Test with known good inputs.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generation_III_onward
        #
        stat = PKMN::Calculations::get_modern_stat("HP", 78, 1.0, 108, 74, 24)
        assert_ints_almost_equal(stat, 289)
        stat = PKMN::Calculations::get_modern_stat("Attack", 78, 1.1, 130, 195, 12)
        assert_ints_almost_equal(stat, 280)
    end
end
