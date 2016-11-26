#!/usr/bin/env python
#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import unittest

class gen2_items_test(unittest.TestCase):

    #
    # Helper functions
    #

    def __test_item_pocket(self, item_pocket, game):
        # Check unchanging and initial values.
        self.assertEqual(item_pocket.get_name(), "Items")
        self.assertEqual(item_pocket.get_game(), game)
        self.assertEqual(item_pocket.get_capacity(), 20)
        self.assertEqual(item_pocket.get_num_items(), 0)

        # Make sure item slots start as correctly empty.
        self.assertEqual(len(item_pocket.as_list()), item_pocket.get_capacity())
        for i in range(item_pocket.get_capacity()):
            self.assertEqual(item_pocket[i].item.get_name(), "None")
            self.assertEqual(item_pocket[i].amount, 0)

        # Confirm errors are thrown when expected.
        with self.assertRaises(IndexError):
            item_pocket.add("Potion", 0)
        with self.assertRaises(IndexError):
            item_pocket.add("Potion", 100)
        with self.assertRaises(IndexError):
            item_pocket.remove("Potion", 0)
        with self.assertRaises(IndexError):
            item_pocket.remove("Potion", 100)

        # Make sure we can't add items from other pockets.
        with self.assertRaises(ValueError):
            item_pocket.add("Bicycle", 1)
        with self.assertRaises(ValueError):
            item_pocket.add("Master Ball", 1)
        with self.assertRaises(ValueError):
            item_pocket.add("HM01", 1)
        self.assertEqual(item_pocket.get_num_items(), 0)

        # Start adding and removing stuff, and make sure the numbers are accurate.
        item_pocket.add("Potion", 30)
        item_pocket.add("PSNCureBerry", 99)
        item_pocket.add("Leftovers", 1)

        self.assertEqual(item_pocket[0].item.get_name(), "Potion")
        self.assertEqual(item_pocket[0].amount, 30)
        self.assertEqual(item_pocket[1].item.get_name(), "PSNCureBerry")
        self.assertEqual(item_pocket[1].amount, 99)
        self.assertEqual(item_pocket[2].item.get_name(), "Leftovers")
        self.assertEqual(item_pocket[2].amount, 1)
        self.assertEqual(item_pocket.get_num_items(), 3)

        item_pocket.add("Leftovers", 15)
        self.assertEqual(item_pocket[0].item.get_name(), "Potion")
        self.assertEqual(item_pocket[0].amount, 30)
        self.assertEqual(item_pocket[1].item.get_name(), "PSNCureBerry")
        self.assertEqual(item_pocket[1].amount, 99)
        self.assertEqual(item_pocket[2].item.get_name(), "Leftovers")
        self.assertEqual(item_pocket[2].amount, 16)
        self.assertEqual(item_pocket.get_num_items(), 3)

        item_pocket.remove("PSNCureBerry", 20)
        self.assertEqual(item_pocket[0].item.get_name(), "Potion")
        self.assertEqual(item_pocket[0].amount, 30)
        self.assertEqual(item_pocket[1].item.get_name(), "PSNCureBerry")
        self.assertEqual(item_pocket[1].amount, 79)
        self.assertEqual(item_pocket[2].item.get_name(), "Leftovers")
        self.assertEqual(item_pocket[2].amount, 16)
        self.assertEqual(item_pocket.get_num_items(), 3)

        item_pocket.move(0, 1)
        self.assertEqual(item_pocket[0].item.get_name(), "PSNCureBerry")
        self.assertEqual(item_pocket[0].amount, 79)
        self.assertEqual(item_pocket[1].item.get_name(), "Potion")
        self.assertEqual(item_pocket[1].amount, 30)
        self.assertEqual(item_pocket[2].item.get_name(), "Leftovers")
        self.assertEqual(item_pocket[2].amount, 16)
        self.assertEqual(item_pocket.get_num_items(), 3)

        item_pocket.remove("Potion", 30)
        self.assertEqual(item_pocket[0].item.get_name(), "PSNCureBerry")
        self.assertEqual(item_pocket[0].amount, 79)
        self.assertEqual(item_pocket[1].item.get_name(), "Leftovers")
        self.assertEqual(item_pocket[1].amount, 16)
        self.assertEqual(item_pocket[2].item.get_name(), "None")
        self.assertEqual(item_pocket[2].amount, 0)
        self.assertEqual(item_pocket.get_num_items(), 2)

        valid_items = item_pocket.get_valid_items()
        self.assertGreater(len(valid_items), 0)

    def __test_key_item_pocket(self, key_item_pocket, game):
        # Check unchanging and initial values.
        self.assertEqual(key_item_pocket.get_name(), "KeyItems")
        self.assertEqual(key_item_pocket.get_game(), game)
        self.assertEqual(key_item_pocket.get_capacity(), 26)
        self.assertEqual(key_item_pocket.get_num_items(), 0)

        # Make sure item slots start as correctly empty.
        self.assertEqual(len(key_item_pocket.as_list()), key_item_pocket.get_capacity())
        for i in range(key_item_pocket.get_capacity()):
            self.assertEqual(key_item_pocket[i].item.get_name(), "None")
            self.assertEqual(key_item_pocket[i].amount, 0)

        # Confirm errors are thrown when expected.
        with self.assertRaises(IndexError):
            key_item_pocket.add("Bicycle", 0)
        with self.assertRaises(IndexError):
            key_item_pocket.add("Bicycle", 100)
        with self.assertRaises(IndexError):
            key_item_pocket.remove("Bicycle", 0)
        with self.assertRaises(IndexError):
            key_item_pocket.remove("Bicycle", 100)

        # Make sure we can't add items from other pockets.
        with self.assertRaises(ValueError):
            key_item_pocket.add("potion", 1)
        with self.assertRaises(ValueError):
            key_item_pocket.add("Master Ball", 1)
        with self.assertRaises(ValueError):
            key_item_pocket.add("HM01", 1)
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

            self.assertEqual(key_item_pocket.get_num_items(), 0)
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
        key_item_pocket.add("Bicycle", 30)
        key_item_pocket.add("Basement Key", 99)
        key_item_pocket.add("SecretPotion", 1)

        self.assertEqual(key_item_pocket[0].item.get_name(), "Bicycle")
        self.assertEqual(key_item_pocket[0].amount, 30)
        self.assertEqual(key_item_pocket[1].item.get_name(), "Basement Key")
        self.assertEqual(key_item_pocket[1].amount, 99)
        self.assertEqual(key_item_pocket[2].item.get_name(), "SecretPotion")
        self.assertEqual(key_item_pocket[2].amount, 1)
        self.assertEqual(key_item_pocket.get_num_items(), 3)

        key_item_pocket.add("SecretPotion", 15)
        self.assertEqual(key_item_pocket[0].item.get_name(), "Bicycle")
        self.assertEqual(key_item_pocket[0].amount, 30)
        self.assertEqual(key_item_pocket[1].item.get_name(), "Basement Key")
        self.assertEqual(key_item_pocket[1].amount, 99)
        self.assertEqual(key_item_pocket[2].item.get_name(), "SecretPotion")
        self.assertEqual(key_item_pocket[2].amount, 16)
        self.assertEqual(key_item_pocket.get_num_items(), 3)

        key_item_pocket.remove("Basement Key", 20)
        self.assertEqual(key_item_pocket[0].item.get_name(), "Bicycle")
        self.assertEqual(key_item_pocket[0].amount, 30)
        self.assertEqual(key_item_pocket[1].item.get_name(), "Basement Key")
        self.assertEqual(key_item_pocket[1].amount, 79)
        self.assertEqual(key_item_pocket[2].item.get_name(), "SecretPotion")
        self.assertEqual(key_item_pocket[2].amount, 16)
        self.assertEqual(key_item_pocket.get_num_items(), 3)

        key_item_pocket.move(0, 1)
        self.assertEqual(key_item_pocket[0].item.get_name(), "Basement Key")
        self.assertEqual(key_item_pocket[0].amount, 79)
        self.assertEqual(key_item_pocket[1].item.get_name(), "Bicycle")
        self.assertEqual(key_item_pocket[1].amount, 30)
        self.assertEqual(key_item_pocket[2].item.get_name(), "SecretPotion")
        self.assertEqual(key_item_pocket[2].amount, 16)
        self.assertEqual(key_item_pocket.get_num_items(), 3)

        key_item_pocket.remove("Bicycle", 30)
        self.assertEqual(key_item_pocket[0].item.get_name(), "Basement Key")
        self.assertEqual(key_item_pocket[0].amount, 79)
        self.assertEqual(key_item_pocket[1].item.get_name(), "SecretPotion")
        self.assertEqual(key_item_pocket[1].amount, 16)
        self.assertEqual(key_item_pocket[2].item.get_name(), "None")
        self.assertEqual(key_item_pocket[2].amount, 0)
        self.assertEqual(key_item_pocket.get_num_items(), 2)

        valid_items = key_item_pocket.get_valid_items()
        self.assertGreater(len(valid_items), 0)

    def __test_ball_pocket(self, ball_pocket, game):
        # Check unchanging and initial values.
        self.assertEqual(ball_pocket.get_name(), "Balls")
        self.assertEqual(ball_pocket.get_game(), game)
        self.assertEqual(ball_pocket.get_capacity(), 12)
        self.assertEqual(ball_pocket.get_num_items(), 0)

        # Make sure item slots start as correctly empty.
        self.assertEqual(len(ball_pocket.as_list()), ball_pocket.get_capacity())
        for i in range(ball_pocket.get_capacity()):
            self.assertEqual(ball_pocket[i].item.get_name(), "None")
            self.assertEqual(ball_pocket[i].amount, 0)

        # Confirm errors are thrown when expected.
        with self.assertRaises(IndexError):
            ball_pocket.add("Master Ball", 0)
        with self.assertRaises(IndexError):
            ball_pocket.add("Master Ball", 100)
        with self.assertRaises(IndexError):
            ball_pocket.remove("Master Ball", 0)
        with self.assertRaises(IndexError):
            ball_pocket.remove("Master Ball", 100)

        # Make sure we can't add items from other pockets.
        with self.assertRaises(ValueError):
            ball_pocket.add("Potion", 1)
        with self.assertRaises(ValueError):
            ball_pocket.add("Bicycle", 1)
        with self.assertRaises(ValueError):
            ball_pocket.add("HM01", 1)
        self.assertEqual(ball_pocket.get_num_items(), 0)

        # Start adding and removing stuff, and make sure the numbers are accurate.
        ball_pocket.add("Great Ball", 30)
        ball_pocket.add("Fast Ball", 99)
        ball_pocket.add("Park Ball", 1)

        self.assertEqual(ball_pocket[0].item.get_name(), "Great Ball")
        self.assertEqual(ball_pocket[0].amount, 30)
        self.assertEqual(ball_pocket[1].item.get_name(), "Fast Ball")
        self.assertEqual(ball_pocket[1].amount, 99)
        self.assertEqual(ball_pocket[2].item.get_name(), "Park Ball")
        self.assertEqual(ball_pocket[2].amount, 1)
        self.assertEqual(ball_pocket.get_num_items(), 3)

        ball_pocket.add("Park Ball", 15)
        self.assertEqual(ball_pocket[0].item.get_name(), "Great Ball")
        self.assertEqual(ball_pocket[0].amount, 30)
        self.assertEqual(ball_pocket[1].item.get_name(), "Fast Ball")
        self.assertEqual(ball_pocket[1].amount, 99)
        self.assertEqual(ball_pocket[2].item.get_name(), "Park Ball")
        self.assertEqual(ball_pocket[2].amount, 16)
        self.assertEqual(ball_pocket.get_num_items(), 3)

        ball_pocket.remove("Fast Ball", 20)
        self.assertEqual(ball_pocket[0].item.get_name(), "Great Ball")
        self.assertEqual(ball_pocket[0].amount, 30)
        self.assertEqual(ball_pocket[1].item.get_name(), "Fast Ball")
        self.assertEqual(ball_pocket[1].amount, 79)
        self.assertEqual(ball_pocket[2].item.get_name(), "Park Ball")
        self.assertEqual(ball_pocket[2].amount, 16)
        self.assertEqual(ball_pocket.get_num_items(), 3)

        ball_pocket.move(0, 1)
        self.assertEqual(ball_pocket[0].item.get_name(), "Fast Ball")
        self.assertEqual(ball_pocket[0].amount, 79)
        self.assertEqual(ball_pocket[1].item.get_name(), "Great Ball")
        self.assertEqual(ball_pocket[1].amount, 30)
        self.assertEqual(ball_pocket[2].item.get_name(), "Park Ball")
        self.assertEqual(ball_pocket[2].amount, 16)
        self.assertEqual(ball_pocket.get_num_items(), 3)

        ball_pocket.remove("Great Ball", 30)
        self.assertEqual(ball_pocket[0].item.get_name(), "Fast Ball")
        self.assertEqual(ball_pocket[0].amount, 79)
        self.assertEqual(ball_pocket[1].item.get_name(), "Park Ball")
        self.assertEqual(ball_pocket[1].amount, 16)
        self.assertEqual(ball_pocket[2].item.get_name(), "None")
        self.assertEqual(ball_pocket[2].amount, 0)
        self.assertEqual(ball_pocket.get_num_items(), 2)

        valid_items = ball_pocket.get_valid_items()
        self.assertGreater(len(valid_items), 0)

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
