#!/usr/bin/env python
#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import unittest

class gen1_items_test(unittest.TestCase):

    #
    # Helper functions
    #

    def __test_item_list_common(self, items, game):
        # Make sure item slots start as correcty empty.
        self.assertEqual(len(items.as_list()), items.get_capacity())
        for i in range(items.get_capacity()):
            self.assertEqual(items[i].item.get_name(), "None")
            self.assertEqual(items[i].amount, 0)

        # Confirm errors are thrown when expected.
        with self.assertRaises(IndexError):
            items.add("Potion", 0)
        with self.assertRaises(IndexError):
            items.add("Potion", 100)
        with self.assertRaises(IndexError):
            items.remove("Potion", 0)
        with self.assertRaises(IndexError):
            items.remove("Potion", 100)

        # Start adding and removing stuff, and make sure the numbers are accurate.
        items.add("Potion", 30)
        items.add("Great Ball", 99)
        items.add("Ether", 1)

        self.assertEqual(items[0].item.get_name(), "Potion")
        self.assertEqual(items[0].amount, 30)
        self.assertEqual(items[1].item.get_name(), "Great Ball")
        self.assertEqual(items[1].amount, 99)
        self.assertEqual(items[2].item.get_name(), "Ether")
        self.assertEqual(items[2].amount, 1)
        self.assertEqual(items.get_num_items(), 3)

        items.add("Ether", 15)
        self.assertEqual(items[0].item.get_name(), "Potion")
        self.assertEqual(items[0].amount, 30)
        self.assertEqual(items[1].item.get_name(), "Great Ball")
        self.assertEqual(items[1].amount, 99)
        self.assertEqual(items[2].item.get_name(), "Ether")
        self.assertEqual(items[2].amount, 16)
        self.assertEqual(items.get_num_items(), 3)

        items.remove("Great Ball", 20)
        self.assertEqual(items[0].item.get_name(), "Potion")
        self.assertEqual(items[0].amount, 30)
        self.assertEqual(items[1].item.get_name(), "Great Ball")
        self.assertEqual(items[1].amount, 79)
        self.assertEqual(items[2].item.get_name(), "Ether")
        self.assertEqual(items[2].amount, 16)
        self.assertEqual(items.get_num_items(), 3)

        items.move(0, 1)
        self.assertEqual(items[0].item.get_name(), "Great Ball")
        self.assertEqual(items[0].amount, 79)
        self.assertEqual(items[1].item.get_name(), "Potion")
        self.assertEqual(items[1].amount, 30)
        self.assertEqual(items[2].item.get_name(), "Ether")
        self.assertEqual(items[2].amount, 16)
        self.assertEqual(items.get_num_items(), 3)

        items.remove("Potion", 30)
        self.assertEqual(items[0].item.get_name(), "Great Ball")
        self.assertEqual(items[0].amount, 79)
        self.assertEqual(items[1].item.get_name(), "Ether")
        self.assertEqual(items[1].amount, 16)
        self.assertEqual(items[2].item.get_name(), "None")
        self.assertEqual(items[2].amount, 0)
        self.assertEqual(items.get_num_items(), 2)

        valid_items = items.get_valid_items()
        full_item_list = pkmn.database.get_item_list(game)
        self.assertEqual(len(valid_items), len(full_item_list))

    def __test_item_list(self, items, game):
        # Check unchanging and initial values.
        self.assertEqual(items.get_name(), "Items")
        self.assertEqual(items.get_game(), game)
        self.assertEqual(items.get_capacity(), 20)
        self.assertEqual(items.get_num_items(), 0)

        self.__test_item_list_common(items, game)

    def __test_item_pc(self, pc, game):
        # Check unchanging and initial values.
        self.assertEqual(pc.get_name(), "PC")
        self.assertEqual(pc.get_game(), game)
        self.assertEqual(pc.get_capacity(), 50)
        self.assertEqual(pc.get_num_items(), 0)

        self.__test_item_list_common(pc, game)

    def __test_item_bag(self, bag, game):
        # Check unchanging and initial values.
        self.assertEqual(bag.get_game(), game)

        pockets = bag.get_pockets()
        self.assertEqual(pockets.size(), 1)

        self.__test_item_list(pockets["Items"], game)

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
