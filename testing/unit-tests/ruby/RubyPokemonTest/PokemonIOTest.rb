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
    def _test_saving_and_loading_pk1(game)
        pk1_path = File.join(PKMN::Paths::get_tmp_dir(), "#{game}_#{@@RNG.rand(0xFFFF)}.pk1")

        item_list = PKMN::Database::get_item_list(game)
        move_list = PKMN::Database::get_move_list(game)
        pokemon_list = PKMN::Database::get_pokemon_list(1, true)

        random_pokemon = get_random_pokemon(game, pokemon_list, move_list, item_list)
        random_pokemon.export_to_file(pk1_path)

        imported_pokemon = PKMN::Pokemon.new(pk1_path)
        compare_pokemon(random_pokemon, imported_pokemon)

        File.delete(pk1_path)
    end

    ["Red", "Blue", "Yellow"].each do |game|
        define_method("test_saving_and_loading_pk1_#{game}") do
            _test_saving_and_loading_pk1(game)
        end
    end

    def _test_saving_and_loading_pk2(game)
        pk2_path = File.join(PKMN::Paths::get_tmp_dir(), "#{game}_#{@@RNG.rand(0xFFFF)}.pk2")

        item_list = PKMN::Database::get_item_list(game)
        move_list = PKMN::Database::get_move_list(game)
        pokemon_list = PKMN::Database::get_pokemon_list(2, true)

        random_pokemon = get_random_pokemon(game, pokemon_list, move_list, item_list)
        random_pokemon.export_to_file(pk2_path)

        imported_pokemon = PKMN::Pokemon.new(pk2_path)
        compare_pokemon(random_pokemon, imported_pokemon)

        File.delete(pk2_path)
    end

    ["Gold", "Silver", "Crystal"].each do |game|
        define_method("test_saving_and_loading_pk2_#{game}") do
            _test_saving_and_loading_pk2(game)
        end
    end

    def _test_saving_and_loading_3gpkm(game)
        _3gpkm_path = File.join(PKMN::Paths::get_tmp_dir(), "#{game}_#{@@RNG.rand(0xFFFF)}.3gpkm")

        item_list = PKMN::Database::get_item_list(game)
        move_list = PKMN::Database::get_move_list(game)
        pokemon_list = PKMN::Database::get_pokemon_list(3, true)

        random_pokemon = get_random_pokemon(game, pokemon_list, move_list, item_list)
        random_pokemon.export_to_file(_3gpkm_path)

        imported_pokemon = PKMN::Pokemon.new(_3gpkm_path)
        compare_pokemon(random_pokemon, imported_pokemon)

        File.delete(_3gpkm_path)
    end

    ["Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen"].each do |game|
        define_method("test_saving_and_loading_3gpkm_#{game}") do
            _test_saving_and_loading_3gpkm(game)
        end
    end

    def test_outside_3gpkm
        _3gpkm_dir = File.join(LIBPKMN_TEST_FILES, "3gpkm")
        mightyena_path = File.join(_3gpkm_dir, "MIGHTYENA.3gpkm")

        mightyena = PKMN::Pokemon.new(mightyena_path)

        assert_equal("Mightyena", mightyena.species)
        assert_equal("Emerald", mightyena.game)
        assert_equal("Standard", mightyena.form)
        assert_equal("MIGHTYENA", mightyena.nickname)
        assert(!mightyena.is_shiny)
        assert_equal("Heart Scale", mightyena.held_item)
        assert_equal("A", mightyena.original_trainer_name)
        assert_equal(61415, mightyena.original_trainer_public_id)
        assert_equal(3417, mightyena.original_trainer_secret_id)
        assert_equal(223997927, mightyena.original_trainer_id)
        assert_equal(PKMN::Gender::FEMALE, mightyena.original_trainer_gender)
        assert_equal(254, mightyena.current_trainer_friendship)
        assert_equal("Intimidate", mightyena.ability)
        assert_equal("Great Ball", mightyena.ball)
        assert_equal(25, mightyena.level_met)
        assert_equal("Route 120", mightyena.location_met)
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

        expected_moves = ["Crunch", "Strength", "Shadow Ball", "Double-Edge"]
        assert_equal(4, expected_moves.length)
        (0..3).each do |index|
            database_entry = PKMN::Database::MoveEntry.new(expected_moves[index], mightyena.game)

            assert_equal(mightyena.moves[index].move, expected_moves[index])
            assert_equal(mightyena.moves[index].pp, database_entry.get_pp(0))
        end

        assert_equal(30, mightyena.EVs["HP"])
        assert_equal(110, mightyena.EVs["Attack"])
        assert_equal(32, mightyena.EVs["Defense"])
        assert_equal(48, mightyena.EVs["Speed"])
        assert_equal(17, mightyena.EVs["Special Attack"])
        assert_equal(83, mightyena.EVs["Special Defense"])

        assert_equal(26, mightyena.IVs["HP"])
        assert_equal(28, mightyena.IVs["Attack"])
        assert_equal(4, mightyena.IVs["Defense"])
        assert_equal(13, mightyena.IVs["Speed"])
        assert_equal(25, mightyena.IVs["Special Attack"])
        assert_equal(26, mightyena.IVs["Special Defense"])

        assert_equal(146, mightyena.stats["HP"])
        assert_equal(122, mightyena.stats["Attack"])
        assert_equal(81, mightyena.stats["Defense"])
        assert_equal(87, mightyena.stats["Speed"])
        assert_equal(79, mightyena.stats["Special Attack"])
        assert_equal(88, mightyena.stats["Special Defense"])
    end
end
