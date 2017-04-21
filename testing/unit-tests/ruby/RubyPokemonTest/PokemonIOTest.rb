#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require_relative "PokemonTest"

require "pathname"

LIBPKMN_TEST_FILES = ENV["LIBPKMN_TEST_FILES"]

class PokemonIO < PokemonTest
    def test_3gpkm
        _3gpkm_dir = File.join(LIBPKMN_TEST_FILES, "3gpkm")
        mightyena_path = File.join(_3gpkm_dir, "MIGHTYENA.3gpkm")

        mightyena = PKMN::Pokemon.new(mightyena_path)

        assert_equal("Mightyena", mightyena.species)
        assert_equal("Emerald", mightyena.game)
        assert_equal("Standard", mightyena.form)
        assert_equal("MIGHTYENA", mightyena.nickname)
        assert(!mightyena.is_shiny?)
        assert_equal("Heart Scale", mightyena.get_held_item().name)
        assert_equal("A", mightyena.trainer_name)
        assert_equal(61415, mightyena.trainer_public_id)
        assert_equal(3417, mightyena.trainer_secret_id)
        assert_equal(223997927, mightyena.trainer_id)
        assert_equal("Female", mightyena.trainer_gender)
        assert_equal(254, mightyena.friendship)
        assert_equal("Intimidate", mightyena.ability)
        assert_equal("Great Ball", mightyena.ball)
        assert_equal(25, mightyena.level_met)
        assert_equal("Route 120", mightyena.get_location_met(false))
        assert_equal("Emerald", mightyena.original_game)
        assert_equal(3557601241, mightyena.personality)
        assert_equal(128734, mightyena.experience)
        assert_equal(50, mightyena.level)

        mightyena.markings.each do |name|
            assert(!mightyena.markings[name])
        end
        
        mightyena.ribbons.each do |name|
            if name == "Champion"
            then
                assert(mightyena.ribbons[name])
            else
                assert(!mightyena.ribbons[name])
            end
        end

        mightyena.contest_stats.each do |name|
            assert_equal(0, mightyena.contest_stats[name])
        end

        mightyena_moves = ["Crunch", "Strength", "Shadow Ball", "Double-Edge"]
        assert_equal(4, mightyena.moves.length)
        (0..3).each do |i|
            assert_equal(mightyena_moves[i], mightyena.moves[i].move.name)
        end

        assert_equal(6, mightyena.EVs.size)
        assert_equal(30, mightyena.EVs["HP"])
        assert_equal(110, mightyena.EVs["Attack"])
        assert_equal(32, mightyena.EVs["Defense"])
        assert_equal(48, mightyena.EVs["Speed"])
        assert_equal(17, mightyena.EVs["Special Attack"])
        assert_equal(83, mightyena.EVs["Special Defense"])

        assert_equal(6, mightyena.IVs.size)
        assert_equal(26, mightyena.IVs["HP"])
        assert_equal(28, mightyena.IVs["Attack"])
        assert_equal(4, mightyena.IVs["Defense"])
        assert_equal(13, mightyena.IVs["Speed"])
        assert_equal(25, mightyena.IVs["Special Attack"])
        assert_equal(26, mightyena.IVs["Special Defense"])

        assert_equal(6, mightyena.stats.size)
        assert_equal(146, mightyena.stats["HP"])
        assert_equal(122, mightyena.stats["Attack"])
        assert_equal(81, mightyena.stats["Defense"])
        assert_equal(87, mightyena.stats["Speed"])
        assert_equal(79, mightyena.stats["Special Attack"])
        assert_equal(88, mightyena.stats["Special Defense"])
    end
end
