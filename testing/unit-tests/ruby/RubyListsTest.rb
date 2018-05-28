#!/usr/bin/ruby
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"

require "test/unit"

class RubyListsTest < Test::Unit::TestCase
    def test_ability_list
        # Make sure trying to create an invalid list results in an error.
        assert_raise IndexError do
            PKMN::Database::get_ability_list(0)
        end

        ability_list = PKMN::Database::get_ability_list(6)
        assert_equal(191, ability_list.length)
        assert_equal("Adaptability", ability_list[0])
        assert_equal("Zen Mode", ability_list[190])
    end

    def test_game_list
        # Make sure trying to create an invalid list results in an error.
        assert_raise IndexError do
            PKMN::Database::get_game_list(7, false)
        end

        game_list = PKMN::Database::get_game_list(6, true)
        assert_equal(26, game_list.length)
        assert_equal("Red", game_list[0])
        assert_equal("Alpha Sapphire", game_list[25])
    end

    def test_gamecube_shadow_pokemon_list
        colosseum_shadow_pokemon_list = PKMN::Database::get_gamecube_shadow_pokemon_list(true)
        assert_equal(48, colosseum_shadow_pokemon_list.length)

        xd_shadow_pokemon_list = PKMN::Database::get_gamecube_shadow_pokemon_list(false)
        assert_equal(83, xd_shadow_pokemon_list.length)
    end

    def test_hm_move_list
        # Make sure trying to create an invalid list results in an error.
        assert_raise ArgumentError do
            PKMN::Database::get_hm_move_list("Not a game")
        end

        hm_move_list = PKMN::Database::get_hm_move_list("Red")
        assert_equal(5, hm_move_list.length)
        assert_equal("Cut", hm_move_list[0])
        assert_equal("Flash", hm_move_list[4])
    end

    def test_item_list
        # Make sure trying to create an invalid list results in an error.
        assert_raise ArgumentError do
            PKMN::Database::get_item_list("Not a game")
        end

        item_list = PKMN::Database::get_item_list("HeartGold")
        assert_equal(513, item_list.length)
        assert_equal("Adamant Orb", item_list[0])
        assert_equal("Zoom Lens", item_list[512])
    end

    def test_location_list
        # Make sure trying to create an invalid list results in an error.
        assert_raise ArgumentError do
            PKMN::Database::get_location_list("Not a game", false)
        end

        location_list = PKMN::Database::get_location_list("Emerald", false)
        assert_equal(106, location_list.length)
        assert_equal("Abandoned Ship", location_list[0])
        assert_equal("Victory Road", location_list[105])
    end

    def test_move_list
        # Make sure trying to create an invalid list results in an error.
        assert_raise ArgumentError do
            PKMN::Database::get_move_list("Not a game")
        end

        move_list = PKMN::Database::get_move_list("Red")
        assert_equal(165, move_list.length)
        assert_equal("Pound", move_list[0])
        assert_equal("Struggle", move_list[164])
    end

    def test_nature_list
        nature_list = PKMN::Database::get_nature_list()
        assert_equal(25, nature_list.length)
        assert_equal("Hardy", nature_list[0])
        assert_equal("Quirky", nature_list[24])
    end

    def test_pokemon_list
        # Make sure trying to create an invalid list results in an error.
        assert_raise IndexError do
            PKMN::Database::get_pokemon_list(0, true)
        end

        pokemon_list = PKMN::Database::get_pokemon_list(1, true)
        assert_equal(151, pokemon_list.length)
        assert_equal("Bulbasaur", pokemon_list[0])
        assert_equal("Mew", pokemon_list[150])
    end

    def test_region_list
        region_list = PKMN::Database::get_region_list()
        assert_equal(7, region_list.length)
        assert_equal("Kanto", region_list[0])
        assert_equal("Kalos", region_list[6])
    end

    def test_ribbon_list
        omit("TODO")
    end

    def test_super_training_medal_list
        super_training_medal_list = PKMN::Database::get_super_training_medal_list()
        assert_equal(30, super_training_medal_list.length)
        assert_equal("Sp. Atk Level 1", super_training_medal_list[0])
        assert_equal("The Battle for the Best!", super_training_medal_list[29])
    end

    def test_tm_move_list
        # Make sure trying to create an invalid list results in an error.
        assert_raise ArgumentError do
            PKMN::Database::get_tm_move_list("Not a game")
        end

        tm_move_list = PKMN::Database::get_tm_move_list("Red")
        assert_equal(50, tm_move_list.length)
        assert_equal("Mega Punch", tm_move_list[0])
        assert_equal("Substitute", tm_move_list[49])
    end

    def test_type_list
        # Make sure trying to create an invalid list results in an error.
        assert_raise ArgumentError do
            PKMN::Database::get_type_list("Not a game")
        end

        type_list = PKMN::Database::get_type_list("Alpha Sapphire")
        assert_equal(18, type_list.length)
        assert_equal("Normal", type_list[0])
        assert_equal("Fairy", type_list[17])
    end
end
