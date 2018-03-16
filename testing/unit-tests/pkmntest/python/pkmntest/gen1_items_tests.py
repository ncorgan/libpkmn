#!/usr/bin/env python
#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

from .items_tests import items_tests

import unittest

class gen1_items_test(items_tests):

    def setUp(self):
        self.__invalid_generation_items = ["Amulet Coin", "Apicot Berry", "Air Mail",
                                           "Air Balloon", "Aqua Suit"]

    #
    # Helper functions
    #

    def __test_item_list_common(self, items, game):
        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(items)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(items, "Potion")

        # Confirm items from later generations can't be added.
        self.item_class_test_invalid_items(
            items,
            self.__invalid_generation_items
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            items,
            ["Potion", "Great Ball", "Ether", "PP Up",
             "TM34", "Moon Stone", "Bicycle", "Full Heal"]
        )

        full_item_list = pkmn.database.lists.get_item_list(game)
        self.assertEqual(len(items.valid_items), len(full_item_list))
        self.item_list_test_both_text_types(items)

    def __test_item_list(self, items, game):
        # Check unchanging and initial values.
        self.assertEqual(items.name, "Items")
        self.assertEqual(items.game, game)
        self.assertEqual(len(items), 20)
        self.assertEqual(items.num_items, 0)

        self.__test_item_list_common(items, game)

    def __test_item_pc(self, pc, game):
        # Check unchanging and initial values.
        self.assertEqual(pc.name, "PC")
        self.assertEqual(pc.game, game)
        self.assertEqual(len(pc), 50)
        self.assertEqual(pc.num_items, 0)

        self.__test_item_list_common(pc, game)

    def __test_item_bag(self, bag, game):
        # Check unchanging and initial values.
        self.assertEqual(bag.game, game)

        self.assertEqual(len(bag), 1)
        self.assertEqual(len(bag.pocket_names), 1)

        self.assertTrue("Items" in bag.pocket_names)

        self.__test_item_list(bag["Items"], game)
        self.item_bag_test_get_pockets_with_both_text_types(bag)

        # Confirm items from later generations can't be added.
        self.item_class_test_invalid_items(
            bag,
            self.__invalid_generation_items
        )

        items = ["Potion", "Great Ball", "Ether", "PP Up",
                 "TM34", "Moon Stone", "Bicycle", "Full Heal"]

        # Make sure adding items through the bag adds to the pocket.
        self.assertEqual(bag["Items"].num_items, 0)
        for i in range(len(items)):
            bag.add(items[i], i+1)

        for i in range(len(items)):
            self.assertEqual(bag["Items"][i].item, items[i])
            self.assertEqual(bag["Items"][i].amount, i+1)

        self.assertEqual(bag["Items"][8].item, "None")
        self.assertEqual(bag["Items"][8].amount, 0)

        for i in range(len(items)):
            bag.remove(items[i], i+1)

        for i in range(len(items)+1):
            self.assertEqual(bag["Items"][i].item, "None")
            self.assertEqual(bag["Items"][i].amount, 0)

    #
    # Red
    #

    def test_red_item_list(self):
        items = pkmn.item_list("Items", "Red")
        self.__test_item_list(items, "Red")

    def test_red_item_pc(self):
        pc = pkmn.item_list("PC", "Red")
        self.__test_item_pc(pc, "Red")

    def test_red_item_bag(self):
        bag = pkmn.item_bag("Red")
        self.__test_item_bag(bag, "Red")

    #
    # Blue
    #

    def test_blue_item_list(self):
        items = pkmn.item_list("Items", "Blue")
        self.__test_item_list(items, "Blue")

    def test_blue_item_pc(self):
        pc = pkmn.item_list("PC", "Blue")
        self.__test_item_pc(pc, "Blue")

    def test_blue_item_bag(self):
        bag = pkmn.item_bag("Blue")
        self.__test_item_bag(bag, "Blue")

    #
    # Yellow
    #

    def test_yellow_item_list(self):
        items = pkmn.item_list("Items", "Yellow")
        self.__test_item_list(items, "Yellow")

    def test_yellow_item_pc(self):
        pc = pkmn.item_list("PC", "Yellow")
        self.__test_item_pc(pc, "Yellow")

    def test_yellow_item_bag(self):
        bag = pkmn.item_bag("Yellow")
        self.__test_item_bag(bag, "Yellow")
