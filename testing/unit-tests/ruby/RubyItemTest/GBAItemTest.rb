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

class GBAItemTest < ItemTest

    @@GAMES = ["Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen"]

    @@ALL_POCKET_ITEM_NAMES = ["Potion", "Mach Bike", "Great Ball", "TM01",
                               "Aspear Berry", "Wailmer Pail", "Master Ball", "HM04"]

    @@WRONG_GAME_ALL_POCKET_ITEMS = ["Pink Bow", "Black Sludge",
                                     "Ein File S", "Gonzap's Key",
                                     "GS Ball", "Poffin Items",
                                     "TM51",
                                     "Berry", "Occa Berry"]

    def _gba_item_pocket_test_common(item_pocket)
        assert_equal("Items", item_pocket.name)

        capacity = 0
        if item_pocket.game == "Ruby" or item_pocket.game == "Sapphire"
            capacity = 20
        elsif item_pocket.game == "Emerald"
            capacity = 30
        else
            capacity = 42
        end

        assert_equal(capacity, item_pocket.length)

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(item_pocket)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(item_pocket, "Potion")

        # Make sure we can't add items from other pockets.
        item_class_test_invalid_items(
            item_pocket,
            ["Bicycle", "Master Ball", "HM01", "Razz Berry"]
        )

        # Make sure we can't add items from other generations.
        item_class_test_invalid_items(
            item_pocket,
            ["Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"]
        )

        # Make sure we can't add items from Gamecube games.
        item_class_test_invalid_items(
            item_pocket,
            ["Time Flute", "Poké Snack"]
        )

        # Start adding and removing items, and make sure the numbers are accurate.
        test_items = ["Potion", "Orange Mail", "Lava Cookie", "Stardust",
                      "Shadow Mail", "Pink Scarf", "Antidote", "Green Shard"]
        item_list_test_setting_items(item_pocket, test_items)
        item_list_test_add_remove(item_pocket, test_items)

        assert_operator(item_pocket.valid_items.length, :>, 0)
    end

    def _gba_key_item_pocket_test_common(key_item_pocket)
        assert_equal("Key Items", key_item_pocket.name)

        capacity = 0
        if key_item_pocket.game == "Ruby" or key_item_pocket.game == "Sapphire"
            capacity = 20
        else
            capacity = 30
        end

        assert_equal(capacity, key_item_pocket.length)

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(key_item_pocket)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(key_item_pocket, "Basement Key")

        # Make sure we can't add items from other pockets.
        item_class_test_invalid_items(
            key_item_pocket,
            ["Potion", "Master Ball", "HM01", "Razz Berry"]
        )

        # Make sure we can't add items from other generations.
        item_class_test_invalid_items(
            key_item_pocket,
            ["GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"]
        )

        # Make sure we can't add items from incompatible Generation III games.
        item_class_test_invalid_items(
            key_item_pocket,
            ["Ein File S", "Powerup Part", "Gonzap's Key", "Krane Memo 1"] # Gamecube
        )
        if key_item_pocket.game == "Ruby" or key_item_pocket.game == "Sapphire"
            item_class_test_invalid_items(
                key_item_pocket,
                ["Helix Fossil", "Tea", "Ruby"] # FR/LG
            )
        end
        if key_item_pocket.game != "Emerald"
            item_class_test_invalid_items(
                key_item_pocket,
                ["Magma Emblem", "Old Sea Map"] # Emerald
            )
        end

        # Start adding and removing items, and make sure the numbers are accurate.
        test_items = ["Wailmer Pail", "Basement Key", "Meteorite", "Old Rod",
                      "Red Orb", "Root Fossil", "Contest Pass", "Eon Ticket"]
        item_list_test_setting_items(key_item_pocket, test_items)
        item_list_test_add_remove(key_item_pocket, test_items)

        assert_operator(key_item_pocket.valid_items.length, :>, 0)
    end

    def _gba_ball_pocket_test_common(ball_pocket)
        assert_equal("Poké Balls", ball_pocket.name)

        capacity = 0
        if ball_pocket.game == "FireRed" or ball_pocket.game == "LeafGreen"
            capacity = 13
        else
            capacity = 16
        end

        assert_equal(capacity, ball_pocket.length)

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(ball_pocket)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(ball_pocket, "Master Ball")

        # Make sure we can't add items from other pockets.
        item_class_test_invalid_items(
            ball_pocket,
            ["Potion", "Bicycle", "HM01", "Razz Berry"]
        )

        # Make sure we can't add items from other generations.
        item_class_test_invalid_items(
            ball_pocket,
            ["Moon Ball", "Heal Ball", "Dream Ball"]
        )

        # Start adding and removing items, and make sure the numbers are accurate.
        test_items = ["Master Ball", "Ultra Ball", "Great Ball", "Poké Ball",
                      "Safari Ball", "Net Ball", "Dive Ball", "Nest Ball"]
        item_list_test_setting_items(ball_pocket, test_items)
        item_list_test_add_remove(ball_pocket, test_items)

        assert_operator(ball_pocket.valid_items.length, :>, 0)
    end

    def _gba_tmhm_pocket_test_common(tmhm_pocket)
        name = ""
        capacity = 0
        if tmhm_pocket.game == "FireRed" or tmhm_pocket.game == "LeafGreen"
            name = "TM Case"
            capacity = 58
        else
            name = "TMs & HMs"
            capacity = 64
        end

        assert_equal(name, tmhm_pocket.name)
        assert_equal(capacity, tmhm_pocket.length)

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(tmhm_pocket)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(tmhm_pocket, "TM01")

        # Make sure we can't add items from other pockets.
        item_class_test_invalid_items(
            tmhm_pocket,
            ["Potion", "Bicycle", "Master Ball", "Razz Berry"]
        )

        # Make sure we can't add items from other generations.
        item_class_test_invalid_items(
            tmhm_pocket,
            ["TM51"]
        )

        # Start adding and removing items, and make sure the numbers are accurate.
        test_items = ["TM01", "HM01", "TM02", "HM02",
                      "TM03", "HM03", "TM04", "HM04"]
        item_list_test_setting_items(tmhm_pocket, test_items)
        item_list_test_add_remove(tmhm_pocket, test_items)

        assert_operator(tmhm_pocket.valid_items.length, :>, 0)
    end

    def _gba_berry_pocket_test_common(berry_pocket)
        name = ""
        capacity = 0
        if berry_pocket.game == "FireRed" or berry_pocket.game == "LeafGreen"
            name = "Berry Pouch"
            capacity = 43
        else
            name = "Berries"
            capacity = 46
        end

        assert_equal(name, berry_pocket.name)
        assert_equal(capacity, berry_pocket.length)

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(berry_pocket)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(berry_pocket, "Razz Berry")

        # Make sure we can't add items from other pockets.
        item_class_test_invalid_items(
            berry_pocket,
            ["Potion", "Bicycle", "Master Ball", "TM01"]
        )

        # Make sure we can't add items from other generations.
        item_class_test_invalid_items(
            berry_pocket,
            ["Berry", "Occa Berry", "Roseli Berry"]
        )

        # Start adding and removing items, and make sure the numbers are accurate.
        test_items = ["Cheri Berry", "Razz Berry", "Lum Berry", "Pinap Berry",
                      "Aspear Berry", "Iapapa Berry", "Wiki Berry", "Apicot Berry"]
        item_list_test_setting_items(berry_pocket, test_items)
        item_list_test_add_remove(berry_pocket, test_items)

        assert_operator(berry_pocket.valid_items.length, :>, 0)
    end

    def _gba_item_pc_test_common(pc)
        assert_equal("PC", pc.name)
        assert_equal(50, pc.length)

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(pc)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(pc, "Potion")

        # Make sure we can't add items from other generations.
        item_class_test_invalid_items(
            pc,
            @@WRONG_GAME_ALL_POCKET_ITEMS
        )

        # Start adding and removing items, and make sure the numbers are accurate.
        item_list_test_setting_items(pc, @@ALL_POCKET_ITEM_NAMES)
        item_list_test_add_remove(pc, @@ALL_POCKET_ITEM_NAMES)
    end

    def _gba_item_pocket_test(game)
        item_pocket = PKMN::ItemList.new("Items", game)
        assert_equal(game, item_pocket.game)
        _gba_item_pocket_test_common(item_pocket)
    end

    def _gba_key_item_pocket_test(game)
        key_item_pocket = PKMN::ItemList.new("Key Items", game)
        assert_equal(game, key_item_pocket.game)
        _gba_key_item_pocket_test_common(key_item_pocket)
    end

    def _gba_ball_pocket_test(game)
        ball_pocket = PKMN::ItemList.new("Poké Balls", game)
        assert_equal(game, ball_pocket.game)
        _gba_ball_pocket_test_common(ball_pocket)
    end

    def _gba_tmhm_pocket_test(game)
        name = ""
        if game == "FireRed" or game == "LeafGreen"
            name = "TM Case"
        else
            name = "TMs & HMs"
        end
        tmhm_pocket = PKMN::ItemList.new(name, game)
        assert_equal(game, tmhm_pocket.game)
        _gba_tmhm_pocket_test_common(tmhm_pocket)
    end

    def _gba_berry_pocket_test(game)
        name = ""
        if game == "FireRed" or game == "LeafGreen"
            name = "Berry Pouch"
        else
            name = "Berries"
        end
        berry_pocket = PKMN::ItemList.new(name, game)
        assert_equal(game, berry_pocket.game)
        _gba_berry_pocket_test_common(berry_pocket)
    end

    def _gba_item_pc_test(game)
        item_pc = PKMN::ItemList.new("PC", game)
        assert_equal(game, item_pc.game)
        _gba_item_pc_test_common(item_pc)
    end

    def _gba_item_bag_test(game)
        tmhm_pocket_name = ""
        berry_pocket_name = ""
        if game == "FireRed" or game == "LeafGreen"
            tmhm_pocket_name = "TM Case"
            berry_pocket_name = "Berry Pouch"
        else
            tmhm_pocket_name = "TMs & HMs"
            berry_pocket_name = "Berries"
        end

        bag = PKMN::ItemBag.new(game)
        assert_equal(5, bag.length)

        bag.pocket_names.each do |name|
            assert_equal(name, bag[name].name)
            assert_equal(game, bag[name].game)
        end

        assert(bag.pocket_names.include?("Items"))
        assert(bag.pocket_names.include?("Key Items"))
        assert(bag.pocket_names.include?("Poké Balls"))
        assert(bag.pocket_names.include?(tmhm_pocket_name))
        assert(bag.pocket_names.include?(berry_pocket_name))

        _gba_item_pocket_test_common(bag["Items"])
        _gba_key_item_pocket_test_common(bag["Key Items"])
        _gba_ball_pocket_test_common(bag["Poké Balls"])
        _gba_tmhm_pocket_test_common(bag[tmhm_pocket_name])
        _gba_berry_pocket_test_common(bag[berry_pocket_name])

        bag = PKMN::ItemBag.new(game)
        item_pocket = bag["Items"]
        key_item_pocket = bag["Key Items"]
        ball_pocket = bag["Poké Balls"]
        tmhm_pocket = bag[tmhm_pocket_name]
        berry_pocket = bag[berry_pocket_name]

        # Make sure adding items through the bag adds to the pocket.
        @@ALL_POCKET_ITEM_NAMES.each do |item_name|
            bag.add(item_name, 5)
        end

        assert_equal("Potion", item_pocket[0].item)
        assert_equal(5, item_pocket[0].amount)
        assert_equal("None", item_pocket[1].item)
        assert_equal(0, item_pocket[1].amount)

        assert_equal("Mach Bike", key_item_pocket[0].item)
        assert_equal(5, key_item_pocket[0].amount)
        assert_equal("Wailmer Pail", key_item_pocket[1].item)
        assert_equal(5, key_item_pocket[1].amount)
        assert_equal("None", key_item_pocket[2].item)
        assert_equal(0, key_item_pocket[2].amount)

        assert_equal("Great Ball", ball_pocket[0].item)
        assert_equal(5, ball_pocket[0].amount)
        assert_equal("Master Ball", ball_pocket[1].item)
        assert_equal(5, ball_pocket[1].amount)
        assert_equal("None", ball_pocket[2].item)
        assert_equal(0, ball_pocket[2].amount)

        assert_equal("TM01", tmhm_pocket[0].item)
        assert_equal(5, tmhm_pocket[0].amount)
        assert_equal("HM04", tmhm_pocket[1].item)
        assert_equal(5, tmhm_pocket[1].amount)
        assert_equal("None", tmhm_pocket[3].item)
        assert_equal(0, tmhm_pocket[3].amount)

        assert_equal("Aspear Berry", berry_pocket[0].item)
        assert_equal(5, berry_pocket[0].amount)
        assert_equal("None", berry_pocket[1].item)
        assert_equal(0, berry_pocket[1].amount)

        # Make sure removing items through the bag removes from the pocket.
        @@ALL_POCKET_ITEM_NAMES.each do |item_name|
            bag.remove(item_name, 5)
        end

        assert_equal("None", item_pocket[0].item)
        assert_equal(0, item_pocket[0].amount)
        assert_equal("None", item_pocket[1].item)
        assert_equal(0, item_pocket[1].amount)

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

        assert_equal("None", tmhm_pocket[0].item)
        assert_equal(0, tmhm_pocket[0].amount)
        assert_equal("None", tmhm_pocket[1].item)
        assert_equal(0, tmhm_pocket[1].amount)
        assert_equal("None", tmhm_pocket[3].item)
        assert_equal(0, tmhm_pocket[3].amount)

        assert_equal("None", berry_pocket[0].item)
        assert_equal(0, berry_pocket[0].amount)
        assert_equal("None", berry_pocket[1].item)
        assert_equal(0, berry_pocket[1].amount)

        # Make sure we can't add items from other generations or incompatible
        # Generation III games.
        item_class_test_invalid_items(
            bag,
            @@WRONG_GAME_ALL_POCKET_ITEMS
        )
    end

    @@GAMES.each do |game|
        define_method("test_gba_item_pocket_#{game}") do
            _gba_item_pocket_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gba_key_item_pocket_#{game}") do
            _gba_key_item_pocket_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gba_ball_pocket_#{game}") do
            _gba_ball_pocket_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gba_tmhm_pocket_#{game}") do
            _gba_tmhm_pocket_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gba_berry_pocket_#{game}") do
            _gba_berry_pocket_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gba_item_pc_#{game}") do
            _gba_item_pc_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gba_item_bag_#{game}") do
            _gba_item_bag_test(game)
        end
    end
end
