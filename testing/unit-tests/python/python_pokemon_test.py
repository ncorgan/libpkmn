#!/usr/bin/env python
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmntest

import sys
import unittest

if __name__ == "__main__":
    suite1 = unittest.TestLoader().loadTestsFromTestCase(pkmntest.gen1_pokemon_test)
    test_program1 = unittest.TextTestRunner(verbosity=3).run(suite1)

    suite2 = unittest.TestLoader().loadTestsFromTestCase(pkmntest.gen2_pokemon_test)
    test_program2 = unittest.TextTestRunner(verbosity=3).run(suite2)

    suite3 = unittest.TestLoader().loadTestsFromTestCase(pkmntest.gen3_pokemon_test)
    test_program3 = unittest.TextTestRunner(verbosity=3).run(suite3)

    suite4 = unittest.TestLoader().loadTestsFromTestCase(pkmntest.pokemon_conversions_test)
    test_program4 = unittest.TextTestRunner(verbosity=3).run(suite4)

    suite5 = unittest.TestLoader().loadTestsFromTestCase(pkmntest.pokemon_io_test)
    test_program5 = unittest.TextTestRunner(verbosity=3).run(suite5)

    success = test_program1.wasSuccessful() and test_program2.wasSuccessful() and \
              test_program3.wasSuccessful() and test_program4.wasSuccessful() and \
              test_program4.wasSuccessful()

    sys.exit(0 if success else 1)
