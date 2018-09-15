#!/usr/bin/env python
#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import pkmntest

from nose_parameterized import parameterized
import unittest

import random
import sys

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(pkmntest.pokemon_polymorphism_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
