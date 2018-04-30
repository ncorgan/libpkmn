#!/usr/bin/env python
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

from .items_tests import items_tests

import sys
import unittest

PYTHON_MAJOR_VERSION = sys.version_info[0]

COLOSSEUM_ALL_POCKET_ITEMS = [
    "Potion", "Ein File S", "Great Ball", "TM01",
    "TM02", "Aspear Berry", "Joy Scent", "Excite Scent"
]
XD_ALL_POCKET_ITEMS = [
    "Potion", "Gonzap's Key", "Great Ball", "TM01",
    "TM02", "Aspear Berry", "Joy Scent", "Battle CD 01"
]
COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEMS = [
    "Pink Bow", "Black Sludge",
    "GS Ball", "Gonzap's Key", "Poffin Items",
    "TM51", "HM01",
    "Berry", "Occa Berry"
]
XD_WRONG_GAME_ALL_POCKET_ITEMS = [
    "Pink Bow", "Black Sludge",
    "GS Ball", "Ein File S", "Poffin Items",
    "TM51", "HM01",
    "Berry", "Occa Berry"
]

class gcn_items_test(items_tests):


    #
    # Helper functions
    #

    def __test_item_pocket(self, item_pocket, game):
        colosseum = (game == "Colosseum")
        expected_capacity = 20 if colosseum else 30

        # Check unchanging and initial values.
        self.assertEqual(len(item_pocket), expected_capacity)
        self.assertEqual(item_pocket.name, "Items")
        self.assertEqual(item_pocket.game, game)
        self.assertEqual(len(item_pocket), expected_capacity)
        self.assertEqual(item_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(item_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(item_pocket, "Potion")

        # Make sure we can't add items from other pockets.'
        wrong_pocket_items = ["Ein File S", "Great Ball", "TM01", "Oran Berry", "Joy Scent"] if colosseum else \
                             ["Miror Radar", "Great Ball", "TM01", "Oran Berry", "Joy Scent", "Battle CD 01"]
        self.item_class_test_invalid_items(
            item_pocket,
            wrong_pocket_items
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            item_pocket,
            ["Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            item_pocket,
            [u"Potion", u"Orange Mail", u"Lava Cookie", u"Stardust",
             u"Shadow Mail", u"Pink Scarf", u"Antidote", u"Green Shard"]
        )

        valid_items = item_pocket.valid_items
        self.assertGreater(len(valid_items), 0)

        # See TODO in item_entry.hpp
        #self.item_list_test_both_text_types(item_pocket)

    def __test_key_item_pocket(self, key_item_pocket, game):
        colosseum = (game == "Colosseum")
        expected_capacity = 43

        # Check unchanging and initial values.
        self.assertEqual(len(key_item_pocket), expected_capacity)
        self.assertEqual(key_item_pocket.name, "Key Items")
        self.assertEqual(key_item_pocket.game, game)
        self.assertEqual(len(key_item_pocket), expected_capacity)
        self.assertEqual(key_item_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(key_item_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(
            key_item_pocket,
            "Ein File S" if colosseum else "Miror Radar"
        )

        # Make sure we can't add items from other pockets.
        wrong_pocket_items = ["Potion", "Great Ball", "TM01", "Oran Berry", "Joy Scent"] if colosseum else \
                             ["Potion", "Great Ball", "TM01", "Oran Berry", "Joy Scent", "Battle CD 01"]
        self.item_class_test_invalid_items(
            key_item_pocket,
            wrong_pocket_items
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            key_item_pocket,
            ["GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"]
        )

        # Make sure we can't add items from incompatible Generation III games.
        self.item_class_test_invalid_items(
            key_item_pocket,
            ["Helix Fossil", "Tea", "Ruby"]
        )
        self.item_class_test_invalid_items(
            key_item_pocket,
            ["Magma Emblem", "Old Sea Map"]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        items = ["Jail Key", "Elevator Key", "Small Tablet", "F-Disk",
                 "R-Disk", "L-Disk", "D-Disk", "U-Disk"] if colosseum else \
                ["Krane Memo 1", "Krane Memo 2", "Krane Memo 3", "Krane Memo 4",
                 "Krane Memo 5", "Voice Case 1", "Voice Case 2", "Voice Case 3"]
        self.item_list_test_add_remove(
            key_item_pocket,
            items
        )

        valid_items = key_item_pocket.valid_items
        self.assertGreater(len(valid_items), 0)
        self.item_list_test_both_text_types(key_item_pocket)

    def __test_ball_pocket(self, ball_pocket, game):
        colosseum = (game == "Colosseum")
        expected_capacity = 16

        # Check unchanging and initial values.
        self.assertEqual(len(ball_pocket), expected_capacity)
        self.assertStringEqual(ball_pocket.name, u"Pok\u00e9 Balls")
        self.assertEqual(ball_pocket.game, game)
        self.assertEqual(len(ball_pocket), expected_capacity)
        self.assertEqual(ball_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(ball_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(ball_pocket, "Master Ball")

        # Make sure we can't add items from other pockets.
        wrong_pocket_items = ["Ein File S", "Potion", "TM01", "Oran Berry", "Joy Scent"] if colosseum else \
                             ["Miror Radar", "Potion", "TM01", "Oran Berry", "Joy Scent", "Battle CD 01"]
        self.item_class_test_invalid_items(
            ball_pocket,
            wrong_pocket_items
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            ball_pocket,
            ["Moon Ball", "Heal Ball", "Dream Ball"]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        item_names = [u"Master Ball", u"Ultra Ball", u"Great Ball", u"Pok\u00e9 Ball",
                      u"Safari Ball", u"Net Ball", u"Dive Ball", u"Nest Ball"]
        self.item_list_test_add_remove(
            ball_pocket,
            item_names
        )

        valid_items = ball_pocket.valid_items
        self.assertGreater(len(valid_items), 0)
        self.item_list_test_both_text_types(ball_pocket)

    def __test_tm_pocket(self, tm_pocket, game):
        colosseum = (game == "Colosseum")
        expected_capacity = 64

        # Check unchanging and initial values.
        self.assertEqual(len(tm_pocket), expected_capacity)
        self.assertEqual(tm_pocket.name, "TMs")
        self.assertEqual(tm_pocket.game, game)
        self.assertEqual(len(tm_pocket), expected_capacity)
        self.assertEqual(tm_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(tm_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(tm_pocket, "TM01")

        # Make sure we can't add items from other pockets.
        wrong_pocket_items = ["Ein File S", "Potion", "Great Ball", "Oran Berry", "Joy Scent"] if colosseum else \
                             ["Miror Radar", "Potion", "Great Ball", "Oran Berry", "Joy Scent", "Battle CD 01"]
        self.item_class_test_invalid_items(
            tm_pocket,
            wrong_pocket_items
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            tm_pocket,
            ["TM51"]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            tm_pocket,
            ["TM01", "TM02", "TM03", "TM04",
             "TM05", "TM06", "TM07", "TM08"]
        )

        valid_items = tm_pocket.valid_items
        self.assertGreater(len(valid_items), 0)
        self.item_list_test_both_text_types(tm_pocket)

    def __test_berry_pocket(self, berry_pocket, game):
        colosseum = (game == "Colosseum")
        expected_capacity = 46

        # Check unchanging and initial values.
        self.assertEqual(len(berry_pocket), expected_capacity)
        self.assertEqual(berry_pocket.name, "Berries")
        self.assertEqual(berry_pocket.game, game)
        self.assertEqual(len(berry_pocket), expected_capacity)
        self.assertEqual(berry_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(berry_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(berry_pocket, "Razz Berry")

        # Make sure we can't add items from other pockets.
        wrong_pocket_items = ["Ein File S", "Potion", "Great Ball", "TM01", "Joy Scent"] if colosseum else \
                             ["Miror Radar", "Potion", "Great Ball", "TM01", "Joy Scent", "Battle CD 01"]
        self.item_class_test_invalid_items(
            berry_pocket,
            wrong_pocket_items
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            berry_pocket,
            ["Berry", "Occa Berry", "Roseli Berry"]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            berry_pocket,
            [u"Cheri Berry", u"Razz Berry", u"Lum Berry", u"Pinap Berry",
             u"Aspear Berry", u"Iapapa Berry", u"Wiki Berry", u"Apicot Berry"]
        )

        valid_items = berry_pocket.valid_items
        self.assertGreater(len(valid_items), 0)
        self.item_list_test_both_text_types(berry_pocket)

    def __test_cologne_pocket(self, cologne_pocket, game):
        colosseum = (game == "Colosseum")
        expected_capacity = 3

        # Check unchanging and initial values.
        self.assertEqual(len(cologne_pocket), expected_capacity)
        self.assertEqual(cologne_pocket.name, "Colognes")
        self.assertEqual(cologne_pocket.game, game)
        self.assertEqual(len(cologne_pocket), expected_capacity)
        self.assertEqual(cologne_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(cologne_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(cologne_pocket, "Joy Scent")

        # Make sure we can't add items from other pockets.
        wrong_pocket_items = ["Ein File S", "Potion", "Great Ball", "TM01", "Oran Berry"] if colosseum else \
                             ["Miror Radar", "Potion", "Great Ball", "TM01", "Oran Berry", "Battle CD 01"]
        self.item_class_test_invalid_items(
            cologne_pocket,
            wrong_pocket_items
        )

        # Since this pocket can only have 3 items, we can't use our typical function, which requires 8.
        cologne_pocket.add("Joy Scent", 3)
        cologne_pocket.add("Excite Scent", 3)
        cologne_pocket.add("Vivid Scent", 3)

        cologne_pocket.remove("Excite Scent", 3)
        cologne_pocket.remove("Vivid Scent", 1)

        self.assertEquals("Joy Scent", cologne_pocket[0].item)
        self.assertEquals(3, cologne_pocket[0].amount)
        self.assertEquals("Vivid Scent", cologne_pocket[1].item)
        self.assertEquals(2, cologne_pocket[1].amount)
        self.assertEquals("None", cologne_pocket[2].item)
        self.assertEquals(0, cologne_pocket[2].amount)

        cologne_pocket.remove("Joy Scent", 3)
        cologne_pocket.remove("Vivid Scent", 2)

        self.assertEquals("None", cologne_pocket[0].item)
        self.assertEquals(0, cologne_pocket[0].amount)
        self.assertEquals("None", cologne_pocket[1].item)
        self.assertEquals(0, cologne_pocket[1].amount)
        self.assertEquals("None", cologne_pocket[2].item)
        self.assertEquals(0, cologne_pocket[2].amount)

        valid_items = cologne_pocket.valid_items
        self.assertEquals(len(valid_items), 3)
        self.item_list_test_both_text_types(cologne_pocket)

    def __test_battle_cd_pocket(self, battle_cd_pocket, game):
        expected_capacity = 60

        # Check unchanging and initial values.
        self.assertEqual(len(battle_cd_pocket), expected_capacity)
        self.assertEqual(battle_cd_pocket.name, "Battle CDs")
        self.assertEqual(battle_cd_pocket.game, game)
        self.assertEqual(len(battle_cd_pocket), expected_capacity)
        self.assertEqual(battle_cd_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(battle_cd_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(battle_cd_pocket, "Battle CD 01")

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            battle_cd_pocket,
            ["Miror Radar", "Potion", "Great Ball", "TM01", "Joy Scent", "Oran Berry"]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            battle_cd_pocket,
            [u"Battle CD 01", u"Battle CD 02", u"Battle CD 03", u"Battle CD 04",
             u"Battle CD 05", u"Battle CD 06", u"Battle CD 07", u"Battle CD 08"]
        )

        valid_items = battle_cd_pocket.valid_items
        self.assertEquals(len(valid_items), 60)
        self.item_list_test_both_text_types(battle_cd_pocket)

    def __test_pc(self, pc, game):
        colosseum = (game == "Colosseum")
        expected_capacity = 235

        # Check unchanging and initial values.
        self.assertEqual(len(pc), expected_capacity)
        self.assertEqual(pc.name, "PC")
        self.assertEqual(pc.game, game)
        self.assertEqual(len(pc), expected_capacity)
        self.assertEqual(pc.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(pc)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(pc, "Potion")

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            pc,
            COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEMS if colosseum else XD_WRONG_GAME_ALL_POCKET_ITEMS
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            pc,
            COLOSSEUM_ALL_POCKET_ITEMS if colosseum else XD_ALL_POCKET_ITEMS
        )

        valid_items = pc.valid_items
        self.assertGreater(len(valid_items), 0)
        self.item_list_test_both_text_types(pc)

    def __test_item_bag(self, bag, game):
        colosseum = (game == "Colosseum")

        # Check unchanging and initial values.
        self.assertEqual(bag.game, game)

        expected_num_pockets = 6 if colosseum else 7
        self.assertEqual(len(bag), expected_num_pockets)
        self.assertEqual(len(bag.pocket_names), expected_num_pockets)

        unicode_pocket_names = []
        if PYTHON_MAJOR_VERSION == 2:
            unicode_pocket_names = [pocket_name.decode("utf-8") for pocket_name in bag.pocket_names]
        else:
            unicode_pocket_names = bag.pocket_names

        self.assertTrue("Items" in unicode_pocket_names)
        self.assertTrue("Key Items" in unicode_pocket_names)
        self.assertTrue(u"Pok\u00e9 Balls" in unicode_pocket_names)
        self.assertTrue("TMs" in unicode_pocket_names)
        self.assertTrue("Berries" in unicode_pocket_names)
        self.assertTrue("Colognes" in unicode_pocket_names)
        if not colosseum:
            self.assertTrue("Battle CDs" in unicode_pocket_names)

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            bag,
            COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEMS if colosseum else XD_WRONG_GAME_ALL_POCKET_ITEMS
        )

        self.__test_item_pocket(bag["Items"], game)
        self.__test_key_item_pocket(bag["Key Items"], game)
        self.__test_ball_pocket(bag[u"Pok\u00e9 Balls"], game)
        self.__test_tm_pocket(bag["TMs"], game)
        self.__test_berry_pocket(bag["Berries"], game)
        self.__test_cologne_pocket(bag["Colognes"], game)
        if not colosseum:
            self.__test_battle_cd_pocket(bag["Battle CDs"], game)
        self.item_bag_test_get_pockets_with_both_text_types(bag)

        items = COLOSSEUM_ALL_POCKET_ITEMS if colosseum else XD_ALL_POCKET_ITEMS

        # Make sure adding items through the bag adds to the proper pocket.
        self.assertEqual(bag["Items"].num_items, 0)
        self.assertEqual(bag["Key Items"].num_items, 0)
        self.assertEqual(bag[u"Pok\u00e9 Balls"].num_items, 0)
        self.assertEqual(bag["TMs"].num_items, 0)
        self.assertEqual(bag["Berries"].num_items, 0)
        self.assertEqual(bag["Colognes"].num_items, 0)
        if not colosseum:
            self.assertEqual(bag["Battle CDs"].num_items, 0)
        for item in items:
            bag.add(item, 5)

        self.assertEqual(bag["Items"][0].item, "Potion")
        self.assertEqual(bag["Items"][0].amount, 5)
        self.assertEqual(bag["Items"][1].item, "None")
        self.assertEqual(bag["Items"][1].amount, 0)

        key_item = "Ein File S" if colosseum else "Gonzap's Key"
        self.assertEqual(bag["Key Items"][0].item, key_item)
        self.assertEqual(bag["Key Items"][0].amount, 5)
        self.assertEqual(bag["Key Items"][1].item, "None")
        self.assertEqual(bag["Key Items"][1].amount, 0)

        self.assertEqual(bag[u"Pok\u00e9 Balls"][0].item, "Great Ball")
        self.assertEqual(bag[u"Pok\u00e9 Balls"][0].amount, 5)
        self.assertEqual(bag[u"Pok\u00e9 Balls"][1].item, "None")
        self.assertEqual(bag[u"Pok\u00e9 Balls"][1].amount, 0)

        self.assertEqual(bag["TMs"][0].item, "TM01")
        self.assertEqual(bag["TMs"][0].amount, 5)
        self.assertEqual(bag["TMs"][1].item, "TM02")
        self.assertEqual(bag["TMs"][1].amount, 5)
        self.assertEqual(bag["TMs"][2].item, "None")
        self.assertEqual(bag["TMs"][2].amount, 0)

        self.assertEqual(bag["Berries"][0].item, "Aspear Berry")
        self.assertEqual(bag["Berries"][0].amount, 5)
        self.assertEqual(bag["Berries"][1].item, "None")
        self.assertEqual(bag["Berries"][1].amount, 0)

        self.assertEqual(bag["Colognes"][0].item, "Joy Scent")
        self.assertEqual(bag["Colognes"][0].amount, 5)
        if colosseum:
            self.assertEqual(bag["Colognes"][1].item, "Excite Scent")
            self.assertEqual(bag["Colognes"][1].amount, 5)
            self.assertEqual(bag["Colognes"][2].item, "None")
            self.assertEqual(bag["Colognes"][2].amount, 0)
        else:
            self.assertEqual(bag["Colognes"][1].item, "None")
            self.assertEqual(bag["Colognes"][1].amount, 0)

            self.assertEqual(bag["Battle CDs"][0].item, "Battle CD 01")
            self.assertEqual(bag["Battle CDs"][0].amount, 5)
            self.assertEqual(bag["Battle CDs"][1].item, "None")
            self.assertEqual(bag["Battle CDs"][1].amount, 0)

        # Make sure removing items through the bag removes from the proper pocket.
        for item in items:
            bag.remove(item, 5)

        self.assertEqual(bag["Items"][0].item, "None")
        self.assertEqual(bag["Items"][0].amount, 0)
        self.assertEqual(bag["Items"][1].item, "None")
        self.assertEqual(bag["Items"][1].amount, 0)

        self.assertEqual(bag["Key Items"][0].item, "None")
        self.assertEqual(bag["Key Items"][0].amount, 0)
        self.assertEqual(bag["Key Items"][1].item, "None")
        self.assertEqual(bag["Key Items"][1].amount, 0)

        self.assertEqual(bag[u"Pok\u00e9 Balls"][0].item, "None")
        self.assertEqual(bag[u"Pok\u00e9 Balls"][0].amount, 0)
        self.assertEqual(bag[u"Pok\u00e9 Balls"][1].item, "None")
        self.assertEqual(bag[u"Pok\u00e9 Balls"][1].amount, 0)

        self.assertEqual(bag["TMs"][0].item, "None")
        self.assertEqual(bag["TMs"][0].amount, 0)
        self.assertEqual(bag["TMs"][1].item, "None")
        self.assertEqual(bag["TMs"][1].amount, 0)
        self.assertEqual(bag["TMs"][2].item, "None")
        self.assertEqual(bag["TMs"][2].amount, 0)

        self.assertEqual(bag["Berries"][0].item, "None")
        self.assertEqual(bag["Berries"][0].amount, 0)
        self.assertEqual(bag["Berries"][1].item, "None")
        self.assertEqual(bag["Berries"][1].amount, 0)

        self.assertEqual(bag["Colognes"][0].item, "None")
        self.assertEqual(bag["Colognes"][0].amount, 0)
        if colosseum:
            self.assertEqual(bag["Colognes"][1].item, "None")
            self.assertEqual(bag["Colognes"][1].amount, 0)
            self.assertEqual(bag["Colognes"][2].item, "None")
            self.assertEqual(bag["Colognes"][2].amount, 0)
        else:
            self.assertEqual(bag["Colognes"][1].item, "None")
            self.assertEqual(bag["Colognes"][1].amount, 0)

            self.assertEqual(bag["Battle CDs"][0].item, "None")
            self.assertEqual(bag["Battle CDs"][0].amount, 0)
            self.assertEqual(bag["Battle CDs"][1].item, "None")
            self.assertEqual(bag["Battle CDs"][1].amount, 0)

    #
    # Colosseum
    #

    def test_colosseum_item_pocket(self):
        item_pocket = pkmn.item_list("Items", "Colosseum")
        self.__test_item_pocket(item_pocket, "Colosseum")

    def test_colosseum_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("Key Items", "Colosseum")
        self.__test_key_item_pocket(key_item_pocket, "Colosseum")

    def test_colosseum_ball_pocket(self):
        ball_pocket = pkmn.item_list(u"Pok\u00e9 Balls", "Colosseum")
        self.__test_ball_pocket(ball_pocket, "Colosseum")

    def test_colosseum_tm_pocket(self):
        tm_pocket = pkmn.item_list("TMs", "Colosseum")
        self.__test_tm_pocket(tm_pocket, "Colosseum")

    def test_colosseum_berry_pocket(self):
        berry_pocket = pkmn.item_list("Berries", "Colosseum")
        self.__test_berry_pocket(berry_pocket, "Colosseum")

    def test_colosseum_cologne_pocket(self):
        cologne_pocket = pkmn.item_list("Colognes", "Colosseum")
        self.__test_cologne_pocket(cologne_pocket, "Colosseum")

    def test_colosseum_pc(self):
        pc = pkmn.item_list("PC", "Colosseum")
        self.__test_pc(pc, "Colosseum")

    def test_colosseum_bag(self):
        bag = pkmn.item_bag("Colosseum")
        self.__test_item_bag(bag, "Colosseum")

    #
    # XD
    #

    def test_xd_item_pocket(self):
        item_pocket = pkmn.item_list("Items", "XD")
        self.__test_item_pocket(item_pocket, "XD")

    def test_xd_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("Key Items", "XD")
        self.__test_key_item_pocket(key_item_pocket, "XD")

    def test_xd_ball_pocket(self):
        ball_pocket = pkmn.item_list(u"Pok\u00e9 Balls", "XD")
        self.__test_ball_pocket(ball_pocket, "XD")

    def test_xd_tm_pocket(self):
        tm_pocket = pkmn.item_list("TMs", "XD")
        self.__test_tm_pocket(tm_pocket, "XD")

    def test_xd_berry_pocket(self):
        berry_pocket = pkmn.item_list("Berries", "XD")
        self.__test_berry_pocket(berry_pocket, "XD")

    def test_xd_cologne_pocket(self):
        cologne_pocket = pkmn.item_list("Colognes", "XD")
        self.__test_cologne_pocket(cologne_pocket, "XD")

    def test_xd_battle_cd_pocket(self):
        battle_cd_pocket = pkmn.item_list("Battle CDs", "XD")
        self.__test_battle_cd_pocket(battle_cd_pocket, "XD")

    def test_xd_pc(self):
        pc = pkmn.item_list("PC", "XD")
        self.__test_pc(pc, "XD")

    def test_xd_bag(self):
        bag = pkmn.item_bag("XD")
        self.__test_item_bag(bag, "XD")
