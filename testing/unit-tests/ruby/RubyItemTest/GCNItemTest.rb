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

class GCNItemTest < ItemTest

    @@GAMES = ["Colosseum", "XD"]

    @@COLOSSEUM_ALL_POCKET_ITEMS = ["Potion", "Ein File S", "Great Ball", "TM01",
                                    "TM02", "Aspear Berry", "Joy Scent", "Excite Scent"]

    @@XD_ALL_POCKET_ITEMS = ["Potion", "Gonzap's Key", "Great Ball", "TM01",
                             "TM02", "Aspear Berry", "Joy Scent", "Battle CD 01"]

    @@COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEMS = ["Pink Bow", "Black Sludge",
                                               "GS Ball", "Gonzap's Key", "Poffin Items",
                                               "TM51", "HM01",
                                               "Berry", "Occa Berry"]

    @@XD_WRONG_GAME_ALL_POCKET_ITEMS = ["Pink Bow", "Black Sludge",
                                        "GS Ball", "Ein File S", "Poffin Items",
                                        "TM51", "HM01",
                                        "Berry", "Occa Berry"]

    def _gcn_item_pocket_test_common(item_pocket)
        assert_equal("Items", item_pocket.name)

        is_colosseum = (item_pocket.game == "Colosseum")
        capacity = 0
        if is_colosseum
            capacity = 20
        else
            capacity = 30
        end

        assert_equal(capacity, item_pocket.length)

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(item_pocket)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(item_pocket, "Potion")

        # Make sure we can't add items from other pockets.
        invalid_items = []
        if is_colosseum
            invalid_items = ["Ein File S", "Great Ball", "TM01", "Oran Berry", "Joy Scent"]
        else
            invalid_items = ["Miror Radar", "Great Ball", "TM01", "Oran Berry", "Joy Scent", "Battle CD 01"]
        end
        item_class_test_invalid_items(item_pocket, invalid_items)

        # Make sure we can't add items from other generations.
        item_class_test_invalid_items(
            item_pocket,
            ["Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"]
        )

        # Start adding and removing items, and make sure the numbers are accurate.
        item_list_test_add_remove(
            item_pocket,
            ["Potion", "Orange Mail", "Lava Cookie", "Stardust",
             "Shadow Mail", "Pink Scarf", "Antidote", "Green Shard"]
        )

        assert_operator(item_pocket.valid_items.length, :>, 0)

        # Make sure the pocket holds both GBA and GCN items.
        gcn_item = ""
        if is_colosseum
            gcn_item = "Time Flute"
        else
            gcn_item = "Poké Snack"
        end

        assert(item_pocket.valid_items.include?("Potion"))
        assert(item_pocket.valid_items.include?(gcn_item))
    end

    def _gcn_key_item_pocket_test_common(key_item_pocket)
        assert_equal("Key Items", key_item_pocket.name)

        is_colosseum = (key_item_pocket.game == "Colosseum")

        assert_equal(43, key_item_pocket.length)

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(key_item_pocket)

        # Confirm errors are thrown when expected.
        gcn_item = ""
        if is_colosseum
            gcn_item = "Ein File S"
        else
            gcn_item = "Miror Radar"
        end
        item_list_test_out_of_range(key_item_pocket, gcn_item)

        # Make sure we can't add items from other pockets.
        invalid_items = []
        if is_colosseum
            invalid_items = ["Potion", "Great Ball", "TM01", "Oran Berry", "Joy Scent"]
        else
            invalid_items = ["Potion", "Great Ball", "TM01", "Oran Berry", "Joy Scent", "Battle CD 01"]
        end
        item_class_test_invalid_items(key_item_pocket, invalid_items)

        # Make sure we can't add items from other generations.
        item_class_test_invalid_items(
            key_item_pocket,
            ["GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"]
        )

        # Make sure we can't add items from incompatible Generation III games.
        item_class_test_invalid_items(
            key_item_pocket,
            ["Helix Fossil", "Tea", "Ruby"] # FR/LG
        )
        item_class_test_invalid_items(
            key_item_pocket,
            ["Magma Emblem", "Old Sea Map"] # Emerald
        )

        # Start adding and removing items, and make sure the numbers are accurate.
        items = []
        if is_colosseum
            items = ["Jail Key", "Elevator Key", "Small Tablet", "F-Disk",
                     "R-Disk", "L-Disk", "D-Disk", "U-Disk"]
        else
            items = ["Krane Memo 1", "Krane Memo 2", "Krane Memo 3", "Krane Memo 4",
                     "Krane Memo 5", "Voice Case 1", "Voice Case 2", "Voice Case 3"]
        end
        item_list_test_add_remove(key_item_pocket, items)

        assert_operator(key_item_pocket.valid_items.length, :>, 0)
        assert(key_item_pocket.valid_items.include?(gcn_item))
    end

    def _gcn_ball_pocket_test_common(ball_pocket)
        assert_equal("Poké Balls", ball_pocket.name)
        assert_equal(16, ball_pocket.length)

        is_colosseum = (ball_pocket.game == "Colosseum")

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(ball_pocket)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(ball_pocket, "Master Ball")

        # Make sure we can't add items from other pockets.
        invalid_items = []
        if is_colosseum
            invalid_items = ["Ein File S", "Potion", "TM01", "Oran Berry", "Joy Scent"]
        else
            invalid_items = ["Miror Radar", "Potion", "TM01", "Oran Berry", "Joy Scent", "Battle CD 01"]
        end
        item_class_test_invalid_items(ball_pocket, invalid_items)

        # Make sure we can't add items from other generations.
        item_class_test_invalid_items(
            ball_pocket,
            ["Moon Ball", "Heal Ball", "Dream Ball"]
        )

        # Start adding and removing items, and make sure the numbers are accurate.
        item_list_test_add_remove(
            ball_pocket,
            ["Master Ball", "Ultra Ball", "Great Ball", "Poké Ball",
             "Safari Ball", "Net Ball", "Dive Ball", "Nest Ball"]
        )

        assert_operator(ball_pocket.valid_items.length, :>, 0)
    end

    def _gcn_tm_pocket_test_common(tm_pocket)
        assert_equal("TMs", tm_pocket.name)
        assert_equal(64, tm_pocket.length)

        is_colosseum = (tm_pocket.game == "Colosseum")

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(tm_pocket)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(tm_pocket, "TM01")

        # Make sure we can't add items from other pockets.
        invalid_items = []
        if is_colosseum
            invalid_items = ["Ein File S", "Potion", "Great Ball", "Oran Berry", "Joy Scent"]
        else
            invalid_items = ["Miror Radar", "Potion", "Great Ball", "Joy Scent", "Battle CD 01"]
        end
        item_class_test_invalid_items(tm_pocket, invalid_items)

        # Make sure we can't add items from other generations or incompatible Generation III games.
        item_class_test_invalid_items(
            tm_pocket,
            ["TM51", "HM01"]
        )

        # Start adding and removing items, and make sure the numbers are accurate.
        item_list_test_add_remove(
            tm_pocket,
            ["TM01", "TM02", "TM03", "TM04",
             "TM05", "TM06", "TM07", "TM08"]
        )

        assert_equal(50, tm_pocket.valid_items.length)

        # Gamecube games have no HMs.
        assert(!tm_pocket.valid_items.include?("HM01"))
    end

    def _gcn_berry_pocket_test_common(berry_pocket)
        assert_equal("Berries", berry_pocket.name)
        assert_equal(46, berry_pocket.length)

        is_colosseum = (berry_pocket.game == "Colosseum")

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(berry_pocket)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(berry_pocket, "Razz Berry")

        # Make sure we can't add items from other pockets.
        invalid_items = []
        if is_colosseum
            invalid_items = ["Ein File S", "Potion", "Great Ball", "TM01", "Joy Scent"]
        else
            invalid_items = ["Miror Radar", "Potion", "TM01", "Great Ball", "Joy Scent", "Battle CD 01"]
        end
        item_class_test_invalid_items(berry_pocket, invalid_items)

        # Make sure we can't add items from other generations.
        item_class_test_invalid_items(
            berry_pocket,
            ["Berry", "Occa Berry", "Roseli Berry"]
        )

        # Start adding and removing items, and make sure the numbers are accurate.
        item_list_test_add_remove(
            berry_pocket,
            ["Cheri Berry", "Razz Berry", "Lum Berry", "Pinap Berry",
             "Aspear Berry", "Iapapa Berry", "Wiki Berry", "Apicot Berry"]
        )

        assert_operator(berry_pocket.valid_items.length, :>, 0)
    end

    def _gcn_cologne_pocket_test_common(cologne_pocket)
        assert_equal("Colognes", cologne_pocket.name)
        assert_equal(3, cologne_pocket.length)

        is_colosseum = (cologne_pocket.game == "Colosseum")

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(cologne_pocket)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(cologne_pocket, "Razz Berry")

        # Make sure we can't add items from other pockets.
        invalid_items = []
        if is_colosseum
            invalid_items = ["Ein File S", "Potion", "Great Ball", "TM01", "Oran Berry"]
        else
            invalid_items = ["Miror Radar", "Potion", "TM01", "Great Ball", "Oran Berry", "Battle CD 01"]
        end
        item_class_test_invalid_items(cologne_pocket, invalid_items)

        cologne_pocket.add("Joy Scent", 3)
        cologne_pocket.add("Excite Scent", 3)
        cologne_pocket.add("Vivid Scent", 3)

        cologne_pocket.remove("Excite Scent", 3)
        cologne_pocket.remove("Vivid Scent", 1)

        assert_equal("Joy Scent", cologne_pocket[0].item)
        assert_equal(3, cologne_pocket[0].amount)
        assert_equal("Vivid Scent", cologne_pocket[1].item)
        assert_equal(2, cologne_pocket[1].amount)
        assert_equal("None", cologne_pocket[2].item)
        assert_equal(0, cologne_pocket[2].amount)

        assert_equal(3, cologne_pocket.valid_items.length)
        assert(cologne_pocket.valid_items.include?("Joy Scent"))
    end

    def _gcn_battle_cd_pocket_test_common(battle_cd_pocket)
        assert_equal("Battle CDs", battle_cd_pocket.name)
        assert_equal(60, battle_cd_pocket.length)

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(battle_cd_pocket)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(battle_cd_pocket, "Battle CD 01")

        # Make sure we can't add items from other pockets.
        item_class_test_invalid_items(
            battle_cd_pocket,
            ["Miror Radar", "Potion", "TM01", "Great Ball", "Oran Berry", "Joy Scent"]
        )

        # Start adding and removing items, and make sure the numbers are accurate.
        item_list_test_add_remove(
            battle_cd_pocket,
            ["Battle CD 01", "Battle CD 02", "Battle CD 03", "Battle CD 04",
             "Battle CD 05", "Battle CD 06", "Battle CD 07", "Battle CD 08"]
        )

        assert_operator(battle_cd_pocket.valid_items.length, :>, 0)
    end

    def _gcn_item_pc_test_common(pc)
        assert_equal("PC", pc.name)
        assert_equal(235, pc.length)

        is_colosseum = (pc.game == "Colosseum")

        # Make sure item slots start as correctly empty.
        item_list_test_empty_slots(pc)

        # Confirm errors are thrown when expected.
        item_list_test_out_of_range(pc, "Potion")

        # Make sure we can't add items from other generations.
        wrong_game_all_pocket_items = []
        if is_colosseum
            wrong_game_all_pocket_items = @@COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEMS
        else
            wrong_game_all_pocket_items = @@XD_WRONG_GAME_ALL_POCKET_ITEMS
        end
        item_class_test_invalid_items(pc, wrong_game_all_pocket_items)

        # Start adding and removing items, and make sure the numbers are accurate.
        all_pocket_items = []
        if is_colosseum
            all_pocket_items = @@COLOSSEUM_ALL_POCKET_ITEMS
        else
            all_pocket_items = @@XD_ALL_POCKET_ITEMS
        end
        item_list_test_add_remove(pc, all_pocket_items)
    end

    def _gcn_item_pocket_test(game)
        item_pocket = PKMN::ItemList.new("Items", game)
        assert_equal(game, item_pocket.game)
        _gcn_item_pocket_test_common(item_pocket)
    end

    def _gcn_key_item_pocket_test(game)
        key_item_pocket = PKMN::ItemList.new("Key Items", game)
        assert_equal(game, key_item_pocket.game)
        _gcn_key_item_pocket_test_common(key_item_pocket)
    end

    def _gcn_ball_pocket_test(game)
        ball_pocket = PKMN::ItemList.new("Poké Balls", game)
        assert_equal(game, ball_pocket.game)
        _gcn_ball_pocket_test_common(ball_pocket)
    end

    def _gcn_tm_pocket_test(game)
        tm_pocket = PKMN::ItemList.new("TMs", game)
        assert_equal(game, tm_pocket.game)
        _gcn_tm_pocket_test_common(tm_pocket)
    end

    def _gcn_berry_pocket_test(game)
        berry_pocket = PKMN::ItemList.new("Berries", game)
        assert_equal(game, berry_pocket.game)
        _gcn_berry_pocket_test_common(berry_pocket)
    end

    def _gcn_cologne_pocket_test(game)
        cologne_pocket = PKMN::ItemList.new("Colognes", game)
        assert_equal(game, cologne_pocket.game)
        _gcn_cologne_pocket_test_common(cologne_pocket)
    end

    def _gcn_battle_cd_pocket_test(game)
        battle_cd_pocket = PKMN::ItemList.new("Battle CDs", game)
        assert_equal(game, battle_cd_pocket.game)
        _gcn_battle_cd_pocket_test_common(battle_cd_pocket)
    end

    def _gcn_item_pc_test(game)
        item_pc = PKMN::ItemList.new("PC", game)
        assert_equal(game, item_pc.game)
        _gcn_item_pc_test_common(item_pc)
    end

    def _gcn_item_bag_test(game)
        is_colosseum = (game == "Colosseum")

        num_pockets = 0
        if is_colosseum
            num_pockets = 6
        else
            num_pockets = 7
        end

        bag = PKMN::ItemBag.new(game)
        assert_equal(num_pockets, bag.length)

        bag.pocket_names.each do |name|
            assert_equal(name, bag[name].name)
            assert_equal(game, bag[name].game)
        end

        assert(bag.pocket_names.include?("Items"))
        assert(bag.pocket_names.include?("Key Items"))
        assert(bag.pocket_names.include?("Poké Balls"))
        assert(bag.pocket_names.include?("TMs"))
        assert(bag.pocket_names.include?("Berries"))
        assert(bag.pocket_names.include?("Colognes"))
        if not is_colosseum
            assert(bag.pocket_names.include?("Battle CDs"))
        end

        _gcn_item_pocket_test_common(bag["Items"])
        _gcn_key_item_pocket_test_common(bag["Key Items"])
        _gcn_ball_pocket_test_common(bag["Poké Balls"])
        _gcn_tm_pocket_test_common(bag["TMs"])
        _gcn_berry_pocket_test_common(bag["Berries"])
        if not is_colosseum
            _gcn_battle_cd_pocket_test_common(bag["Battle CDs"])
        end

        bag = PKMN::ItemBag.new(game)
        item_pocket = bag["Items"]
        key_item_pocket = bag["Key Items"]
        ball_pocket = bag["Poké Balls"]
        tm_pocket = bag["TMs"]
        berry_pocket = bag["Berries"]
        cologne_pocket = bag["Colognes"]

        # Make sure adding items through the bag adds to the pocket.
        all_pocket_item_names = []
        if is_colosseum
            all_pocket_item_names = @@COLOSSEUM_ALL_POCKET_ITEMS
        else
            all_pocket_item_names = @@XD_ALL_POCKET_ITEMS
        end
        all_pocket_item_names.each do |item_name|
            bag.add(item_name, 5)
        end

        gcn_item = ""
        if is_colosseum
            gcn_item = "Ein File S" 
        else
            gcn_item = "Gonzap's Key"
        end

        assert_equal("Potion", item_pocket[0].item)
        assert_equal(5, item_pocket[0].amount)
        assert_equal("None", item_pocket[1].item)
        assert_equal(0, item_pocket[1].amount)

        assert_equal(gcn_item, key_item_pocket[0].item)
        assert_equal(5, key_item_pocket[0].amount)
        assert_equal("None", key_item_pocket[1].item)
        assert_equal(0, key_item_pocket[1].amount)

        assert_equal("Great Ball", ball_pocket[0].item)
        assert_equal(5, ball_pocket[0].amount)
        assert_equal("None", ball_pocket[1].item)
        assert_equal(0, ball_pocket[1].amount)

        assert_equal("TM01", tm_pocket[0].item)
        assert_equal(5, tm_pocket[0].amount)
        assert_equal("TM02", tm_pocket[1].item)
        assert_equal(5, tm_pocket[1].amount)
        assert_equal("None", tm_pocket[3].item)
        assert_equal(0, tm_pocket[3].amount)

        assert_equal("Aspear Berry", berry_pocket[0].item)
        assert_equal(5, berry_pocket[0].amount)
        assert_equal("None", berry_pocket[1].item)
        assert_equal(0, berry_pocket[1].amount)

        assert_equal("Joy Scent", cologne_pocket[0].item)
        assert_equal(5, cologne_pocket[0].amount)

        if is_colosseum
            assert_equal("Excite Scent", cologne_pocket[1].item)
            assert_equal(5, cologne_pocket[1].amount)
        else
            assert_equal("None", cologne_pocket[1].item)
            assert_equal(0, cologne_pocket[1].amount)

            battle_cd_pocket = bag["Battle CDs"]
            assert_equal("Battle CD 01", battle_cd_pocket[0].item)
            assert_equal(5, battle_cd_pocket[0].amount)
            assert_equal("None", battle_cd_pocket[1].item)
            assert_equal(0, battle_cd_pocket[1].amount)
        end

        # Make sure removing items through the bag removes from the pocket.
        all_pocket_item_names.each do |item_name|
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

        assert_equal("None", ball_pocket[0].item)
        assert_equal(0, ball_pocket[0].amount)
        assert_equal("None", ball_pocket[1].item)
        assert_equal(0, ball_pocket[1].amount)

        assert_equal("None", tm_pocket[0].item)
        assert_equal(0, tm_pocket[0].amount)
        assert_equal("None", tm_pocket[1].item)
        assert_equal(0, tm_pocket[1].amount)
        assert_equal("None", tm_pocket[3].item)
        assert_equal(0, tm_pocket[3].amount)

        assert_equal("None", berry_pocket[0].item)
        assert_equal(0, berry_pocket[0].amount)
        assert_equal("None", berry_pocket[1].item)
        assert_equal(0, berry_pocket[1].amount)

        assert_equal("None", cologne_pocket[0].item)
        assert_equal(0, cologne_pocket[0].amount)

        if is_colosseum
            assert_equal("None", cologne_pocket[1].item)
            assert_equal(0, cologne_pocket[1].amount)
        else
            assert_equal("None", cologne_pocket[1].item)
            assert_equal(0, cologne_pocket[1].amount)

            battle_cd_pocket = bag["Battle CDs"]
            assert_equal("None", battle_cd_pocket[0].item)
            assert_equal(0, battle_cd_pocket[0].amount)
            assert_equal("None", battle_cd_pocket[1].item)
            assert_equal(0, battle_cd_pocket[1].amount)
        end

        # Make sure we can't add items from other generations or incompatible
        # Generation III games.
        wrong_game_all_pocket_items = []
        if is_colosseum
            wrong_game_all_pocket_items = @@COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEMS
        else
            wrong_game_all_pocket_items = @@XD_WRONG_GAME_ALL_POCKET_ITEMS
        end
        item_class_test_invalid_items(bag, wrong_game_all_pocket_items)
    end

    @@GAMES.each do |game|
        define_method("test_gcn_item_pocket_#{game}") do
            _gcn_item_pocket_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gcn_key_item_pocket_#{game}") do
            _gcn_key_item_pocket_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gcn_ball_pocket_#{game}") do
            _gcn_ball_pocket_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gcn_tm_pocket_#{game}") do
            _gcn_tm_pocket_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gcn_berry_pocket_#{game}") do
            _gcn_berry_pocket_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gcn_cologne_pocket_#{game}") do
            _gcn_cologne_pocket_test(game)
        end
    end

    def test_gcn_battle_cd_pocket_XD
        _gcn_battle_cd_pocket_test("XD")
    end

    @@GAMES.each do |game|
        define_method("test_gcn_item_pc_#{game}") do
            _gcn_item_pc_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_gcn_item_bag_#{game}") do
            _gcn_item_bag_test(game)
        end
    end
end
