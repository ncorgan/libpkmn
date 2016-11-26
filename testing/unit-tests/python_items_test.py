#!/usr/bin/env python
#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmntest

import sys
import unittest

if __name__ == "__main__":
    suite1 = unittest.TestLoader().loadTestsFromTestCase(pkmntest.gen1_items_test)
    test_program1 = unittest.TextTestRunner(verbosity=3).run(suite1)

    suite2 = unittest.TestLoader().loadTestsFromTestCase(pkmntest.gen2_items_test)
    test_program2 = unittest.TextTestRunner(verbosity=3).run(suite2)

    success = test_program1.wasSuccessful() and test_program2.wasSuccessful()

    sys.exit(0 if success else 1)
