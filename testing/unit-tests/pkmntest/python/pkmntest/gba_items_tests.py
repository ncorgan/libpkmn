#!/usr/bin/env python
#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

from .items_tests import items_tests

import sys
import unittest

PYTHON_MAJOR_VERSION = sys.version_info[0]

class gba_items_test(items_tests):

    def setUp(self):
        self.__all_pocket_item_names = ["Potion", "Mach Bike", "Great Ball", "TM01",
                                        "Aspear Berry", "Wailmer Pail", "Master Ball", "HM04"]
        self.__wrong_game_all_pocket_items = ["Pink Bow", "Black Sludge",
                                              "Ein File S", "Gonzap's Key",
                                              "GS Ball", "Poffin Items",
                                              "TM51",
                                              "Berry", "Occa Berry"]

    #
    # Helper functions
    #

    def __test_item_pocket(self, item_pocket, game):
        if game in ["Ruby", "Sapphire"]:
            expected_capacity = 20
        elif game == "Emerald":
            expected_capacity = 30
        else:
            expected_capacity = 42

        # Check unchanging and initial values.
        self.assertEqual(item_pocket.name, "Items")
        self.assertEqual(item_pocket.game, game)
        self.assertEqual(len(item_pocket), expected_capacity)
        self.assertEqual(item_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(item_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(item_pocket, "Potion")

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            item_pocket,
            ["Bicycle", "Master Ball", "HM01", "Razz Berry"]
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            item_pocket,
            ["Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"]
        )

        # Make sure we can't add items from Gamecube games.
        self.item_class_test_invalid_items(
            item_pocket,
            ["Time Flute", u"Pok\u00e9 Snack"]
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
        if game in ["Ruby", "Sapphire"]:
            expected_capacity = 20
        else:
            expected_capacity = 30

        # Check unchanging and initial values.
        self.assertEqual(key_item_pocket.name, "Key Items")
        self.assertEqual(key_item_pocket.game, game)
        self.assertEqual(len(key_item_pocket), expected_capacity)
        self.assertEqual(key_item_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(key_item_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(key_item_pocket, "Basement Key")

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            key_item_pocket,
            ["Potion", "Master Ball", "HM01", "Razz Berry"]
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            key_item_pocket,
            ["GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"]
        )

        # Make sure we can't add items from Gamecube games.
        self.item_class_test_invalid_items(
            key_item_pocket,
            ["Ein File S", "Powerup Part",
             "Gonzap's Key", "Krane Memo 1"]
        )
        if game == "Ruby" or game == "Sapphire":
            self.item_class_test_invalid_items(
                key_item_pocket,
                ["Helix Fossil", "Tea", "Ruby"]
            )
        if game != "Emerald":
            self.item_class_test_invalid_items(
                key_item_pocket,
                ["Magma Emblem", "Old Sea Map"]
            )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            key_item_pocket,
            [u"Wailmer Pail", u"Basement Key", u"Meteorite", u"Old Rod",
             u"Red Orb", u"Root Fossil", u"Contest Pass", u"Eon Ticket"]
        )

        valid_items = key_item_pocket.valid_items
        self.assertGreater(len(valid_items), 0)
        self.item_list_test_both_text_types(key_item_pocket)

    def __test_ball_pocket(self, ball_pocket, game):
        if game in ["FireRed", "LeafGreen"]:
            expected_capacity = 13
        else:
            expected_capacity = 16

        # Check unchanging and initial values.
        self.assertStringEqual(ball_pocket.name, u"Pok\u00e9 Balls")
        self.assertEqual(ball_pocket.game, game)
        self.assertEqual(len(ball_pocket), expected_capacity)
        self.assertEqual(ball_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(ball_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(ball_pocket, "Master Ball")

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            ball_pocket,
            ["Potion", "Bicycle", "HM01", "Razz Berry"]
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

    def __test_tmhm_pocket(self, tmhm_pocket, game):
        if game in ["FireRed", "LeafGreen"]:
            expected_capacity = 58
            pocket_name = "TM Case"
        else:
            expected_capacity = 64
            pocket_name = "TMs & HMs"

        # Check unchanging and initial values.
        self.assertEqual(tmhm_pocket.name, pocket_name)
        self.assertEqual(tmhm_pocket.game, game)
        self.assertEqual(len(tmhm_pocket), expected_capacity)
        self.assertEqual(tmhm_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(tmhm_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(tmhm_pocket, "TM01")

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            tmhm_pocket,
            ["Potion", "Master Ball", "Great Ball", "Razz Berry"]
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            tmhm_pocket,
            ["TM51"]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            tmhm_pocket,
            ["TM01", "HM01", "TM02", "HM02",
             "TM03", "HM03", "TM04", "HM04"]
        )

        valid_items = tmhm_pocket.valid_items
        self.assertGreater(len(valid_items), 0)
        self.item_list_test_both_text_types(tmhm_pocket)

    def __test_berry_pocket(self, berry_pocket, game):
        if game in ["FireRed", "LeafGreen"]:
            expected_capacity = 43
            pocket_name = "Berry Pouch"
        else:
            expected_capacity = 46
            pocket_name = "Berries"

        # Check unchanging and initial values.
        self.assertEqual(berry_pocket.name, pocket_name)
        self.assertEqual(berry_pocket.game, game)
        self.assertEqual(len(berry_pocket), expected_capacity)
        self.assertEqual(berry_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(berry_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(berry_pocket, "Razz Berry")

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            berry_pocket,
            ["Potion", "Bicycle", "Great Ball", "HM02"]
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

    def __test_pc(self, pc, game):
        # Check unchanging and initial values.
        self.assertEqual(pc.name, "PC")
        self.assertEqual(pc.game, game)
        self.assertEqual(len(pc), 50)
        self.assertEqual(pc.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(pc)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(pc, "Potion")

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            pc,
            self.__wrong_game_all_pocket_items
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            pc,
            [u"Potion", u"Mach Bike", u"Great Ball", u"TM01",
             u"Aspear Berry", u"Wailmer Pail", u"Master Ball", u"HM04"]
        )

        valid_items = pc.valid_items
        self.assertGreater(len(valid_items), 0)
        self.item_list_test_both_text_types(pc)

    def __test_item_bag(self, bag, game):
        if game in ["FireRed", "LeafGreen"]:
            tmhm_pocket_name = "TM Case"
            berry_pocket_name = "Berry Pouch"
        else:
            tmhm_pocket_name = "TMs & HMs"
            berry_pocket_name = "Berries"
        ball_pocket_name = u"Pok\u00e9 Balls"

        # Check unchanging and initial values.
        self.assertEqual(bag.game, game)

        self.assertEqual(len(bag), 5)
        self.assertEqual(len(bag.pocket_names), 5)

        unicode_pocket_names = []
        if PYTHON_MAJOR_VERSION == 2:
            unicode_pocket_names = [pocket_name.decode("utf-8") for pocket_name in bag.pocket_names]
        else:
            unicode_pocket_names = [unicode(pocket_name) for pocket_name in bag.pocket_names]

        self.assertTrue("Items" in unicode_pocket_names)
        self.assertTrue("Key Items" in unicode_pocket_names)
        self.assertTrue(ball_pocket_name in unicode_pocket_names)
        self.assertTrue(tmhm_pocket_name in unicode_pocket_names)
        self.assertTrue(berry_pocket_name in unicode_pocket_names)

        self.__test_item_pocket(bag["Items"], game)
        self.__test_key_item_pocket(bag["Key Items"], game)
        self.__test_ball_pocket(bag[ball_pocket_name], game)
        self.__test_tmhm_pocket(bag[tmhm_pocket_name], game)
        self.__test_berry_pocket(bag[berry_pocket_name], game)
        self.item_bag_test_get_pockets_with_both_text_types(bag)

        items = [u"Potion", u"Mach Bike", u"Great Ball", u"TM01",
                 u"Aspear Berry", u"Wailmer Pail", u"Master Ball", u"HM04"]

        # Make sure adding items through the bag adds to the proper pocket.
        self.assertEqual(bag["Items"].num_items, 0)
        self.assertEqual(bag["Key Items"].num_items, 0)
        self.assertEqual(bag[ball_pocket_name].num_items, 0)
        self.assertEqual(bag[tmhm_pocket_name].num_items, 0)
        self.assertEqual(bag[berry_pocket_name].num_items, 0)
        for item in items:
            bag.add(item, 5)

        self.assertEqual(bag["Items"][0].item, "Potion")
        self.assertEqual(bag["Items"][0].amount, 5)
        self.assertEqual(bag["Items"][1].item, "None")
        self.assertEqual(bag["Items"][1].amount, 0)

        self.assertEqual(bag["Key Items"][0].item, "Mach Bike")
        self.assertEqual(bag["Key Items"][0].amount, 5)
        self.assertEqual(bag["Key Items"][1].item, "Wailmer Pail")
        self.assertEqual(bag["Key Items"][1].amount, 5)
        self.assertEqual(bag["Key Items"][2].item, "None")
        self.assertEqual(bag["Key Items"][2].amount, 0)

        self.assertEqual(bag[ball_pocket_name][0].item, "Great Ball")
        self.assertEqual(bag[ball_pocket_name][0].amount, 5)
        self.assertEqual(bag[ball_pocket_name][1].item, "Master Ball")
        self.assertEqual(bag[ball_pocket_name][1].amount, 5)
        self.assertEqual(bag[ball_pocket_name][2].item, "None")
        self.assertEqual(bag[ball_pocket_name][2].amount, 0)

        self.assertEqual(bag[tmhm_pocket_name][0].item, "TM01")
        self.assertEqual(bag[tmhm_pocket_name][0].amount, 5)
        self.assertEqual(bag[tmhm_pocket_name][1].item, "HM04")
        self.assertEqual(bag[tmhm_pocket_name][1].amount, 5)
        self.assertEqual(bag[tmhm_pocket_name][2].item, "None")
        self.assertEqual(bag[tmhm_pocket_name][2].amount, 0)

        self.assertEqual(bag[berry_pocket_name][0].item, "Aspear Berry")
        self.assertEqual(bag[berry_pocket_name][0].amount, 5)
        self.assertEqual(bag[berry_pocket_name][1].item, "None")
        self.assertEqual(bag[berry_pocket_name][1].amount, 0)

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
        self.assertEqual(bag["Key Items"][2].item, "None")
        self.assertEqual(bag["Key Items"][2].amount, 0)

        self.assertEqual(bag[ball_pocket_name][0].item, "None")
        self.assertEqual(bag[ball_pocket_name][0].amount, 0)
        self.assertEqual(bag[ball_pocket_name][1].item, "None")
        self.assertEqual(bag[ball_pocket_name][1].amount, 0)
        self.assertEqual(bag[ball_pocket_name][2].item, "None")
        self.assertEqual(bag[ball_pocket_name][2].amount, 0)

        self.assertEqual(bag[tmhm_pocket_name][0].item, "None")
        self.assertEqual(bag[tmhm_pocket_name][0].amount, 0)
        self.assertEqual(bag[tmhm_pocket_name][1].item, "None")
        self.assertEqual(bag[tmhm_pocket_name][1].amount, 0)
        self.assertEqual(bag[tmhm_pocket_name][2].item, "None")
        self.assertEqual(bag[tmhm_pocket_name][2].amount, 0)

        self.assertEqual(bag[berry_pocket_name][0].item, "None")
        self.assertEqual(bag[berry_pocket_name][0].amount, 0)
        self.assertEqual(bag[berry_pocket_name][1].item, "None")
        self.assertEqual(bag[berry_pocket_name][1].amount, 0)

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            bag,
            self.__wrong_game_all_pocket_items
        )

    #
    # Ruby
    #

    def test_ruby_item_pocket(self):
        item_pocket = pkmn.item_list("Items", "Ruby")
        self.__test_item_pocket(item_pocket, "Ruby")

    def test_ruby_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("Key Items", "Ruby")
        self.__test_key_item_pocket(key_item_pocket, "Ruby")

    def test_ruby_ball_pocket(self):
        ball_pocket = pkmn.item_list(u"Pok\u00e9 Balls", "Ruby")
        self.__test_ball_pocket(ball_pocket, "Ruby")

    def test_ruby_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TMs & HMs", "Ruby")
        self.__test_tmhm_pocket(tmhm_pocket, "Ruby")

    def test_ruby_berry_pocket(self):
        berry_pocket = pkmn.item_list("Berries", "Ruby")
        self.__test_berry_pocket(berry_pocket, "Ruby")

    def test_ruby_pc(self):
        pc = pkmn.item_list("PC", "Ruby")
        self.__test_pc(pc, "Ruby")

    def test_ruby_item_bag(self):
        item_bag = pkmn.item_bag("Ruby")
        self.__test_item_bag(item_bag, "Ruby")

    #
    # Sapphire
    #

    def test_sapphire_item_pocket(self):
        item_pocket = pkmn.item_list("Items", "Sapphire")
        self.__test_item_pocket(item_pocket, "Sapphire")

    def test_sapphire_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("Key Items", "Sapphire")
        self.__test_key_item_pocket(key_item_pocket, "Sapphire")

    def test_sapphire_ball_pocket(self):
        ball_pocket = pkmn.item_list(u"Pok\u00e9 Balls", "Sapphire")
        self.__test_ball_pocket(ball_pocket, "Sapphire")

    def test_sapphire_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TMs & HMs", "Sapphire")
        self.__test_tmhm_pocket(tmhm_pocket, "Sapphire")

    def test_sapphire_berry_pocket(self):
        berry_pocket = pkmn.item_list("Berries", "Sapphire")
        self.__test_berry_pocket(berry_pocket, "Sapphire")

    def test_sapphire_pc(self):
        pc = pkmn.item_list("PC", "Sapphire")
        self.__test_pc(pc, "Sapphire")

    def test_sapphire_item_bag(self):
        item_bag = pkmn.item_bag("Sapphire")
        self.__test_item_bag(item_bag, "Sapphire")

    #
    # Emerald
    #

    def test_emerald_item_pocket(self):
        item_pocket = pkmn.item_list("Items", "Emerald")
        self.__test_item_pocket(item_pocket, "Emerald")

    def test_emerald_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("Key Items", "Emerald")
        self.__test_key_item_pocket(key_item_pocket, "Emerald")

    def test_emerald_ball_pocket(self):
        ball_pocket = pkmn.item_list(u"Pok\u00e9 Balls", "Emerald")
        self.__test_ball_pocket(ball_pocket, "Emerald")

    def test_emerald_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TMs & HMs", "Emerald")
        self.__test_tmhm_pocket(tmhm_pocket, "Emerald")

    def test_emerald_berry_pocket(self):
        berry_pocket = pkmn.item_list("Berries", "Emerald")
        self.__test_berry_pocket(berry_pocket, "Emerald")

    def test_emerald_pc(self):
        pc = pkmn.item_list("PC", "Emerald")
        self.__test_pc(pc, "Emerald")

    def test_emerald_item_bag(self):
        item_bag = pkmn.item_bag("Emerald")
        self.__test_item_bag(item_bag, "Emerald")

    #
    # FireRed
    #

    def test_firered_item_pocket(self):
        item_pocket = pkmn.item_list("Items", "FireRed")
        self.__test_item_pocket(item_pocket, "FireRed")

    def test_firered_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("Key Items", "FireRed")
        self.__test_key_item_pocket(key_item_pocket, "FireRed")

    def test_firered_ball_pocket(self):
        ball_pocket = pkmn.item_list(u"Pok\u00e9 Balls", "FireRed")
        self.__test_ball_pocket(ball_pocket, "FireRed")

    def test_firered_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TM Case", "FireRed")
        self.__test_tmhm_pocket(tmhm_pocket, "FireRed")

    def test_firered_berry_pocket(self):
        berry_pocket = pkmn.item_list("Berry Pouch", "FireRed")
        self.__test_berry_pocket(berry_pocket, "FireRed")

    def test_firered_pc(self):
        pc = pkmn.item_list("PC", "FireRed")
        self.__test_pc(pc, "FireRed")

    def test_firered_item_bag(self):
        item_bag = pkmn.item_bag("FireRed")
        self.__test_item_bag(item_bag, "FireRed")

    #
    # LeafGreen
    #

    def test_leafgreen_item_pocket(self):
        item_pocket = pkmn.item_list("Items", "LeafGreen")
        self.__test_item_pocket(item_pocket, "LeafGreen")

    def test_leafgreen_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("Key Items", "LeafGreen")
        self.__test_key_item_pocket(key_item_pocket, "LeafGreen")

    def test_leafgreen_ball_pocket(self):
        ball_pocket = pkmn.item_list(u"Pok\u00e9 Balls", "LeafGreen")
        self.__test_ball_pocket(ball_pocket, "LeafGreen")

    def test_leafgreen_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TM Case", "LeafGreen")
        self.__test_tmhm_pocket(tmhm_pocket, "LeafGreen")

    def test_leafgreen_berry_pocket(self):
        berry_pocket = pkmn.item_list("Berry Pouch", "LeafGreen")
        self.__test_berry_pocket(berry_pocket, "LeafGreen")

    def test_leafgreen_pc(self):
        pc = pkmn.item_list("PC", "LeafGreen")
        self.__test_pc(pc, "LeafGreen")

    def test_leafgreen_item_bag(self):
        item_bag = pkmn.item_bag("LeafGreen")
        self.__test_item_bag(item_bag, "LeafGreen")
