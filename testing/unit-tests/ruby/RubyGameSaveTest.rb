#!/usr/bin/ruby
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"
require "PKMNTest"

require "minitest/autorun"

class GameSaveTest < PKMNTest
    @@GB_GAMES = ["Red", "Blue", "Yellow",
                  "Gold", "Silver", "Crystal"]

    @@RIVAL_NAME_SET_GAMES = ["Ruby", "Sapphire", "Emerald", "Colosseum", "XD",
                              "Black", "White",
                              "X", "Y"]

    @@MALE_ONLY_GAMES = ["Red", "Blue", "Yellow",
                         "Gold", "Silver",
                         "Colosseum", "XD"]

    @@GAMES = ["Red", "Blue", "Yellow",
               "Gold", "Silver", "Crystal",
               "Ruby", "Sapphire", "Emerald",
               "FireRed", "LeafGreen",
               "Colosseum", "XD"]

    @@MONEY_MAX_VALUE = 999999

    @@RNG = Random.new

    @@DEFAULT_TRAINER_PID = PKMN::Pokemon.DEFAULT_TRAINER_ID & 0xFFFF
    @@DEFAULT_TRAINER_SID = PKMN::Pokemon.DEFAULT_TRAINER_ID >> 16

    def _test_trainer_name(save)
        assert_raises ArgumentError do
            save.trainer_name = ""
        end
        assert_raises ArgumentError do
            save.trainer_name = "LibPKMNLibPKMN" # Too long
        end

        save.trainer_name = PKMN::Pokemon.DEFAULT_TRAINER_NAME
        assert_equal(PKMN::Pokemon.DEFAULT_TRAINER_NAME, save.trainer_name)
    end

    def _test_trainer_id(save)
        if @@GB_GAMES.include?(save.game)
            assert_equal(@@DEFAULT_TRAINER_PID, save.trainer_id)
            assert_equal(@@DEFAULT_TRAINER_PID, save.trainer_public_id)
            assert_equal(0, save.trainer_secret_id)
        else
            assert_equal(PKMN::Pokemon.DEFAULT_TRAINER_ID, save.trainer_id)
            assert_equal(@@DEFAULT_TRAINER_PID, save.trainer_public_id)
            assert_equal(@@DEFAULT_TRAINER_SID, save.trainer_secret_id)
        end
    end

    def _test_rival_name(save)
        if @@RIVAL_NAME_SET_GAMES.include?(save.game)
            assert_raises RuntimeError do
                save.rival_name = "ABCDEF"
            end
        else
            assert_raises ArgumentError do
                save.rival_name = ""
            end
            assert_raises ArgumentError do
                save.rival_name = "LibPKMNLibPKMN" # Too long
            end

            save.rival_name = PKMN::Pokemon.DEFAULT_TRAINER_NAME
            assert_equal(PKMN::Pokemon.DEFAULT_TRAINER_NAME, save.rival_name)
        end
    end

    def _test_game_save_common_fields(save)
        generation = @@GAME_GENERATIONS[save.game]

        _test_trainer_name(save)

        if @@GB_GAMES.include?(save.game)
            save.trainer_id = @@DEFAULT_TRAINER_PID
        else
            save.trainer_id = PKMN::Pokemon.DEFAULT_TRAINER_ID
        end
        _test_trainer_id(save)

        save.trainer_public_id = @@DEFAULT_TRAINER_PID
        _test_trainer_id(save)

        if @@GB_GAMES.include?(save.game)
            assert_raises RuntimeError do
                save.trainer_secret_id = @@DEFAULT_TRAINER_SID
            end
        else
            save.trainer_secret_id = @@DEFAULT_TRAINER_SID
        end
        _test_trainer_id(save)

        _test_rival_name(save)

        if @@MALE_ONLY_GAMES.include?(save.game)
            assert_equal("Male", save.trainer_gender)
            assert_raises RuntimeError do
                save.trainer_gender = "Male"
            end
            assert_raises RuntimeError do
                save.trainer_gender = "Female"
            end
        else
            save.trainer_gender = "Male"
            assert_equal("Male", save.trainer_gender)
            save.trainer_gender = "Female"
            assert_equal("Female", save.trainer_gender)
            assert_raises ArgumentError do
                save.trainer_gender = "Genderless"
            end
        end

        assert_raises IndexError do
            save.money = -1
        end
        assert_raises IndexError do
            save.money = @@MONEY_MAX_VALUE + 1
        end

        save.money = 123456
        assert_equal(123456, save.money)

        # Make sure party.num_pokemon is accurate.
        assert_equal(6, save.pokemon_party.length)
        (0..5).each do |i|
            if i < save.pokemon_party.num_pokemon
                assert(save.pokemon_party[i].species != "None")

                if save.game != "Colosseum" and save.game != "XD"
                    if not save.pokemon_party[i].is_egg
                        assert(save.pokedex.seen_pokemon_hash[save.pokemon_party[i].species])
                        assert(save.pokedex.caught_pokemon_hash[save.pokemon_party[i].species])
                    end
                end
            else
                assert_equal("None", save.pokemon_party[i].species)
            end
        end

        if generation >= 2
            assert_equal(save.pokemon_pc.length, save.pokemon_pc.box_names.length)
        end

        save.pokemon_pc.each do |box|
            (0..(box.length-1)).each do |i|
                # Boxes are only contiguous in Game Boy games.
                if @@GB_GAMES.include?(save.game)
                    if i < box.num_pokemon
                        assert(box[i].species != "None")

                        if save.game != "Colosseum" and save.game != "XD"
                            if not box[i].is_egg
                                assert(save.pokedex.seen_pokemon_hash[box[i].species])
                                assert(save.pokedex.caught_pokemon_hash[box[i].species])
                            end
                        end
                    else
                        assert_equal("None", box[i].species)
                    end
                end
            end
        end
    end

    def _test_attributes(save)
        generation = @@GAME_GENERATIONS[save.game]

        case generation
        when 1
            assert(save.numeric_attributes.names.include?("Casino coins"))
            assert_operator(
                save.numeric_attributes["Casino coins"],
                :>=,
                0
            )
            assert_operator(
                save.numeric_attributes["Casino coins"],
                :<=,
                9999
            )

            # TODO: uncomment after fixing:
            # * https://github.com/ncorgan/pksav/issues/3
