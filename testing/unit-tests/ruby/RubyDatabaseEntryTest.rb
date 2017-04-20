#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"

require "os"
require "pathname"
require "test/unit"

# For these tests, just make sure the wrapper constant works as intended.
class RubyDatabaseEntryTest < Test::Unit::TestCase
    def assert_float_equal(expected,actual)
        assert_true((expected-actual).abs < 0.00001)
    end

    def test_item_entry
        # Make sure trying to create an invalid entry results in an error.
        assert_raise ArgumentError do
            PKMN::Database::ItemEntry.new("Not an item", "SoulSilver")
        end
        assert_raise ArgumentError do
            PKMN::Database::ItemEntry.new("Fast Ball", "Not a game")
        end
        assert_raise ArgumentError do
            PKMN::Database::ItemEntry.new("Fast Ball", "Red")
        end

        entry = PKMN::Database::ItemEntry.new("Fast Ball", "SoulSilver")

        assert_equal("Fast Ball", entry.name)
        assert_equal("SoulSilver", entry.game)
        assert_equal("Apricorn balls", entry.category)
        assert_equal("Poké Balls", entry.pocket)

        # Just make sure it works.
        entry.description

        assert_equal(300, entry.cost)
        assert_true(entry.is_holdable?)
        assert_equal(0, entry.fling_power)
        assert_equal("None", entry.fling_effect)

        entry_same      = PKMN::Database::ItemEntry.new("Fast Ball", "SoulSilver")
        entry_different = PKMN::Database::ItemEntry.new("Great Ball", "SoulSilver")
        assert_equal(entry_same, entry)
        assert_false(entry_different == entry)
    end

    def test_move_entry
        # Make sure trying to create an invalid entry results in an error.
        assert_raise ArgumentError do
            PKMN::Database::MoveEntry.new("Not an item", "Silver")
        end
        assert_raise ArgumentError do
            PKMN::Database::MoveEntry.new("Octazooka", "Not a game")
        end
        assert_raise ArgumentError do
            PKMN::Database::MoveEntry.new("Octazooka", "Red")
        end

        entry = PKMN::Database::MoveEntry.new("Octazooka", "Silver")

        assert_equal("Octazooka", entry.name)
        assert_equal("Silver", entry.game)
        assert_equal("Water", entry.type)

        # Just make sure it works.
        entry.description

        assert_equal("Selected Pokémon", entry.target)
        assert_equal("Special", entry.damage_class)
        assert_equal(65, entry.base_power)

        pps = [10,12,14,16]
        (0..3).each do |i|
            assert_equal(pps[i], entry.get_pp(i))
        end

        assert_float_equal(0.85, entry.accuracy)
        assert_equal(0, entry.priority)

        # Just make sure it works
        entry.effect

        assert_equal("None", entry.contest_type)
        assert_equal("None", entry.contest_effect)
        assert_equal("None", entry.super_contest_effect)

        entry_same      = PKMN::Database::MoveEntry.new("Octazooka", "Silver")
        entry_different = PKMN::Database::MoveEntry.new("Hydro Pump", "Silver")
        assert_equal(entry_same, entry)
        assert_false(entry_different == entry)
    end

    def test_pokemon_entry
        # Make sure trying to create an invalid entry results in an error.
        assert_raise ArgumentError do
            PKMN::Database::PokemonEntry.new("Invalid", "Black 2", "Sunny")
        end
        assert_raise ArgumentError do
            PKMN::Database::PokemonEntry.new("Castform", "Not a game", "Sunny")
        end
        assert_raise ArgumentError do
            PKMN::Database::PokemonEntry.new("Castform", "Black 2", "Not a form")
        end

        entry = PKMN::Database::PokemonEntry.new("Stunfisk", "Black 2", "")

        assert_equal("Stunfisk", entry.name)
        assert_equal("Black 2", entry.game)
        assert_equal("Trap", entry.species)
        assert_equal("Standard", entry.form)
        assert(entry.pokedex_entry.length > 0)
        assert_float_equal(0.7, entry.height)
        assert_float_equal(11.0, entry.weight)
        assert_float_equal(0.5, entry.chance_male)
        assert_float_equal(0.5, entry.chance_female)
        assert(!entry.has_gender_differences?)
        assert_equal(70, entry.base_friendship)

        assert_equal("Ground", entry.types.first)
        assert_equal("Electric", entry.types.second)

        assert_equal("Static", entry.abilities.first)
        assert_equal("Limber", entry.abilities.second)

        assert_equal("Sand Veil", entry.hidden_ability)

        assert_equal("Water 1", entry.egg_groups.first)
        assert_equal("Amorphous", entry.egg_groups.second)

        assert_equal(109, entry.base_stats["HP"])
        assert_equal(66,  entry.base_stats["Attack"])
        assert_equal(84,  entry.base_stats["Defense"])
        assert_equal(32,  entry.base_stats["Speed"])
        assert_equal(81,  entry.base_stats["Special Attack"])
        assert_equal(99,  entry.base_stats["Special Defense"])

        assert_equal(2, entry.EV_yields["HP"])
        assert_equal(0, entry.EV_yields["Attack"])
        assert_equal(0, entry.EV_yields["Defense"])
        assert_equal(0, entry.EV_yields["Speed"])
        assert_equal(0, entry.EV_yields["Special Attack"])
        assert_equal(0, entry.EV_yields["Special Defense"])

        assert_equal(165, entry.experience_yield)
        assert_equal(125000, entry.get_experience_at_level(50))
        assert_equal(58, entry.get_level_at_experience(200000))
        assert(entry.levelup_moves.size > 0)
        assert(entry.tm_hm_moves.size > 0)
        assert(entry.egg_moves.size > 0)
        assert(entry.tutor_moves.size > 0)
        assert_equal(1, entry.forms.size)
        assert_equal(0, entry.evolutions.size)

        assert(Pathname.new(entry.get_icon_filepath(false)).exist?)
        assert(Pathname.new(entry.get_icon_filepath(true)).exist?)
        assert(Pathname.new(entry.get_sprite_filepath(false,false)).exist?)
        assert(Pathname.new(entry.get_sprite_filepath(false,true)).exist?)
        assert(Pathname.new(entry.get_sprite_filepath(true,false)).exist?)
        assert(Pathname.new(entry.get_sprite_filepath(true,true)).exist?)

        # Use different Pokémon for testing (in)equality
        entry_first = PKMN::Database::PokemonEntry.new("Pikachu", "Omega Ruby", "Standard")
        entry_same = PKMN::Database::PokemonEntry.new("Pikachu", "Omega Ruby", "Standard")
        entry_different = PKMN::Database::PokemonEntry.new("Pichu", "Omega Ruby", "Standard")

        assert_equal(entry_same, entry_first)
        assert_not_equal(entry_different, entry_first)
    end
end
