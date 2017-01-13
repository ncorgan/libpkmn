#!/usr/bin/env python
#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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
        self.assertEqual(len(items.as_list()), items.get_capacity())

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

        valid_items = items.get_valid_items()
        full_item_list = pkmn.database.get_item_list(game)
        self.assertEqual(len(valid_items), len(full_item_list))
        self.item_list_test_both_text_types(items)

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
        self.assertTrue(pockets.has_key("Items"))

        self.__test_item_list(pockets["Items"], game)
        self.item_bag_test_get_pockets_with_both_text_types(bag)

        # Confirm items from later generations can't be added.
        self.item_class_test_invalid_items(
            bag,
            self.__invalid_generation_items
        )

        items = ["Potion", "Great Ball", "Ether", "PP Up",
                 "TM34", "Moon Stone", "Bicycle", "Full Heal"]

        # Make sure adding items through the bag adds to the pocket.
        self.assertEqual(pockets["Items"].get_num_items(), 0)
        for i in range(len(items)):
            bag.add(items[i], i+1)

        for i in range(len(items)):
            self.assertEqual(pockets["Items"][i].item.get_name(), items[i])
            self.assertEqual(pockets["Items"][i].amount, i+1)

        self.assertEqual(pockets["Items"][8].item.get_name(), "None")
        self.assertEqual(pockets["Items"][8].amount, 0)

        for i in range(len(items)):
            bag.remove(items[i], i+1)

        for i in range(len(items)+1):
            self.assertEqual(pockets["Items"][i].item.get_name(), "None")
            self.assertEqual(pockets["Items"][i].amount, 0)

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