=begin
            casino_coins = @@RNG.rand(10000)
            save.numeric_attributes["Casino coins"] = casino_coins
            assert_equal(casino_coins, save.numeric_attributes["Casino coins"])
=end

            if save.game == "Yellow"
                assert(save.numeric_attributes.names.include?("Pikachu friendship"))
                assert_operator(
                    save.numeric_attributes["Pikachu friendship"],
                    :>=,
                    0
                )
                assert_operator(
                    save.numeric_attributes["Pikachu friendship"],
                    :<=,
                    255
                )

                pikachu_friendship = @@RNG.rand(256)
                save.numeric_attributes["Pikachu friendship"] = pikachu_friendship
                assert_equal(pikachu_friendship, save.numeric_attributes["Pikachu friendship"])
            else
                assert(!save.numeric_attributes.names.include?("Pikachu friendship"))
            end
        when 3
            if save.game != "Colosseum" and save.game != "XD"
                assert(save.numeric_attributes.names.include?("Casino coins"))
                assert_operator(
                    save.numeric_attributes["Casino coins"],
                    :>=,
                    0
                )
                assert_operator(
                    save.numeric_attributes["Casino coins"],
                    :<=,
                    9999
                )

                casino_coins = @@RNG.rand(10000)
                save.numeric_attributes["Casino coins"] = casino_coins
                assert_equal(casino_coins, save.numeric_attributes["Casino coins"])
            else
                assert(!save.numeric_attributes.names.include?("Casino coins"))
            end
        end
    end

    def _randomize_pokemon(save, item_list)
        pokemon_list = PKMN::Database::get_pokemon_list(@@GAME_GENERATIONS[save.game], true)
        move_list = PKMN::Database::get_move_list(save.game)

        (0..5).each do |i|
            save.pokemon_party[i] = get_random_pokemon(save.game, pokemon_list, move_list, item_list)
        end

        save.pokemon_pc.each do |box|
            (0..(box.length-1)).each do |i|
                box[i] = get_random_pokemon(save.game, pokemon_list, move_list, item_list)
            end
        end
    end

    def _compare_item_lists(item_list1, item_list2)
        assert_equal(item_list1.length, item_list2.length)

        (0..(item_list1.length-1)).each do |i|
            assert_equal(item_list1[i].item, item_list2[i].item)
            assert_equal(item_list1[i].amount, item_list2[i].amount)
        end
    end

    def _compare_pokemon(pokemon1, pokemon2)
        assert_equal(pokemon1.species, pokemon2.species)
        assert_equal(pokemon1.level, pokemon2.level)
        assert_equal(pokemon1.nickname, pokemon2.nickname)
        assert_equal(pokemon1.original_trainer_name, pokemon2.original_trainer_name)
    end

    def _compare_game_saves(save1, save2)
        assert_equal(save1.game, save2.game)
        assert_equal(save1.trainer_name, save2.trainer_name)
        assert_equal(save1.trainer_id, save2.trainer_id)
        assert_equal(save1.trainer_public_id, save2.trainer_public_id)

        if not @@GB_GAMES.include?(save1.game)
            assert_equal(save1.trainer_secret_id, save2.trainer_secret_id)
        end
        if not @@MALE_ONLY_GAMES.include?(save1.game)
            assert_equal(save1.trainer_gender, save2.trainer_gender)
        end
        if not @@RIVAL_NAME_SET_GAMES.include?(save1.game)
            assert_equal(save1.rival_name, save2.rival_name)
        end

        assert_equal(save1.money, save2.money)

        save1.item_bag.pocket_names.each do |pocket_name|
            _compare_item_lists(save1.item_bag[pocket_name], save2.item_bag[pocket_name])
        end
        if @@GAME_GENERATIONS[save1.game] <= 3
            _compare_item_lists(save1.item_pc, save2.item_pc)
        end

        (0..5).each do |i|
            _compare_pokemon(save1.pokemon_party[i], save2.pokemon_party[i])
        end

        assert_equal(save1.pokemon_pc.length, save2.pokemon_pc.length)
        (0..(save1.pokemon_pc.length-1)).each do |i|
            assert_equal(save1.pokemon_pc[i].length, save2.pokemon_pc[i].length)
            (0..(save1.pokemon_pc[i].length-1)).each do |j|
                _compare_pokemon(save1.pokemon_pc[i][j], save2.pokemon_pc[i][j])
            end
        end

        if save1.game != "Colosseum" and save1.game != "XD"
            assert_equal(save1.pokedex.all_seen, save2.pokedex.all_seen)
            assert_equal(save1.pokedex.all_caught, save2.pokedex.all_caught)
        end
    end

    def _game_save_test(test_params)
        save_type = test_params[0]
        game = test_params[1]
        subpath = test_params[2]

        save_file_dir = ""
        if (game == "Colosseum") or (game == "XD")
            save_file_dir = ENV["LIBPKMN_TEST_FILES"]
        else
            save_file_dir = ENV["PKSAV_TEST_SAVES"]
        end

        save_path = File.join(save_file_dir, subpath)
        assert_equal(save_type, PKMN::GameSave::detect_type(save_path))

        save = PKMN::GameSave.new(save_path)
        assert_equal(game, save.game)

        item_list = PKMN::Database::get_item_list(game)

        _test_game_save_common_fields(save)
        _test_attributes(save)
        _randomize_pokemon(save, item_list)

        temp_save_path = File.join(PKMN::Paths::get_tmp_dir(), "#{save.game}_#{@@RNG.rand(0xFFFF)}.sav")
        save.save_as(temp_save_path)
        save2 = PKMN::GameSave.new(temp_save_path)
        _compare_game_saves(save, save2)

        compare_attributes(save, save2)

        File.delete(temp_save_path)
    end

    [["Red/Blue", "Red", "red_blue/pokemon_red.sav"],
     ["Yellow", "Yellow", "yellow/pokemon_yellow.sav"],
     ["Gold/Silver", "Gold", "gold_silver/pokemon_gold.sav"],
     ["Crystal", "Crystal", "crystal/pokemon_crystal.sav"],
     ["Ruby/Sapphire", "Ruby", "ruby_sapphire/pokemon_ruby.sav"],
     ["Emerald", "Emerald", "emerald/pokemon_emerald.sav"],
     ["FireRed/LeafGreen", "FireRed", "firered_leafgreen/pokemon_firered.sav"],
     ["Colosseum/XD", "Colosseum", "gamecube_saves/pokemon_colosseum.gci"],
     ["Colosseum/XD", "XD", "gamecube_saves/pokemon_xd.gci"]].each do |test_params|
        define_method("test_game_save_#{test_params[1]}") do
            _game_save_test(test_params)
        end
    end
end
