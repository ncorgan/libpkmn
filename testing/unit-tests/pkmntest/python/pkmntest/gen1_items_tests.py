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
        self.__valid_items = [
            pkmn.item.POTION,
            pkmn.item.GREAT_BALL,
            pkmn.item.ETHER,
            pkmn.item.PP_UP,
            pkmn.item.TM34,
            pkmn.item.MOON_STONE,
            pkmn.item.BICYCLE,
            pkmn.item.FULL_HEAL
        ]
        self.__invalid_generation_items = [
            pkmn.item.AMULET_COIN,
            pkmn.item.APICOT_BERRY,
            pkmn.item.AIR_MAIL,
            pkmn.item.AIR_BALLOON,
            pkmn.item.AQUA_SUIT
        ]

    #
    # Helper functions
    #

    def __test_item_list_common(self, items, game):
        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(items)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(items, pkmn.item.POTION)

        # Confirm items from later generations can't be added.
        self.item_class_test_invalid_items(
            items,
            self.__invalid_generation_items
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            items,
            self.__valid_items
        )

        full_item_list = pkmn.database.lists.get_item_list(game)
        self.assertEqual(len(items.valid_items), len(full_item_list))

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

        # Make sure adding items through the bag adds to the pocket.
        self.assertEqual(bag["Items"].num_items, 0)
        for i in range(len(self.__valid_items)):
            bag.add(self.__valid_items[i], i+1)

        for i in range(len(self.__valid_items)):
            self.assertEqual(bag["Items"][i].item, self.__valid_items[i])
            self.assertEqual(bag["Items"][i].amount, i+1)

        self.assertEqual(bag["Items"][8].item, pkmn.item.NONE)
        self.assertEqual(bag["Items"][8].amount, 0)

        for i in range(len(self.__valid_items)):
            bag.remove(self.__valid_items[i], i+1)

        for i in range(len(self.__valid_items)+1):
            self.assertEqual(bag["Items"][i].item, pkmn.item.NONE)
            self.assertEqual(bag["Items"][i].amount, 0)

    #
    # Red
    #

    def test_red_item_list(self):
        items = pkmn.item_list("Items", pkmn.game.RED)
        self.__test_item_list(items, pkmn.game.RED)
'''
    def test_red_item_pc(self):
        pc = pkmn.item_list("PC", pkmn.game.RED)
        self.__test_item_pc(pc, pkmn.game.RED)

    def test_red_item_bag(self):
        bag = pkmn.item_bag(pkmn.game.RED)
        self.__test_item_bag(bag, pkmn.game.RED)

    #
    # Blue
    #

    def test_blue_item_list(self):
        items = pkmn.item_list("Items", pkmn.game.BLUE)
        self.__test_item_list(items, pkmn.game.BLUE)

    def test_blue_item_pc(self):
        pc = pkmn.item_list("PC", pkmn.game.BLUE)
        self.__test_item_pc(pc, pkmn.game.BLUE)

    def test_blue_item_bag(self):
        bag = pkmn.item_bag(pkmn.game.BLUE)
        self.__test_item_bag(bag, pkmn.game.BLUE)

    #
    # Yellow
    #

    def test_yellow_item_list(self):
        items = pkmn.item_list("Items", pkmn.game.YELLOW)
        self.__test_item_list(items, pkmn.game.YELLOW)

    def test_yellow_item_pc(self):
        pc = pkmn.item_list("PC", pkmn.game.YELLOW)
        self.__test_item_pc(pc, pkmn.game.YELLOW)

    def test_yellow_item_bag(self):
        bag = pkmn.item_bag(pkmn.game.YELLOW)
        self.__test_item_bag(bag, pkmn.game.YELLOW)
        '''
