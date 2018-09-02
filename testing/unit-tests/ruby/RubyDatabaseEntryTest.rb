#!/usr/bin/ruby
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
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
            PKMN::Database::ItemEntry.new(
                PKMN::Item::FAST_BALL,
                PKMN::Game::NONE
            )
        end
        assert_raise ArgumentError do
            PKMN::Database::ItemEntry.new(
                PKMN::Item::FAST_BALL,
                PKMN::Game::RED
            )
        end

        entry = PKMN::Database::ItemEntry.new(
                    PKMN::Item::FAST_BALL,
                    PKMN::Game::SOULSILVER
                )

        assert_equal(PKMN::Item::FAST_BALL, entry.item)
        assert_equal("Fast Ball", entry.name)
        assert_equal(PKMN::Game::SOULSILVER, entry.game)
        assert_equal("Apricorn balls", entry.category)
        assert_equal("Poké Balls", entry.pocket)

        # Just make sure it works.
        entry.description

        assert_equal(300, entry.cost)
        assert_true(entry.is_holdable)
        assert_equal(0, entry.fling_power)
        assert_equal("None", entry.fling_effect)
    end

    def test_move_entry
        # Make sure trying to create an invalid entry results in an error.
        assert_raise ArgumentError do
            PKMN::Database::MoveEntry.new(
                PKMN::Move::OCTAZOOKA,
                PKMN::Game::NONE
            )
        end
        assert_raise ArgumentError do
            PKMN::Database::MoveEntry.new(
                PKMN::Move::OCTAZOOKA,
                PKMN::Game::RED
            )
        end

        entry = PKMN::Database::MoveEntry.new(
                    PKMN::Move::OCTAZOOKA,
                    PKMN::Game::SILVER
                )

        assert_equal(PKMN::Move::OCTAZOOKA, entry.move)
        assert_equal("Octazooka", entry.name)
        assert_equal(PKMN::Game::SILVER, entry.game)
        assert_equal(PKMN::Type::WATER, entry.type)

        # Just make sure it works.
        entry.description

        assert_equal(PKMN::MoveTarget::SELECTED_POKEMON, entry.target)
        assert_equal(PKMN::MoveDamageClass::SPECIAL, entry.damage_class)
        assert_equal(65, entry.base_power)

        pps = [10,12,14,16]
        (0..3).each do |i|
            assert_equal(pps[i], entry.get_pp(i))
        end

        assert_float_equal(0.85, entry.accuracy)
        assert_equal(0, entry.priority)

        # Just make sure it works
        entry.effect

        assert_equal(PKMN::ContestStat::NONE, entry.contest_type)
        assert_equal("None", entry.contest_effect)
        assert_equal("None", entry.super_contest_effect)
    end

    def test_pokemon_entry
        # Make sure trying to create an invalid entry results in an error.
        assert_raise ArgumentError do
            PKMN::Database::PokemonEntry.new(
                PKMN::Species::CASTFORM,
                PKMN::Game::NONE,
                "Sunny"
            )
        end
        assert_raise ArgumentError do
            PKMN::Database::PokemonEntry.new(
                PKMN::Species::CASTFORM,
                PKMN::Game::BLACK2,
                "Not a form"
            )
        end

        entry = PKMN::Database::PokemonEntry.new(
                    PKMN::Species::STUNFISK,
                    PKMN::Game::BLACK2,
                    ""
                )

        assert_equal(PKMN::Species::STUNFISK, entry.species)
        assert_equal("Stunfisk", entry.species_name)
        assert_equal(PKMN::Game::BLACK2, entry.game)
        assert_equal("Trap", entry.category)
        assert_equal("Standard", entry.form)
        assert(entry.pokedex_entry.length > 0)
        assert_float_equal(0.7, entry.height)
        assert_float_equal(11.0, entry.weight)
        assert_float_equal(0.5, entry.chance_male)
        assert_float_equal(0.5, entry.chance_female)
        assert(!entry.has_gender_differences)
        assert_equal(70, entry.base_friendship)

        assert_equal(PKMN::Type::GROUND, entry.types.first)
        assert_equal(PKMN::Type::ELECTRIC, entry.types.second)

        assert_equal(PKMN::Ability::STATIC, entry.abilities.first)
        assert_equal(PKMN::Ability::LIMBER, entry.abilities.second)

        assert_equal(PKMN::Ability::SAND_VEIL, entry.hidden_ability)

        assert_equal(PKMN::EggGroup::WATER1, entry.egg_groups.first)
        assert_equal(PKMN::EggGroup::INDETERMINATE, entry.egg_groups.second)

        assert_equal(109, entry.base_stats[PKMN::Stat::HP])
        assert_equal(66,  entry.base_stats[PKMN::Stat::ATTACK])
        assert_equal(84,  entry.base_stats[PKMN::Stat::DEFENSE])
        assert_equal(32,  entry.base_stats[PKMN::Stat::SPEED])
        assert_equal(81,  entry.base_stats[PKMN::Stat::SPECIAL_ATTACK])
        assert_equal(99,  entry.base_stats[PKMN::Stat::SPECIAL_DEFENSE])

        assert_equal(2, entry.EV_yields[PKMN::Stat::HP])
        assert_equal(0, entry.EV_yields[PKMN::Stat::ATTACK])
        assert_equal(0, entry.EV_yields[PKMN::Stat::DEFENSE])
        assert_equal(0, entry.EV_yields[PKMN::Stat::SPEED])
        assert_equal(0, entry.EV_yields[PKMN::Stat::SPECIAL_ATTACK])
        assert_equal(0, entry.EV_yields[PKMN::Stat::SPECIAL_DEFENSE])

        assert_equal(165, entry.experience_yield)
        assert_equal(125000, entry.get_experience_at_level(50))
        assert_equal(58, entry.get_level_at_experience(200000))
        assert(entry.levelup_moves.length > 0)
        assert(entry.tm_hm_moves.length > 0)
        assert(entry.egg_moves.length > 0)
        assert(entry.tutor_moves.length > 0)
        assert_equal(1, entry.forms.length)
        assert_equal(0, entry.evolutions.length)

        assert(Pathname.new(entry.get_icon_filepath(false)).exist?)
        assert(Pathname.new(entry.get_icon_filepath(true)).exist?)
        assert(Pathname.new(entry.get_sprite_filepath(false,false)).exist?)
        assert(Pathname.new(entry.get_sprite_filepath(false,true)).exist?)
        assert(Pathname.new(entry.get_sprite_filepath(true,false)).exist?)
        assert(Pathname.new(entry.get_sprite_filepath(true,true)).exist?)
    end
end
