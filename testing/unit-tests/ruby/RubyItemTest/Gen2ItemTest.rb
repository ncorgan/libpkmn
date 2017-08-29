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

class Gen2ItemTest < ItemTest

    @@GAMES = ["Gold", "Silver", "Crystal"]

    @@ALL_POCKET_ITEM_NAMES = ["Potion", "Bicycle", "Great Ball", "TM28",
                               "Berry", "SquirtBottle", "Friend Ball", "HM01"]

    @@CRYSTAL_ITEMS = ["Clear Bell", "GS Ball", "Blue Card", "Egg Ticket"]

    @@WRONG_GENERATION_ALL_POCKET_ITEMS = ["Black Sludge", "Poffin Items", "TM51", "Occa Berry"]

    def _gen2_item_pocket_test_common(item_pocket)
        assert_equal("Items", item_pocket.name)
        assert_equal(20, item_pocket.length)

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(item_pocket)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(item_pocket, "Potion")

        # Make sure we can't add items from other pockets.
        item_class_test_invalid_items(
            item_pocket,
            ["Bicycle", "Master Ball", "HM01"]
        )

        # Make sure we can't add items from other generations.
        item_class_test_invalid_items(
            item_pocket,
            ["Black Flute", "Black Sludge", "Binding Band", "Beedrillite"]
        )

        # Start adding and removing items, and make sure the numbers are accurate.
        item_list_test_add_remove(
            item_pocket,
            ["Potion", "HP Up", "Wht Apricorn", "Lucky Egg",
             "Flower Mail", "Burn Heal", "PSNCureBerry", "Stick"]
        )

        assert_operator(item_pocket.valid_items.length, :>, 0)
    end

    def _gen2_key_item_pocket_test_common(key_item_pocket)
        assert_equal("KeyItems", key_item_pocket.name)
        assert_equal(26, key_item_pocket.length)

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(key_item_pocket)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(key_item_pocket, "Bicycle")

        # Make sure we can't add items from other pockets.
        item_class_test_invalid_items(
            key_item_pocket,
            ["Potion", "Master Ball", "HM01"]
        )

        # Make sure we can't add items from other generations.
        item_class_test_invalid_items(
            key_item_pocket,
            ["Mach Bike", "Jade Orb", "Light Stone", "Aqua Suit"]
        )

        # Crystal-specific items.
        if key_item_pocket.game == "Crystal"
            @@CRYSTAL_ITEMS.each do |item_name|
                key_item_pocket.add(item_name, 1)
                key_item_pocket.remove(item_name, 1)
            end
        else
            @@CRYSTAL_ITEMS.each do |item_name|
                assert_raises ArgumentError do
                    key_item_pocket.add(item_name, 1)
                end
            end
        end

        # Start adding and removing items, and make sure the numbers are valid.
        item_list_test_add_remove(
            key_item_pocket,
            ["Bicycle", "Basement Key", "SecretPotion", "Mystery Egg",
             "Silver Wing", "Lost Item", "SquirtBottle", "Rainbow Wing"]
        )

        assert_operator(key_item_pocket.valid_items.length, :>, 0)
    end

    def _gen2_ball_pocket_test_common(ball_pocket)
        assert_equal("Balls", ball_pocket.name)
        assert_equal(12, ball_pocket.length)

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(ball_pocket)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(ball_pocket, "Potion")

        # Make sure we can't add items from other pockets.
        item_class_test_invalid_items(
            ball_pocket,
            ["Potion", "Bicycle", "HM01"]
        )

        # Make sure we can't add items from other generations.
        item_class_test_invalid_items(
            ball_pocket,
            ["Premier Ball", "Heal Ball", "Dream Ball"]
        )

        # Start adding and removing items, and make sure the numbers are accurate.
        item_list_test_add_remove(
            ball_pocket,
            ["Great Ball", "Poké Ball", "Park Ball", "Fast Ball",
             "Master Ball", "Friend Ball", "Love Ball", "Level Ball"]
        )

        assert_operator(ball_pocket.valid_items.length, :>, 0)
    end

    def _gen2_tmhm_pocket_test_common(tmhm_pocket)
        assert_equal("TM/HM", tmhm_pocket.name)
        assert_equal(57, tmhm_pocket.length)

        # Make sure item slots start as correctly empty.
        (0..49).each do |i|
            assert_equal("TM%02d" % (i+1), tmhm_pocket[i].item)
            assert_equal(0, tmhm_pocket[i].amount)
        end
        (0..6).each do |i|
            assert_equal("HM%02d" % (i+1), tmhm_pocket[50+i].item)
            assert_equal(0, tmhm_pocket[50+i].amount)
        end

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(tmhm_pocket, "TM10")

        # Make sure we can't add items from later generations.
        item_class_test_invalid_items(tmhm_pocket, ["TM51"])

        # Start adding and removing TMs, and make sure the numbers are accurate.
        (1..50).each do |i|
            item_name = "TM%02d" % i
            tmhm_pocket.add(item_name, 50)
            assert_equal(i, tmhm_pocket.num_items)
            assert_equal(item_name, tmhm_pocket[i-1].item)
            assert_equal(50, tmhm_pocket[i-1].amount)
        end
        (1..50).reverse_each do |i|
            item_name = "TM%02d" % i
            tmhm_pocket.remove(item_name, 50)
            assert_equal(i-1, tmhm_pocket.num_items)
            assert_equal(item_name, tmhm_pocket[i-1].item)
            assert_equal(0, tmhm_pocket[i-1].amount)
        end

        # Start adding and removing HMs, and make sure the numbers are accurate.
        (1..7).each do |i|
            item_name = "HM%02d" % i
            tmhm_pocket.add(item_name, 50)
            assert_equal(i, tmhm_pocket.num_items)
            assert_equal(item_name, tmhm_pocket[50+i-1].item)
            assert_equal(50, tmhm_pocket[50+i-1].amount)
        end
        (1..7).reverse_each do |i|
            item_name = "HM%02d" % i
            tmhm_pocket.remove(item_name, 50)
            assert_equal(i-1, tmhm_pocket.num_items)
            assert_equal(item_name, tmhm_pocket[50+i-1].item)
            assert_equal(0, tmhm_pocket[50+i-1].amount)
        end

        assert_equal(57, tmhm_pocket.valid_items.length)
    end

    def _gen2_item_pc_test_common(pc)
        assert_equal("PC", pc.name)
        assert_equal(50, pc.length)

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(pc)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(pc, "Potion")

        # Make sure we can't add items from other generations.
        item_class_test_invalid_items(
            pc,
            @@WRONG_GENERATION_ALL_POCKET_ITEMS
        )

        # Start adding and removing items, and make sure the numbers are accurate.
        item_list_test_add_remove(
            pc,
            @@ALL_POCKET_ITEM_NAMES
        )

        full_item_list = PKMN::Database::get_item_list(pc.game)
        assert_equal(pc.valid_items.length, full_item_list.length)
    end

    def _gen2_item_pocket_test(game)
        item_pocket = PKMN::ItemList.new("Items", game)
        assert_equal(game, item_pocket.game)
        _gen2_item_pocket_test_common(item_pocket)
    end

    def _gen2_key_item_pocket_test(game)
        key_item_pocket = PKMN::ItemList.new("KeyItems", game)
        assert_equal(game, key_item_pocket.game)
        _gen2_key_item_pocket_test_common(key_item_pocket)
    end

    def _gen2_ball_pocket_test(game)
        ball_pocket = PKMN::ItemList.new("Balls", game)
        assert_equal(game, ball_pocket.game)
        _gen2_ball_pocket_test_common(ball_pocket)
    end

    def _gen2_tmhm_pocket_test(game)
        tmhm_pocket = PKMN::ItemList.new("TM/HM", game)
        assert_equal(game, tmhm_pocket.game)
        _gen2_tmhm_pocket_test_common(tmhm_pocket)
    end

    def _gen2_item_pc_test(game)
        item_pc = PKMN::ItemList.new("PC", game)
        assert_equal(game, item_pc.game)
        _gen2_item_pc_test_common(item_pc)
    end

    def _gen2_item_bag_test(game)
        bag = PKMN::ItemBag.new(game)
        assert_equal(4, bag.length)

        bag.pocket_names.each do |name|
            assert_equal(name, bag[name].name)
            assert_equal(game, bag[name].game)
        end

        assert(bag.pocket_names.include?("Items"))
        assert(bag.pocket_names.include?("KeyItems"))
        assert(bag.pocket_names.include?("Balls"))
        assert(bag.pocket_names.include?("TM/HM"))

        _gen2_item_pocket_test_common(bag["Items"])
        _gen2_key_item_pocket_test_common(bag["KeyItems"])
        _gen2_ball_pocket_test_common(bag["Balls"])
        _gen2_tmhm_pocket_test_common(bag["TM/HM"])

        bag = PKMN::ItemBag.new(game)
        item_pocket = bag["Items"]
        key_item_pocket = bag["KeyItems"]
        ball_pocket = bag["Balls"]
        tmhm_pocket = bag["TM/HM"]

        # Make sure adding items through the bag adds to the pocket.
        @@ALL_POCKET_ITEM_NAMES.each do |item_name|
            bag.add(item_name, 5)
        end

        assert_equal("Potion", item_pocket[0].item)
        assert_equal(5, item_pocket[0].amount)
        assert_equal("Berry", item_pocket[1].item)
        assert_equal(5, item_pocket[1].amount)
        assert_equal("None", item_pocket[2].item)
        assert_equal(0, item_pocket[2].amount)

        assert_equal("Bicycle", key_item_pocket[0].item)
        assert_equal(5, key_item_pocket[0].amount)
        assert_equal("SquirtBottle", key_item_pocket[1].item)
        assert_equal(5, key_item_pocket[1].amount)
        assert_equal("None", key_item_pocket[2].item)
        assert_equal(0, key_item_pocket[2].amount)

        assert_equal("Great Ball", ball_pocket[0].item)
        assert_equal(5, ball_pocket[0].amount)
        assert_equal("Friend Ball", ball_pocket[1].item)
        assert_equal(5, ball_pocket[1].amount)
        assert_equal("None", ball_pocket[2].item)
        assert_equal(0, ball_pocket[2].amount)

        assert_equal("TM01", tmhm_pocket[0].item)
        assert_equal(0, tmhm_pocket[0].amount)
        assert_equal("TM02", tmhm_pocket[1].item)
        assert_equal(0, tmhm_pocket[1].amount)
        assert_equal("TM28", tmhm_pocket[27].item)
        assert_equal(5, tmhm_pocket[27].amount)
        assert_equal("HM01", tmhm_pocket[50].item)
        assert_equal(5, tmhm_pocket[50].amount)

        # Make sure removing items through the bag removes from the pocket.
        @@ALL_POCKET_ITEM_NAMES.each do |item_name|
            bag.remove(item_name, 5)
        end

        assert_equal("None", item_pocket[0].item)
        assert_equal(0, item_pocket[0].amount)
        assert_equal("None", item_pocket[1].item)
        assert_equal(0, item_pocket[1].amount)
        assert_equal("None", item_pocket[2].item)
        assert_equal(0, item_pocket[2].amount)

        assert_equal("None", key_item_pocket[0].item)
        assert_equal(0, key_item_pocket[0].amount)
        assert_equal("None", key_item_pocket[1].item)
        assert_equal(0, key_item_pocket[1].amount)
        assert_equal("None", key_item_pocket[2].item)
        assert_equal(0, key_item_pocket[2].amount)

        assert_equal("None", ball_pocket[0].item)
        assert_equal(0, ball_pocket[0].amount)
        assert_equal("None", ball_pocket[1].item)
        assert_equal(0, ball_pocket[1].amount)
        assert_equal("None", ball_pocket[2].item)
        assert_equal(0, ball_pocket[2].amount)

        assert_equal("TM01", tmhm_pocket[0].item)
        assert_equal(0, tmhm_pocket[0].amount)
        assert_equal("TM02", tmhm_pocket[1].item)
        assert_equal(0, tmhm_pocket[1].amount)
        assert_equal("TM28", tmhm_pocket[27].item)
        assert_equal(0, tmhm_pocket[27].amount)
        assert_equal("HM01", tmhm_pocket[50].item)
        assert_equal(0, tmhm_pocket[50].amount)

        # Make sure we can't add/remove Crystal-specific items from a Gold/Silver bag.
        if bag.game == "Crystal"
            @@CRYSTAL_ITEMS.each do |item_name|
                bag.add(item_name, 1)
            end

            (0..(@@CRYSTAL_ITEMS.length-1)).each do |i|
                assert_equal(@@CRYSTAL_ITEMS[i], key_item_pocket[i].item)
                assert_equal(1, key_item_pocket[i].amount)
            end

            @@CRYSTAL_ITEMS.each do |item_name|
                bag.remove(item_name, 1)
            end

            (0..(@@CRYSTAL_ITEMS.length-1)).each do |i|
                assert_equal("None", key_item_pocket[i].item)
                assert_equal(0, key_item_pocket[i].amount)
            end
        else
            item_class_test_invalid_items(
                bag,
                @@CRYSTAL_ITEMS
            )
        end

        # Make sure we can't add items from other generations.
        item_class_test_invalid_items(
            bag,
            @@WRONG_GENERATION_ALL_POCKET_ITEMS
        )
    end

    @@GAMES.each do |game|
        define_method("test_gen2_item_pocket_#{game}") do
            _gen2_item_pocket_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gen2_key_item_pocket_#{game}") do
            _gen2_key_item_pocket_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gen2_ball_pocket_#{game}") do
            _gen2_ball_pocket_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gen2_tmhm_pocket_#{game}") do
            _gen2_tmhm_pocket_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gen2_item_pc_#{game}") do
            _gen2_item_pc_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gen2_item_bag_#{game}") do
            _gen2_item_bag_test(game)
        end
    end
end
