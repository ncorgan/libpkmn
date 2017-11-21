#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require_relative "ItemTest"

require "PKMN"

require "minitest/autorun"

class Gen1ItemTest < ItemTest

    @@GAMES = ["Red", "Blue", "Yellow"]

    @@ITEM_NAMES = ["Potion", "Great Ball", "Ether", "PP Up",
                    "TM34", "Moon Stone", "Bicycle", "Full Heal"]

    @@INVALID_GENERATION_ITEMS = ["Amulet Coin", "Apicot Berry", "Air Mail",
                                  "Air Balloon", "Aqua Suit"]

    def _test_item_list_common(list)
        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(list)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(list, "Potion")

        # Confirm items from later generations can't be added.
        item_class_test_invalid_items(list, @@INVALID_GENERATION_ITEMS)

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
        assert_equal("None", bag["Items"][8].item)
        assert_equal(0, bag["Items"][8].amount)

        # Make sure removing items through the bag removes from the pocket.
        (0..7).each do |i|
            bag.remove(@@ITEM_NAMES[i], i+1)
        end

        (0..8).each do |i|
            assert_equal("None", bag["Items"][i].item)
            assert_equal(0, bag["Items"][i].amount)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gen1_item_pocket_#{game}") do
            _gen1_item_pocket_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gen1_item_pc_#{game}") do
            _gen1_item_pc_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gen1_item_bag_#{game}") do
            _gen1_item_bag_test(game)
        end
    end
end
