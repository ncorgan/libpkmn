#!/usr/bin/env python
#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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

    def __get_both_string_types(self, input_str):
        if PYTHON_MAJOR_VERSION == 2:
            # Which call succeeds depends on SWIG version
            try:
                return (input_str, input_str.decode("utf-8"))
            except:
                return (input_str, unicode(input_str))
        else:
            return (input_str, input_str)

    def item_bag_test_get_pockets_with_both_text_types(self, bag):
        pockets = bag.get_pockets()
        for pocket_name in pockets:
            strs = self.__get_both_string_types(pocket_name)

            # Test getting pockets from bag
            self.assertEqual(bag.get_pocket(strs[0]), bag.get_pocket(strs[1]))
            self.assertEqual(bag[strs[0]], bag[strs[1]])

            # Test getting pockets standalone
            from_str = pkmn.item_list(strs[0], bag.get_game())
            from_unicode = pkmn.item_list(strs[1], bag.get_game())

    def item_list_test_both_text_types_with_strings(self, items, as_str, as_unicode):
        items.add(as_str, 1)
        items.add(as_unicode, 1)
        self.assertEqual(items.get_num_items(), 1)
        self.assertEqual(items[0].amount, 2)

        items.remove(as_str, 2)
        self.assertEqual(items.get_num_items(), 0)
        self.assertEqual(items[0].amount, 0)

        items.add(as_str, 1)
        items.add(as_unicode, 1)
        self.assertEqual(items.get_num_items(), 1)
        self.assertEqual(items[0].amount, 2)

        items.remove(as_unicode, 2)
        self.assertEqual(items.get_num_items(), 0)
        self.assertEqual(items[0].amount, 0)

    # No matter the version of Python or SWIG, we should be able
    # to use ASCII or Unicode from Python.
    def item_list_test_both_text_types(self, items):
        self.assertEqual(items.get_num_items(), 0)

        strs = self.__get_both_string_types(items.get_valid_items()[0])
        self.item_list_test_both_text_types_with_strings(items, strs[0], strs[1])

    def item_list_test_empty_slot(self, items):
        for i in range(items.get_capacity()):
            self.assertEqual(items[i].item, "None")
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

    def item_class_test_invalid_items(self, items, item_names):
        for item_name in item_names:
            with self.assertRaises(ValueError):
                items.add(item_name, 1)

    def item_list_test_add_remove(self, items, item_names):
        self.assertEqual(len(item_names), 8)

        self.assertEqual(items.get_num_items(), 0)

        # Test setting by index.
        items[0] = pkmn.item_slot(item_names[0], 50)
        items[1] = pkmn.item_slot(item_names[1], 40)
        items[2] = pkmn.item_slot(item_names[2], 30)

        self.assertStringEqual(items[0].item, item_names[0])
        self.assertEqual(items[0].amount, 50)
        self.assertStringEqual(items[1].item, item_names[1])
        self.assertEqual(items[1].amount, 40)
        self.assertStringEqual(items[2].item, item_names[2])
        self.assertEqual(items[2].amount, 30)
        self.assertEqual(items.get_num_items(), 3)

        with self.assertRaises(IndexError):
            items[10] = pkmn.item_slot("Master Ball", 5)
        with self.assertRaises(ValueError):
            items[0] = pkmn.item_slot("None", 5)
        with self.assertRaises(ValueError):
            items[1] = pkmn.item_slot(item_names[0], 1)

        items[1] = pkmn.item_slot("None", 0)

        self.assertStringEqual(items[0].item, item_names[0])
        self.assertEqual(items[0].amount, 50)
        self.assertStringEqual(items[1].item, item_names[2])
        self.assertEqual(items[1].amount, 30)
        self.assertStringEqual(items[2].item, "None")
        self.assertEqual(items[2].amount, 0)
        self.assertEqual(items.get_num_items(), 2)

        items[0] = pkmn.item_slot("None", 0)
        items[0] = pkmn.item_slot("None", 0)

        self.assertStringEqual(items[0].item, "None")
        self.assertEqual(items[0].amount, 0)
        self.assertStringEqual(items[1].item, "None")
        self.assertEqual(items[1].amount, 0)
        self.assertStringEqual(items[2].item, "None")
        self.assertEqual(items[2].amount, 0)
        self.assertEqual(items.get_num_items(), 0)

        # Test add/remove functions.
        items.add(item_names[0], 30)
        items.add(item_names[1], 99)
        items.add(item_names[2], 1)

        self.assertStringEqual(items[0].item, item_names[0])
        self.assertEqual(items[0].amount, 30)
        self.assertStringEqual(items[1].item, item_names[1])
        self.assertEqual(items[1].amount, 99)
        self.assertStringEqual(items[2].item, item_names[2])
        self.assertEqual(items[2].amount, 1)
        self.assertEqual(items.get_num_items(), 3)

        items.add(item_names[2], 15)

        self.assertStringEqual(items[0].item, item_names[0])
        self.assertEqual(items[0].amount, 30)
        self.assertStringEqual(items[1].item, item_names[1])
        self.assertEqual(items[1].amount, 99)
        self.assertStringEqual(items[2].item, item_names[2])
        self.assertEqual(items[2].amount, 16)
        self.assertEqual(items.get_num_items(), 3)

        items.remove(item_names[1], 20)

        self.assertStringEqual(items[0].item, item_names[0])
        self.assertEqual(items[0].amount, 30)
        self.assertStringEqual(items[1].item, item_names[1])
        self.assertEqual(items[1].amount, 79)
        self.assertStringEqual(items[2].item, item_names[2])
        self.assertEqual(items[2].amount, 16)
        self.assertEqual(items.get_num_items(), 3)

        items.move(0, 1)

        self.assertStringEqual(items[0].item, item_names[1])
        self.assertEqual(items[0].amount, 79)
        self.assertStringEqual(items[1].item, item_names[0])
        self.assertEqual(items[1].amount, 30)
        self.assertStringEqual(items[2].item, item_names[2])
        self.assertEqual(items[2].amount, 16)
        self.assertEqual(items.get_num_items(), 3)

        items.remove(item_names[0], 30)

        self.assertStringEqual(items[0].item, item_names[1])
        self.assertEqual(items[0].amount, 79)
        self.assertStringEqual(items[1].item, item_names[2])
        self.assertEqual(items[1].amount, 16)
        self.assertEqual(items[2].item, "None")
        self.assertEqual(items[2].amount, 0)
        self.assertEqual(items.get_num_items(), 2)

        items.add(item_names[3], 90)

        self.assertStringEqual(items[0].item, item_names[1])
        self.assertEqual(items[0].amount, 79)
        self.assertStringEqual(items[1].item, item_names[2])
        self.assertEqual(items[1].amount, 16)
        self.assertStringEqual(items[2].item, item_names[3])
        self.assertEqual(items[2].amount, 90)
        self.assertEqual(items.get_num_items(), 3)

        items.add(item_names[4], 2)

        self.assertStringEqual(items[0].item, item_names[1])
        self.assertEqual(items[0].amount, 79)
        self.assertStringEqual(items[1].item, item_names[2])
        self.assertEqual(items[1].amount, 16)
        self.assertStringEqual(items[2].item, item_names[3])
        self.assertEqual(items[2].amount, 90)
        self.assertStringEqual(items[3].item, item_names[4])
        self.assertEqual(items[3].amount, 2)
        self.assertEqual(items.get_num_items(), 4)

        items.remove(item_names[1], 30)

        self.assertStringEqual(items[0].item, item_names[1])
        self.assertEqual(items[0].amount, 49)
        self.assertStringEqual(items[1].item, item_names[2])
        self.assertEqual(items[1].amount, 16)
        self.assertStringEqual(items[2].item, item_names[3])
        self.assertEqual(items[2].amount, 90)
        self.assertStringEqual(items[3].item, item_names[4])
        self.assertEqual(items[3].amount, 2)
        self.assertEqual(items.get_num_items(), 4)

        items.add(item_names[5], 12)

        self.assertStringEqual(items[0].item, item_names[1])
        self.assertEqual(items[0].amount, 49)
        self.assertStringEqual(items[1].item, item_names[2])
        self.assertEqual(items[1].amount, 16)
        self.assertStringEqual(items[2].item, item_names[3])
        self.assertEqual(items[2].amount, 90)
        self.assertStringEqual(items[3].item, item_names[4])
        self.assertEqual(items[3].amount, 2)
        self.assertStringEqual(items[4].item, item_names[5])
        self.assertEqual(items[4].amount, 12)
        self.assertEqual(items.get_num_items(), 5)

        items.remove(item_names[2], 16)

        self.assertStringEqual(items[0].item, item_names[1])
        self.assertEqual(items[0].amount, 49)
        self.assertStringEqual(items[1].item, item_names[3])
        self.assertEqual(items[1].amount, 90)
        self.assertStringEqual(items[2].item, item_names[4])
        self.assertEqual(items[2].amount, 2)
        self.assertStringEqual(items[3].item, item_names[5])
        self.assertEqual(items[3].amount, 12)
        self.assertEqual(items.get_num_items(), 4)

        items.add(item_names[6], 65)

        self.assertStringEqual(items[0].item, item_names[1])
        self.assertEqual(items[0].amount, 49)
        self.assertStringEqual(items[1].item, item_names[3])
        self.assertEqual(items[1].amount, 90)
        self.assertStringEqual(items[2].item, item_names[4])
        self.assertEqual(items[2].amount, 2)
        self.assertStringEqual(items[3].item, item_names[5])
        self.assertEqual(items[3].amount, 12)
        self.assertStringEqual(items[4].item, item_names[6])
        self.assertEqual(items[4].amount, 65)
        self.assertEqual(items.get_num_items(), 5)

        items.add(item_names[7], 6)

        self.assertStringEqual(items[0].item, item_names[1])
        self.assertEqual(items[0].amount, 49)
        self.assertStringEqual(items[1].item, item_names[3])
        self.assertEqual(items[1].amount, 90)
        self.assertStringEqual(items[2].item, item_names[4])
        self.assertEqual(items[2].amount, 2)
        self.assertStringEqual(items[3].item, item_names[5])
        self.assertEqual(items[3].amount, 12)
        self.assertStringEqual(items[4].item, item_names[6])
        self.assertEqual(items[4].amount, 65)
        self.assertStringEqual(items[5].item, item_names[7])
        self.assertEqual(items[5].amount, 6)
        self.assertEqual(items.get_num_items(), 6)

        items.remove(item_names[4], 2)
        items.remove(item_names[1], 49)
        items.remove(item_names[7], 6)
        items.remove(item_names[5], 12)
        items.remove(item_names[3], 90)
        items.remove(item_names[6], 65)

        self.assertEqual(items.get_num_items(), 0)
