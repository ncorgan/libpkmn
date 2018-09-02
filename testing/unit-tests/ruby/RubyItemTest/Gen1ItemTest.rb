#!/usr/bin/ruby
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require_relative "ItemTest"

require "PKMN"

require "minitest/autorun"

class Gen1ItemTest < ItemTest

    @@GAMES = ["Red", "Blue", "Yellow"]

    @@ITEM_NAMES =
    [
        PKMN::Item::POTION,
        PKMN::Item::GREAT_BALL,
        PKMN::Item::ETHER,
        PKMN::Item::PP_UP,
        PKMN::Item::TM34,
        PKMN::Item::MOON_STONE,
        PKMN::Item::BICYCLE,
        PKMN::Item::FULL_HEAL
    ]

    @@INVALID_GENERATION_ITEMS =
    [
        PKMN::Item::AMULET_COIN,
        PKMN::Item::APICOT_BERRY,
        PKMN::Item::AIR_MAIL,
        PKMN::Item::AIR_BALLOON,
        PKMN::Item::AQUA_SUIT
    ]

    def _test_item_list_common(list)
        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(list)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(list, PKMN::Item::POTION)

        # Confirm items from later generations can't be added.
        item_class_test_invalid_items(list, @@INVALID_GENERATION_ITEMS)

        # Start adding and removing items, and make sure the numbers are accurate.
        item_list_test_setting_items(list, @@ITEM_NAMES)
        item_list_test_add_remove(list, @@ITEM_NAMES)

        full_item_list = PKMN::Database::get_item_list(list.game)
        assert_equal(full_item_list.length, list.valid_items.length)
    end

    def _gen1_item_pocket_test(game)
        item_pocket = PKMN::ItemList.new("Items", game)
        assert_equal(20, item_pocket.length)

        _test_item_list_common(item_pocket)
    end

    def _gen1_item_pc_test(game)
        item_pc = PKMN::ItemList.new("PC", game)
        assert_equal(50, item_pc.length)

        _test_item_list_common(item_pc)
    end

    def _gen1_item_bag_test(game)
        bag = PKMN::ItemBag.new(game)
        assert_equal(1, bag.length)

        assert_equal("Items", bag["Items"].name)
        assert_equal(bag.game, bag["Items"].game)

        # Make sure adding items through the bag adds to the pocket.
        (0..7).each do |i|
            bag.add(@@ITEM_NAMES[i], i+1)
        end

        (0..7).each do |i|
            assert_equal(@@ITEM_NAMES[i], bag["Items"][i].item)
            assert_equal(i+1, bag["Items"][i].amount)
        end
        assert_equal(PKMN::Item::NONE, bag["Items"][8].item)
        assert_equal(0, bag["Items"][8].amount)

        # Make sure removing items through the bag removes from the pocket.
        (0..7).each do |i|
            bag.remove(@@ITEM_NAMES[i], i+1)
        end

        (0..8).each do |i|
            assert_equal(PKMN::Item::NONE, bag["Items"][i].item)
            assert_equal(0, bag["Items"][i].amount)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gen1_item_pocket_#{game}") do
            _gen1_item_pocket_test(PKMN::string_to_game(game))
        end
    end

    @@GAMES.each do |game|
        define_method("test_gen1_item_pc_#{game}") do
            _gen1_item_pc_test(PKMN::string_to_game(game))
        end
    end

    @@GAMES.each do |game|
        define_method("test_gen1_item_bag_#{game}") do
            _gen1_item_bag_test(PKMN::string_to_game(game))
        end
    end
end
