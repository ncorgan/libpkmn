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

class gen2_items_test(items_tests):

    def setUp(self):
        self.__crystal_items = [
            pkmn.item.CLEAR_BELL,
            pkmn.item.GS_BALL,
            pkmn.item.BLUE_CARD,
            pkmn.item.EGG_TICKET
        ]
        self.__wrong_generation_all_pocket_items = [
            pkmn.item.BLACK_SLUDGE,
            pkmn.item.POFFIN_CASE,
            pkmn.item.TM51,
            pkmn.item.OCCA_BERRY
        ]

    #
    # Helper functions
    #

    def __test_item_pocket(self, item_pocket, game):
        # Check unchanging and initial values.
        self.assertEqual(len(item_pocket), 20)
        self.assertEqual(item_pocket.name, "Items")
        self.assertEqual(item_pocket.game, game)
        self.assertEqual(item_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(item_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(item_pocket, pkmn.item.POTION)

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            item_pocket,
            [pkmn.item.BICYCLE, pkmn.item.MASTER_BALL, pkmn.item.HM01]
        )

        # Make sure we can't add items from later generations.
        self.item_class_test_invalid_items(
            item_pocket,
            [pkmn.item.BLACK_FLUTE, pkmn.item.BLACK_SLUDGE,
             pkmn.item.BINDING_BAND, pkmn.item.BEEDRILLITE]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            item_pocket,
            [pkmn.item.POTION, pkmn.item.HP_UP,
             pkmn.item.WHITE_APRICORN, pkmn.item.LUCKY_EGG,
             pkmn.item.FLOWER_MAIL, pkmn.item.BURN_HEAL,
             pkmn.item.PSNCUREBERRY, pkmn.item.BERRY]
        )

        self.assertGreater(len(item_pocket.valid_items), 0)
        self.assertEqual(len(item_pocket.valid_items), len(item_pocket.valid_item_names))

    def __test_key_item_pocket(self, key_item_pocket, game):
        # Check unchanging and initial values.
        self.assertEqual(key_item_pocket.name, "KeyItems")
        self.assertEqual(key_item_pocket.game, game)
        self.assertEqual(len(key_item_pocket), 25)
        self.assertEqual(key_item_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(key_item_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(key_item_pocket, pkmn.item.BICYCLE)

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            key_item_pocket,
            [pkmn.item.POTION, pkmn.item.MASTER_BALL, pkmn.item.HM01]
        )

        # Make sure we can't add items from later generations.
        self.item_class_test_invalid_items(
            key_item_pocket,
            [pkmn.item.MACH_BIKE, pkmn.item.JADE_ORB,
             pkmn.item.LIGHT_STONE, pkmn.item.AQUA_SUIT]
        )

        # Crystal-specific items
        if game == pkmn.game.CRYSTAL:
            for item in self.__crystal_items:
                key_item_pocket.add(item, 1)
                key_item_pocket.remove(item, 1)
            self.assertEqual(key_item_pocket.num_items, 0)
        else:
            self.item_class_test_invalid_items(
                key_item_pocket,
                self.__crystal_items
            )

        # Make sure we can't add or remove more than a single item.
        with self.assertRaises(IndexError):
            key_item_pocket.add(pkmn.item.BICYCLE, 5)
        with self.assertRaises(IndexError):
            key_item_pocket.remove(pkmn.item.BICYCLE, 5)

        # Start adding and removing stuff, and make sure the numbers are accurate.
        items = [pkmn.item.BICYCLE, pkmn.item.BASEMENT_KEY,
                 pkmn.item.SECRETPOTION, pkmn.item.MYSTERY_EGG,
                 pkmn.item.SILVER_WING, pkmn.item.LOST_ITEM,
                 pkmn.item.SQUIRT_BOTTLE, pkmn.item.RAINBOW_WING]
        for item_index in range(len(items)):
            if item_index < 4:
                key_item_pocket.add(items[item_index], 1)
            else:
                key_item_pocket[item_index].item = items[item_index]

        key_item_pocket.remove(items[2], 1)
        key_item_pocket[2].item = pkmn.item.NONE

        while key_item_pocket[0].item != pkmn.item.NONE:
            key_item_pocket[0].item = pkmn.item.NONE

        self.assertGreater(len(key_item_pocket.valid_items), 0)
        self.assertEqual(len(key_item_pocket.valid_items), len(key_item_pocket.valid_item_names))

    def __test_ball_pocket(self, ball_pocket, game):
        # Check unchanging and initial values.
        self.assertEqual(ball_pocket.name, "Balls")
        self.assertEqual(ball_pocket.game, game)
        self.assertEqual(len(ball_pocket), 12)
        self.assertEqual(ball_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(ball_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(ball_pocket, pkmn.item.MASTER_BALL)

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            ball_pocket,
            [pkmn.item.POTION, pkmn.item.BICYCLE, pkmn.item.HM01]
        )

        # Make sure we can't add items from later generations.
        self.item_class_test_invalid_items(
            ball_pocket,
            [pkmn.ball.PREMIER_BALL, pkmn.item.HEAL_BALL, pkmn.item.DREAM_BALL]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        items = [pkmn.item.GREAT_BALL, pkmn.item.POKE_BALL,
                 pkmn.item.PARK_BALL, pkmn.item.FAST_BALL,
                 pkmn.item.MASTER_BALL, pkmn.item.FRIEND_BALL,
                 pkmn.item.LOVE_BALL, pkmn.item.LEVEL_BALL]
        self.item_list_test_add_remove(
            ball_pocket,
            items
        )

        self.assertGreater(len(ball_pocket.valid_items), 0)
        self.assertEqual(len(ball_pocket.valid_items), len(ball_pocket.valid_item_names))

    def __test_tmhm_pocket(self, tmhm_pocket, game):
        # Check unchanging and initial values.
        self.assertEqual(tmhm_pocket.name, "TM/HM")
        self.assertEqual(tmhm_pocket.game, game)
        self.assertEqual(len(tmhm_pocket), 57)
        self.assertEqual(tmhm_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        for i in range(50):
            item = pkmn.item.TM01 + i
            self.assertEqual(item, tmhm_pocket[i].item)
            self.assertEqual(0, tmhm_pocket[i].amount)
        for i in range(7):
            item = pkmn.item.HM01 + i
            self.assertEqual(item, tmhm_pocket[50+i].item)
            self.assertEqual(0, tmhm_pocket[50+i].amount)

        # Confirm errors are thrown when expected.
        with self.assertRaises(IndexError):
            tmhm_pocket.add(pkmn.item.TM10, 0)
        with self.assertRaises(IndexError):
            tmhm_pocket.add(pkmn.item.TM10, 100)
        with self.assertRaises(IndexError):
            tmhm_pocket.remove(pkmn.item.TM10, 0)
        with self.assertRaises(IndexError):
            tmhm_pocket.remove(pkmn.item.TM10, 100)

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            tmhm_pocket,
            [pkmn.item.POTION, pkmn.item.MASTER_BALL, pkmn.item.BICYCLE]
        )

        # Make sure we can't add items from later generations.
        self.item_class_test_invalid_items(
            tmhm_pocket,
            [pkmn.item.TM51]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        for i in range(50):
            item = pkmn.item.TM01 + i
            tmhm_pocket.add(item, 50)
            self.assertEqual(tmhm_pocket.num_items, i+1)
            self.assertEqual(tmhm_pocket[i].item, item)
            self.assertEqual(tmhm_pocket[i].amount, 50)
        for i in range(50)[::-1]:
            item = pkmn.item.TM01 + i
            tmhm_pocket.remove(item, 50)
            self.assertEqual(tmhm_pocket.num_items, i)
            self.assertEqual(tmhm_pocket[i].item, item)
            self.assertEqual(tmhm_pocket[i].amount, 0)
        for i in range(7):
            item = pkmn.item.HM01 + i
            tmhm_pocket.add(item, 1)
            self.assertEqual(tmhm_pocket.num_items, i+1)
            self.assertEqual(tmhm_pocket[50+i].item, item)
            self.assertEqual(tmhm_pocket[50+i].amount, 1)
        for i in range(7)[::-1]:
            item = pkmn.item.HM01 + i
            tmhm_pocket.remove(item, 1)
            self.assertEqual(tmhm_pocket.num_items, i)
            self.assertEqual(tmhm_pocket[50+i].item, item)
            self.assertEqual(tmhm_pocket[50+i].amount, 0)

        self.assertEqual(len(tmhm_pocket.valid_items), 57)
        self.assertEqual(len(tmhm_pocket.valid_item_names), 57)

    def __test_pc(self, pc, game):
        # Check unchanging and initial values.
        self.assertEqual(pc.name, "PC")
        self.assertEqual(pc.game, game)
        self.assertEqual(len(pc), 50)
        self.assertEqual(pc.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(pc)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(pc, pkmn.item.POTION)

        # Crystal-specific items
        if game == pkmn.game.CRYSTAL:
            for item in self.__crystal_items:
                pc.add(item, 1)
                pc.remove(item, 1)
            self.assertEqual(pc.num_items, 0)
        else:
            self.item_class_test_invalid_items(
                pc,
                self.__crystal_items
            )

        # Make sure we can't add items from later generations.
        self.item_class_test_invalid_items(
            pc,
            self.__wrong_generation_all_pocket_items
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            pc,
            [pkmn.item.POTION, pkmn.item.BICYCLE, pkmn.item.GREAT_BALL, pkmn.item.TM28,
             pkmn.item.BERRY, pkmn.item.SQUIRT_BOTTLE, pkmn.item.FRIEND_BALL, pkmn.item.HM01]
        )

        full_item_list = pkmn.database.lists.get_item_list(game)
        self.assertEqual(len(pc.valid_items), len(full_item_list))
        self.assertEqual(len(pc.valid_items), len(pc.valid_item_names))

    def __test_item_bag(self, bag, game):
        # Check unchanging and initial values.
        self.assertEqual(bag.game, game)

        self.assertEqual(len(bag), 4)
        self.assertEqual(len(bag.pocket_names), 4)
        self.item_bag_test_get_pockets_with_both_text_types(bag)

        self.assertTrue("Items" in bag.pocket_names)
        self.assertTrue("KeyItems" in bag.pocket_names)
        self.assertTrue("Balls" in bag.pocket_names)
        self.assertTrue("TM/HM" in bag.pocket_names)

        self.__test_item_pocket(bag["Items"], game)
        self.__test_key_item_pocket(bag["KeyItems"], game)
        self.__test_ball_pocket(bag["Balls"], game)
        self.__test_tmhm_pocket(bag["TM/HM"], game)

        items = [pkmn.item.POTION, pkmn.item.BICYCLE, pkmn.item.GREAT_BALL, pkmn.item.TM28,
                 pkmn.item.BERRY, pkmn.item.SQUIRT_BOTTLE, pkmn.item.FRIEND_BALL, pkmn.item.HM01]

        # Make sure adding items through the bag adds to the proper pockets.
        self.assertEqual(bag["Items"].num_items, 0)
        self.assertEqual(bag["KeyItems"].num_items, 0)
        self.assertEqual(bag["Balls"].num_items, 0)
        self.assertEqual(bag["TM/HM"].num_items, 0)
        for item in items:
            bag.add(item, 1)

        self.assertEqual(bag["Items"][0].item, pkmn.item.POTION)
        self.assertEqual(bag["Items"][0].amount, 1)
        self.assertEqual(bag["Items"][1].item, pkmn.item.BERRY)
        self.assertEqual(bag["Items"][1].amount, 1)
        self.assertEqual(bag["Items"][2].item, pkmn.item.NONE)
        self.assertEqual(bag["Items"][2].amount, 0)

        self.assertEqual(bag["KeyItems"][0].item, pkmn.item.BICYCLE)
        self.assertEqual(bag["KeyItems"][0].amount, 1)
        self.assertEqual(bag["KeyItems"][1].item, pkmn.item.SQUIRT_BOTTLE)
        self.assertEqual(bag["KeyItems"][1].amount, 1)
        self.assertEqual(bag["KeyItems"][2].item, pkmn.item.NONE)
        self.assertEqual(bag["KeyItems"][2].amount, 0)

        self.assertEqual(bag["Balls"][0].item, pkmn.item.GREAT_BALL)
        self.assertEqual(bag["Balls"][0].amount, 1)
        self.assertEqual(bag["Balls"][1].item, pkmn.item.FRIEND_BALL)
        self.assertEqual(bag["Balls"][1].amount, 1)
        self.assertEqual(bag["Balls"][2].item, pkmn.item.NONE)
        self.assertEqual(bag["Balls"][2].amount, 0)

        self.assertEqual(bag["TM/HM"][0].item, pkmn.item.TM01)
        self.assertEqual(bag["TM/HM"][0].amount, 0)
        self.assertEqual(bag["TM/HM"][1].item, pkmn.item.TM02)
        self.assertEqual(bag["TM/HM"][1].amount, 0)
        self.assertEqual(bag["TM/HM"][27].item, pkmn.item.TM28)
        self.assertEqual(bag["TM/HM"][27].amount, 1)
        self.assertEqual(bag["TM/HM"][50].item, pkmn.item.HM01)
        self.assertEqual(bag["TM/HM"][50].amount, 1)

        # Make sure removing items through the bag removes from the proper pockets.
        for item in items:
            bag.remove(item, 1)

        self.assertEqual(bag["Items"][0].item, pkmn.item.NONE)
        self.assertEqual(bag["Items"][0].amount, 0)
        self.assertEqual(bag["Items"][1].item, pkmn.item.NONE)
        self.assertEqual(bag["Items"][1].amount, 0)
        self.assertEqual(bag["Items"][2].item, pkmn.item.NONE)
        self.assertEqual(bag["Items"][2].amount, 0)

        self.assertEqual(bag["KeyItems"][0].item, pkmn.item.NONE)
        self.assertEqual(bag["KeyItems"][0].amount, 0)
        self.assertEqual(bag["KeyItems"][1].item, pkmn.item.NONE)
        self.assertEqual(bag["KeyItems"][1].amount, 0)
        self.assertEqual(bag["KeyItems"][2].item, pkmn.item.NONE)
        self.assertEqual(bag["KeyItems"][2].amount, 0)

        self.assertEqual(bag["Balls"][0].item, pkmn.item.NONE)
        self.assertEqual(bag["Balls"][0].amount, 0)
        self.assertEqual(bag["Balls"][1].item, pkmn.item.NONE)
        self.assertEqual(bag["Balls"][1].amount, 0)
        self.assertEqual(bag["Balls"][2].item, pkmn.item.NONE)
        self.assertEqual(bag["Balls"][2].amount, 0)

        self.assertEqual(bag["TM/HM"][0].item, pkmn.item.TM01)
        self.assertEqual(bag["TM/HM"][0].amount, 0)
        self.assertEqual(bag["TM/HM"][1].item, pkmn.item.TM02)
        self.assertEqual(bag["TM/HM"][1].amount, 0)
        self.assertEqual(bag["TM/HM"][27].item, pkmn.item.TM28)
        self.assertEqual(bag["TM/HM"][27].amount, 0)
        self.assertEqual(bag["TM/HM"][50].item, pkmn.item.HM01)
        self.assertEqual(bag["TM/HM"][50].amount, 0)

        # Make sure we can't add items from later generations.
        self.item_class_test_invalid_items(
            bag,
            self.__wrong_generation_all_pocket_items
        )

    #
    # Gold
    #

    def test_gold_item_pocket(self):
        item_pocket = pkmn.item_list("Items", pkmn.game.GOLD)
        self.__test_item_pocket(item_pocket, pkmn.game.GOLD)

    def test_gold_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("KeyItems", pkmn.game.GOLD)
        self.__test_key_item_pocket(key_item_pocket, pkmn.game.GOLD)

    def test_gold_ball_pocket(self):
        ball_pocket = pkmn.item_list("Balls", pkmn.game.GOLD)
        self.__test_ball_pocket(ball_pocket, pkmn.game.GOLD)

    def test_gold_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TM/HM", pkmn.game.GOLD)
        self.__test_tmhm_pocket(tmhm_pocket, pkmn.game.GOLD)

    def test_gold_item_bag(self):
        bag = pkmn.item_bag(pkmn.game.GOLD)
        self.__test_item_bag(bag, pkmn.game.GOLD)

    def test_gold_pc(self):
        pc = pkmn.item_list("PC", pkmn.game.GOLD)
        self.__test_pc(pc, pkmn.game.GOLD)

    #
    # Silver
    #

    def test_silver_item_pocket(self):
        item_pocket = pkmn.item_list("Items", pkmn.game.SILVER)
        self.__test_item_pocket(item_pocket, pkmn.game.SILVER)

    def test_silver_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("KeyItems", pkmn.game.SILVER)
        self.__test_key_item_pocket(key_item_pocket, pkmn.game.SILVER)

    def test_silver_ball_pocket(self):
        ball_pocket = pkmn.item_list("Balls", pkmn.game.SILVER)
        self.__test_ball_pocket(ball_pocket, pkmn.game.SILVER)

    def test_silver_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TM/HM", pkmn.game.SILVER)
        self.__test_tmhm_pocket(tmhm_pocket, pkmn.game.SILVER)

    def test_silver_item_bag(self):
        bag = pkmn.item_bag(pkmn.game.SILVER)
        self.__test_item_bag(bag, pkmn.game.SILVER)

    def test_silver_pc(self):
        pc = pkmn.item_list("PC", pkmn.game.SILVER)
        self.__test_pc(pc, pkmn.game.SILVER)

    #
    # Crystal
    #

    def test_crystal_item_pocket(self):
        item_pocket = pkmn.item_list("Items", pkmn.game.CRYSTAL)
        self.__test_item_pocket(item_pocket, pkmn.game.CRYSTAL)

    def test_crystal_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("KeyItems", pkmn.game.CRYSTAL)
        self.__test_key_item_pocket(key_item_pocket, pkmn.game.CRYSTAL)

    def test_crystal_ball_pocket(self):
        ball_pocket = pkmn.item_list("Balls", pkmn.game.CRYSTAL)
        self.__test_ball_pocket(ball_pocket, pkmn.game.CRYSTAL)

    def test_crystal_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TM/HM", pkmn.game.CRYSTAL)
        self.__test_tmhm_pocket(tmhm_pocket, pkmn.game.CRYSTAL)

    def test_crystal_item_bag(self):
        bag = pkmn.item_bag(pkmn.game.CRYSTAL)
        self.__test_item_bag(bag, pkmn.game.CRYSTAL)

    def test_crystal_pc(self):
        pc = pkmn.item_list("PC", pkmn.game.CRYSTAL)
        self.__test_pc(pc, pkmn.game.CRYSTAL)
