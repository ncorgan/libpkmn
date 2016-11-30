#!/usr/bin/env python
#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import sys
import unittest

PYTHON_MAJOR_VERSION = sys.version_info[0]

class items_tests(unittest.TestCase):

    def assertStringEqual(self, str1, str2):
        if PYTHON_MAJOR_VERSION == 2:
            if "str" in str(type(str1)):
                str1 = str1.decode("utf-8")
            if "str" in str(type(str2)):
                str2 = str2.decode("utf-8")

        self.assertEqual(str1, str2)

    def item_list_test_empty_slot(self, items):
        none_entry = pkmn.database.item_entry("None", items.get_game())
        for i in range(items.get_capacity()):
            self.assertEqual(items[i].item, none_entry)
            self.assertEqual(items[i].amount, 0)

    def item_list_test_out_of_range_error(self, items, item_name):
        with self.assertRaises(IndexError):
            items.add(item_name, 0)
        with self.assertRaises(IndexError):
            items.add(item_name, 100)
        with self.assertRaises(IndexError):
            items.remove(item_name, 0)
        with self.assertRaises(IndexError):
            items.remove(item_name, 100)

    def item_list_test_items_from_wrong_pocket(self, items, item_names):
        for item_name in item_names:
            with self.assertRaises(ValueError):
                items.add(item_name, 1)

    def item_list_test_add_remove(self, items, item_names):
        self.assertEqual(len(item_names), 8)

        none_entry = pkmn.database.item_entry("None", items.get_game())

        self.assertEqual(items.get_num_items(), 0)

        items.add(item_names[0], 30)
        items.add(item_names[1], 99)
        items.add(item_names[2], 1)

        self.assertStringEqual(items[0].item.get_name(), item_names[0])
        self.assertEqual(items[0].amount, 30)
        self.assertStringEqual(items[1].item.get_name(), item_names[1])
        self.assertEqual(items[1].amount, 99)
        self.assertStringEqual(items[2].item.get_name(), item_names[2])
        self.assertEqual(items[2].amount, 1)
        self.assertEqual(items.get_num_items(), 3)

        items.add(item_names[2], 15)

        self.assertStringEqual(items[0].item.get_name(), item_names[0])
        self.assertEqual(items[0].amount, 30)
        self.assertStringEqual(items[1].item.get_name(), item_names[1])
        self.assertEqual(items[1].amount, 99)
        self.assertStringEqual(items[2].item.get_name(), item_names[2])
        self.assertEqual(items[2].amount, 16)
        self.assertEqual(items.get_num_items(), 3)

        items.remove(item_names[1], 20)

        self.assertStringEqual(items[0].item.get_name(), item_names[0])
        self.assertEqual(items[0].amount, 30)
        self.assertStringEqual(items[1].item.get_name(), item_names[1])
        self.assertEqual(items[1].amount, 79)
        self.assertStringEqual(items[2].item.get_name(), item_names[2])
        self.assertEqual(items[2].amount, 16)
        self.assertEqual(items.get_num_items(), 3)

        items.move(0, 1)

        self.assertStringEqual(items[0].item.get_name(), item_names[1])
        self.assertEqual(items[0].amount, 79)
        self.assertStringEqual(items[1].item.get_name(), item_names[0])
        self.assertEqual(items[1].amount, 30)
        self.assertStringEqual(items[2].item.get_name(), item_names[2])
        self.assertEqual(items[2].amount, 16)
        self.assertEqual(items.get_num_items(), 3)

        items.remove(item_names[0], 30)

        self.assertStringEqual(items[0].item.get_name(), item_names[1])
        self.assertEqual(items[0].amount, 79)
        self.assertStringEqual(items[1].item.get_name(), item_names[2])
        self.assertEqual(items[1].amount, 16)
        self.assertEqual(items[2].item, none_entry)
        self.assertEqual(items[2].amount, 0)
        self.assertEqual(items.get_num_items(), 2)

        items.add(item_names[3], 90)

        self.assertStringEqual(items[0].item.get_name(), item_names[1])
        self.assertEqual(items[0].amount, 79)
        self.assertStringEqual(items[1].item.get_name(), item_names[2])
        self.assertEqual(items[1].amount, 16)
        self.assertStringEqual(items[2].item.get_name(), item_names[3])
        self.assertEqual(items[2].amount, 90)
        self.assertEqual(items.get_num_items(), 3)

        items.add(item_names[4], 2)

        self.assertStringEqual(items[0].item.get_name(), item_names[1])
        self.assertEqual(items[0].amount, 79)
        self.assertStringEqual(items[1].item.get_name(), item_names[2])
        self.assertEqual(items[1].amount, 16)
        self.assertStringEqual(items[2].item.get_name(), item_names[3])
        self.assertEqual(items[2].amount, 90)
        self.assertStringEqual(items[3].item.get_name(), item_names[4])
        self.assertEqual(items[3].amount, 2)
        self.assertEqual(items.get_num_items(), 4)

        items.remove(item_names[1], 30)

        self.assertStringEqual(items[0].item.get_name(), item_names[1])
        self.assertEqual(items[0].amount, 49)
        self.assertStringEqual(items[1].item.get_name(), item_names[2])
        self.assertEqual(items[1].amount, 16)
        self.assertStringEqual(items[2].item.get_name(), item_names[3])
        self.assertEqual(items[2].amount, 90)
        self.assertStringEqual(items[3].item.get_name(), item_names[4])
        self.assertEqual(items[3].amount, 2)
        self.assertEqual(items.get_num_items(), 4)

        items.add(item_names[5], 12)

        self.assertStringEqual(items[0].item.get_name(), item_names[1])
        self.assertEqual(items[0].amount, 49)
        self.assertStringEqual(items[1].item.get_name(), item_names[2])
        self.assertEqual(items[1].amount, 16)
        self.assertStringEqual(items[2].item.get_name(), item_names[3])
        self.assertEqual(items[2].amount, 90)
        self.assertStringEqual(items[3].item.get_name(), item_names[4])
        self.assertEqual(items[3].amount, 2)
        self.assertStringEqual(items[4].item.get_name(), item_names[5])
        self.assertEqual(items[4].amount, 12)
        self.assertEqual(items.get_num_items(), 5)

        items.remove(item_names[2], 16)

        self.assertStringEqual(items[0].item.get_name(), item_names[1])
        self.assertEqual(items[0].amount, 49)
        self.assertStringEqual(items[1].item.get_name(), item_names[3])
        self.assertEqual(items[1].amount, 90)
        self.assertStringEqual(items[2].item.get_name(), item_names[4])
        self.assertEqual(items[2].amount, 2)
        self.assertStringEqual(items[3].item.get_name(), item_names[5])
        self.assertEqual(items[3].amount, 12)
        self.assertEqual(items.get_num_items(), 4)

        items.add(item_names[6], 65)

        self.assertStringEqual(items[0].item.get_name(), item_names[1])
        self.assertEqual(items[0].amount, 49)
        self.assertStringEqual(items[1].item.get_name(), item_names[3])
        self.assertEqual(items[1].amount, 90)
        self.assertStringEqual(items[2].item.get_name(), item_names[4])
        self.assertEqual(items[2].amount, 2)
        self.assertStringEqual(items[3].item.get_name(), item_names[5])
        self.assertEqual(items[3].amount, 12)
        self.assertStringEqual(items[4].item.get_name(), item_names[6])
        self.assertEqual(items[4].amount, 65)
        self.assertEqual(items.get_num_items(), 5)

        items.add(item_names[7], 6)

        self.assertStringEqual(items[0].item.get_name(), item_names[1])
        self.assertEqual(items[0].amount, 49)
        self.assertStringEqual(items[1].item.get_name(), item_names[3])
        self.assertEqual(items[1].amount, 90)
        self.assertStringEqual(items[2].item.get_name(), item_names[4])
        self.assertEqual(items[2].amount, 2)
        self.assertStringEqual(items[3].item.get_name(), item_names[5])
        self.assertEqual(items[3].amount, 12)
        self.assertStringEqual(items[4].item.get_name(), item_names[6])
        self.assertEqual(items[4].amount, 65)
        self.assertStringEqual(items[5].item.get_name(), item_names[7])
        self.assertEqual(items[5].amount, 6)
        self.assertEqual(items.get_num_items(), 6)

        items.remove(item_names[4], 2)
        items.remove(item_names[1], 49)
        items.remove(item_names[7], 6)
        items.remove(item_names[5], 12)
        items.remove(item_names[3], 90)
        items.remove(item_names[6], 65)

        self.assertEqual(items.get_num_items(), 0)
