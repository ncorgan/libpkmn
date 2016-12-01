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

class gen2_items_test(items_tests):

    #
    # Helper functions
    #

    def __test_item_pocket(self, item_pocket, game):
        # Check unchanging and initial values.
        self.assertEqual(len(item_pocket.as_list()), 20)
        self.assertEqual(item_pocket.get_name(), "Items")
        self.assertEqual(item_pocket.get_game(), game)
        self.assertEqual(item_pocket.get_capacity(), 20)
        self.assertEqual(item_pocket.get_num_items(), 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(item_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(item_pocket, "Potion")

        # Make sure we can't add items from other pockets.
        self.item_list_test_items_from_wrong_pocket(
            item_pocket,
            ["Bicycle", "Master Ball", "HM01"]
        )
        self.assertEqual(item_pocket.get_num_items(), 0)

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            item_pocket,
            [u"Potion", u"HP Up", u"Wht Apricorn", u"Lucky Egg",
             u"Flower Mail", u"Burn Heal", u"PSNCureBerry", u"Stick"]
        )

        valid_items = item_pocket.get_valid_items()
        self.assertGreater(len(valid_items), 0)
        self.item_list_test_both_text_types(item_pocket)

    def __test_key_item_pocket(self, key_item_pocket, game):
        # Check unchanging and initial values.
        self.assertEqual(len(key_item_pocket.as_list()), 26)
        self.assertEqual(key_item_pocket.get_name(), "KeyItems")
        self.assertEqual(key_item_pocket.get_game(), game)
        self.assertEqual(key_item_pocket.get_capacity(), 26)
        self.assertEqual(key_item_pocket.get_num_items(), 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(key_item_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(key_item_pocket, "Bicycle")

        # Make sure we can't add items from other pockets.
        self.item_list_test_items_from_wrong_pocket(
            key_item_pocket,
            ["Potion", "Master Ball", "HM01"]
        )
        self.assertEqual(key_item_pocket.get_num_items(), 0)

        # Crystal-specific items
        if game == "Crystal":
            key_item_pocket.add("Clear Bell", 1)
            key_item_pocket.remove("Clear Bell", 1)

            key_item_pocket.add("GS Ball", 1)
            key_item_pocket.remove("GS Ball", 1)

            key_item_pocket.add("Blue Card", 1)
            key_item_pocket.remove("Blue Card", 1)

            key_item_pocket.add("Egg Ticket", 1)
            key_item_pocket.remove("Egg Ticket", 1)
        else:
            with self.assertRaises(ValueError):
                key_item_pocket.add("Clear Bell", 1)
            with self.assertRaises(ValueError):
                key_item_pocket.add("GS Ball", 1)
            with self.assertRaises(ValueError):
                key_item_pocket.add("Blue Card", 1)
            with self.assertRaises(ValueError):
                key_item_pocket.add("Egg Ticket", 1)

        self.assertEqual(key_item_pocket.get_num_items(), 0)

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            key_item_pocket,
            [u"Bicycle", u"Basement Key", u"SecretPotion", u"Mystery Egg",
             u"Silver Wing", u"Lost Item", u"SquirtBottle", u"Rainbow Wing"]
        )

        valid_items = key_item_pocket.get_valid_items()
        self.assertGreater(len(valid_items), 0)
        self.item_list_test_both_text_types(key_item_pocket)

    def __test_ball_pocket(self, ball_pocket, game):
        # Check unchanging and initial values.
        self.assertEqual(len(ball_pocket.as_list()), 12)
        self.assertEqual(ball_pocket.get_name(), "Balls")
        self.assertEqual(ball_pocket.get_game(), game)
        self.assertEqual(ball_pocket.get_capacity(), 12)
        self.assertEqual(ball_pocket.get_num_items(), 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(ball_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(ball_pocket, "Master Ball")

        # Make sure we can't add items from other pockets.
        self.item_list_test_items_from_wrong_pocket(
            ball_pocket,
            ["Potion", "Bicycle", "HM01"]
        )
        self.assertEqual(ball_pocket.get_num_items(), 0)

        # Start adding and removing stuff, and make sure the numbers are accurate.
        item_names = [u"Great Ball", u"Pok\u00e9 Ball", u"Park Ball", u"Fast Ball",
                      u"Master Ball", u"Friend Ball", u"Love Ball", u"Level Ball"]
        self.item_list_test_add_remove(
            ball_pocket,
            item_names
        )

        valid_items = ball_pocket.get_valid_items()
        self.assertGreater(len(valid_items), 0)
        self.item_list_test_both_text_types(ball_pocket)

    def __test_tmhm_pocket(self, tmhm_pocket, game):
        # Check unchanging and initial values.
        self.assertEqual(tmhm_pocket.get_name(), "TM/HM")
        self.assertEqual(tmhm_pocket.get_game(), game)
        self.assertEqual(tmhm_pocket.get_capacity(), 57)
        self.assertEqual(tmhm_pocket.get_num_items(), 0)

        # Make sure item slots start as correctly empty.
        self.assertEqual(len(tmhm_pocket.as_list()), tmhm_pocket.get_capacity())
        for i in range(1,51):
            name = "TM{0}".format(str(i).zfill(2))
            self.assertEqual(tmhm_pocket[i-1].item.get_name(), name)
            self.assertEqual(tmhm_pocket[i-1].amount, 0)
        for i in range(1,8):
            name = "HM{0}".format(str(i).zfill(2))
            self.assertEqual(tmhm_pocket[50+i-1].item.get_name(), name)
            self.assertEqual(tmhm_pocket[50+i-1].amount, 0)

        # Confirm errors are thrown when expected.
        with self.assertRaises(IndexError):
            tmhm_pocket.add("TM10", 0)
        with self.assertRaises(IndexError):
            tmhm_pocket.add("TM10", 100)
        with self.assertRaises(IndexError):
            tmhm_pocket.remove("TM10", 0)
        with self.assertRaises(IndexError):
            tmhm_pocket.remove("TM10", 100)

        # Make sure we can't add items from other pockets.
        with self.assertRaises(ValueError):
            tmhm_pocket.add("Potion", 1)
        with self.assertRaises(ValueError):
            tmhm_pocket.add("Master Ball", 1)
        with self.assertRaises(ValueError):
            tmhm_pocket.add("Bicycle", 1)
        self.assertEqual(tmhm_pocket.get_num_items(), 0)

        # Start adding and removing stuff, and make sure the numbers are accurate.
        for i in range(1,51):
            name = "TM{0}".format(str(i).zfill(2))
            tmhm_pocket.add(name, 50)
            self.assertEqual(tmhm_pocket.get_num_items(), i)
            self.assertEqual(tmhm_pocket[i-1].item.get_name(), name)
            self.assertEqual(tmhm_pocket[i-1].amount, 50)
        for i in range(1,51)[::-1]:
            name = "TM{0}".format(str(i).zfill(2))
            tmhm_pocket.remove(name, 50)
            self.assertEqual(tmhm_pocket.get_num_items(), i-1)
            self.assertEqual(tmhm_pocket[i-1].item.get_name(), name)
            self.assertEqual(tmhm_pocket[i-1].amount, 0)
        for i in range(1,8):
            name = "HM{0}".format(str(i).zfill(2))
            tmhm_pocket.add(name, 1)
            self.assertEqual(tmhm_pocket.get_num_items(), i)
            self.assertEqual(tmhm_pocket[50+i-1].item.get_name(), name)
            self.assertEqual(tmhm_pocket[50+i-1].amount, 1)
        for i in range(1,8)[::-1]:
            name = "HM{0}".format(str(i).zfill(2))
            tmhm_pocket.remove(name, 1)
            self.assertEqual(tmhm_pocket.get_num_items(), i-1)
            self.assertEqual(tmhm_pocket[50+i-1].item.get_name(), name)
            self.assertEqual(tmhm_pocket[50+i-1].amount, 0)

        valid_items = tmhm_pocket.get_valid_items()
        self.assertEqual(len(valid_items), 57)
        self.item_list_test_both_text_types_with_strings(tmhm_pocket, "TM01", u"TM01")

    def __test_pc(self, pc, game):
        # Check unchanging and initial values.
        self.assertEqual(len(pc.as_list()), 50)
        self.assertEqual(pc.get_name(), "PC")
        self.assertEqual(pc.get_game(), game)
        self.assertEqual(pc.get_capacity(), 50)
        self.assertEqual(pc.get_num_items(), 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(pc)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(pc, "Potion")

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            pc,
            [u"Potion", u"Bicycle", u"Great Ball", u"TM28",
             u"Berry", u"SquirtBottle", u"Friend Ball", u"HM01"]
        )

        valid_items = pc.get_valid_items()
        full_item_list = pkmn.database.get_item_list(game)
        self.assertEqual(len(valid_items), len(full_item_list))
        self.item_list_test_both_text_types(pc)

    def __test_item_bag(self, bag, game):
        # Check unchanging and initial values.
        self.assertEqual(bag.get_game(), game)

        pockets = bag.get_pockets()
        self.assertEqual(len(pockets), 4)
        self.assertTrue(pockets.has_key("Items"))
        self.assertTrue(pockets.has_key("KeyItems"))
        self.assertTrue(pockets.has_key("Balls"))
        self.assertTrue(pockets.has_key("TM/HM"))

        self.__test_item_pocket(bag["Items"], game)
        self.__test_key_item_pocket(bag["KeyItems"], game)
        self.__test_ball_pocket(bag["Balls"], game)
        self.__test_tmhm_pocket(bag["TM/HM"], game)
        self.item_bag_test_get_pockets_with_both_text_types(bag)

    #
    # Gold
    #

    def test_gold_item_pocket(self):
        item_pocket = pkmn.item_list("Items", "Gold")
        self.__test_item_pocket(item_pocket, "Gold")

    def test_gold_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("KeyItems", "Gold")
        self.__test_key_item_pocket(key_item_pocket, "Gold")

    def test_gold_ball_pocket(self):
        ball_pocket = pkmn.item_list("Balls", "Gold")
        self.__test_ball_pocket(ball_pocket, "Gold")

    def test_gold_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TM/HM", "Gold")
        self.__test_tmhm_pocket(tmhm_pocket, "Gold")

    def test_gold_item_bag(self):
        bag = pkmn.item_bag("Gold")
        self.__test_item_bag(bag, "Gold")

    def test_gold_pc(self):
        pc = pkmn.item_list("PC", "Gold")
        self.__test_pc(pc, "Gold")

    #
    # Silver
    #

    def test_silver_item_pocket(self):
        item_pocket = pkmn.item_list("Items", "Silver")
        self.__test_item_pocket(item_pocket, "Silver")

    def test_silver_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("KeyItems", "Silver")
        self.__test_key_item_pocket(key_item_pocket, "Silver")

    def test_silver_ball_pocket(self):
        ball_pocket = pkmn.item_list("Balls", "Silver")
        self.__test_ball_pocket(ball_pocket, "Silver")

    def test_silver_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TM/HM", "Silver")
        self.__test_tmhm_pocket(tmhm_pocket, "Silver")

    def test_silver_item_bag(self):
        bag = pkmn.item_bag("Silver")
        self.__test_item_bag(bag, "Silver")

    def test_silver_pc(self):
        pc = pkmn.item_list("PC", "Silver")
        self.__test_pc(pc, "Silver")

    #
    # Crystal
    #

    def test_crystal_item_pocket(self):
        item_pocket = pkmn.item_list("Items", "Crystal")
        self.__test_item_pocket(item_pocket, "Crystal")

    def test_crystal_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("KeyItems", "Crystal")
        self.__test_key_item_pocket(key_item_pocket, "Crystal")

    def test_crystal_ball_pocket(self):
        ball_pocket = pkmn.item_list("Balls", "Crystal")
        self.__test_ball_pocket(ball_pocket, "Crystal")

    def test_crystal_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TM/HM", "Crystal")
        self.__test_tmhm_pocket(tmhm_pocket, "Crystal")

    def test_crystal_item_bag(self):
        bag = pkmn.item_bag("Crystal")
        self.__test_item_bag(bag, "Crystal")

    def test_crystal_pc(self):
        pc = pkmn.item_list("PC", "Crystal")
        self.__test_pc(pc, "Crystal")
