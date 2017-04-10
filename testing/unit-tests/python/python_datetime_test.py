#!/usr/bin/env python
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import os
import sys
import unittest

class datetime_test(unittest.TestCase):

    def assertInRange(self, num, min_num, max_num):
        self.assertGreaterEqual(num, min_num)
        self.assertLessEqual(num, max_num)

    def test_current_datetime(self):
        now = pkmn.current_datetime()
        self.assertGreaterEqual(now.year, 1900)
        self.assertInRange(now.month, 1, 12)
        self.assertInRange(now.day, 1, 31)
        self.assertInRange(now.hour, 0, 23)
        self.assertInRange(now.minute, 0, 59)
        self.assertInRange(now.second, 0, 59)
        self.assertEqual(0, now.frames)

    def test_datetime_equality(self):
        datetime1 = pkmn.datetime(1,2,3,4,5,6,7)
        datetime2 = pkmn.datetime(1,2,3,4,5,6,7)
        datetime3 = pkmn.datetime(3,2,1,4,5,6,7)

        self.assertEqual(datetime1, datetime2)
        self.assertNotEqual(datetime1, datetime3)
        self.assertNotEqual(datetime2, datetime3)

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(datetime_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
