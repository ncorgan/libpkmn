#!/usr/bin/env python
#
# Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

from . import base_test

import pkmn

import sys
import unittest

# TODO: test setting item

PYTHON_MAJOR_VERSION = sys.version_info[0]

class items_tests(base_test):

    def item_bag_test_get_pockets_with_both_text_types(self, bag):
        for pocket_name in bag.pocket_names:
            strs = self.get_both_string_types(pocket_name)
            self.assertEqual(bag[strs[0]], bag[strs[1]])

    def item_list_test_both_text_types_with_strings(self, items, as_str, as_unicode):
        items.add(as_str, 1)
        items.add(as_unicode, 1)
        self.assertEqual(items.num_items, 1)
        self.assertEqual(items[0].amount, 2)

        items.remove(as_str, 2)
        self.assertEqual(items.num_items, 0)
        self.assertEqual(items[0].amount, 0)

        items.add(as_str, 1)
        items.add(as_unicode, 1)
        self.assertEqual(items.num_items, 1)
        self.assertEqual(items[0].amount, 2)

        items.remove(as_unicode, 2)
        self.assertEqual(items.num_items, 0)
        self.assertEqual(items[0].amount, 0)

    # No matter the version of Python or SWIG, we should be able
    # to use ASCII or Unicode from Python.
    def item_list_test_both_text_types(self, items):
        self.assertEqual(items.num_items, 0)

        strs = self.get_both_string_types(items.valid_items[0])
        self.item_list_test_both_text_types_with_strings(items, strs[0], strs[1])

    def item_list_test_empty_slot(self, items):
        for i in range(len(items)):
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

        self.assertEqual(items.num_items, 0)

        items.add(item_names[0], 30)
        items.add(item_names[1], 99)
        items.add(item_names[2], 1)

        self.assertStringEqual(items[0].item, item_names[0])
        self.assertEqual(items[0].amount, 30)
        self.assertStringEqual(items[1].item, item_names[1])
        self.assertEqual(items[1].amount, 99)
        self.assertStringEqual(items[2].item, item_names[2])
        self.assertEqual(items[2].amount, 1)
        self.assertEqual(items.num_items, 3)

        items.add(item_names[2], 15)

        self.assertStringEqual(items[0].item, item_names[0])
        self.assertEqual(items[0].amount, 30)
        self.assertStringEqual(items[1].item, item_names[1])
        self.assertEqual(items[1].amount, 99)
        self.assertStringEqual(items[2].item, item_names[2])
        self.assertEqual(items[2].amount, 16)
        self.assertEqual(items.num_items, 3)

        items.remove(item_names[1], 20)

        self.assertStringEqual(items[0].item, item_names[0])
        self.assertEqual(items[0].amount, 30)
        self.assertStringEqual(items[1].item, item_names[1])
        self.assertEqual(items[1].amount, 79)
        self.assertStringEqual(items[2].item, item_names[2])
        self.assertEqual(items[2].amount, 16)
        self.assertEqual(items.num_items, 3)

        items.move(0, 1)

        self.assertStringEqual(items[0].item, item_names[1])
        self.assertEqual(items[0].amount, 79)
        self.assertStringEqual(items[1].item, item_names[0])
        self.assertEqual(items[1].amount, 30)
        self.assertStringEqual(items[2].item, item_names[2])
        self.assertEqual(items[2].amount, 16)
        self.assertEqual(items.num_items, 3)

        items.remove(item_names[0], 30)

        self.assertStringEqual(items[0].item, item_names[1])
        self.assertEqual(items[0].amount, 79)
        self.assertStringEqual(items[1].item, item_names[2])
        self.assertEqual(items[1].amount, 16)
        self.assertEqual(items[2].item, "None")
        self.assertEqual(items[2].amount, 0)
        self.assertEqual(items.num_items, 2)

        items.add(item_names[3], 90)

        self.assertStringEqual(items[0].item, item_names[1])
        self.assertEqual(items[0].amount, 79)
        self.assertStringEqual(items[1].item, item_names[2])
        self.assertEqual(items[1].amount, 16)
        self.assertStringEqual(items[2].item, item_names[3])
        self.assertEqual(items[2].amount, 90)
        self.assertEqual(items.num_items, 3)

        items.add(item_names[4], 2)

        self.assertStringEqual(items[0].item, item_names[1])
        self.assertEqual(items[0].amount, 79)
        self.assertStringEqual(items[1].item, item_names[2])
        self.assertEqual(items[1].amount, 16)
        self.assertStringEqual(items[2].item, item_names[3])
        self.assertEqual(items[2].amount, 90)
        self.assertStringEqual(items[3].item, item_names[4])
        self.assertEqual(items[3].amount, 2)
        self.assertEqual(items.num_items, 4)

        items.remove(item_names[1], 30)

        self.assertStringEqual(items[0].item, item_names[1])
        self.assertEqual(items[0].amount, 49)
        self.assertStringEqual(items[1].item, item_names[2])
        self.assertEqual(items[1].amount, 16)
        self.assertStringEqual(items[2].item, item_names[3])
        self.assertEqual(items[2].amount, 90)
        self.assertStringEqual(items[3].item, item_names[4])
        self.assertEqual(items[3].amount, 2)
        self.assertEqual(items.num_items, 4)

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
        self.assertEqual(items.num_items, 5)

        items.remove(item_names[2], 16)

        self.assertStringEqual(items[0].item, item_names[1])
        self.assertEqual(items[0].amount, 49)
        self.assertStringEqual(items[1].item, item_names[3])
        self.assertEqual(items[1].amount, 90)
        self.assertStringEqual(items[2].item, item_names[4])
        self.assertEqual(items[2].amount, 2)
        self.assertStringEqual(items[3].item, item_names[5])
        self.assertEqual(items[3].amount, 12)
        self.assertEqual(items.num_items, 4)

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
        self.assertEqual(items.num_items, 5)

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
        self.assertEqual(items.num_items, 6)

        items.remove(item_names[4], 2)
        items.remove(item_names[1], 49)
        items.remove(item_names[7], 6)
        items.remove(item_names[5], 12)
        items.remove(item_names[3], 90)
        items.remove(item_names[6], 65)

        self.assertEqual(items.num_items, 0)
