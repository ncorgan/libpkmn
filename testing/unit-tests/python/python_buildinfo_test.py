#!/usr/bin/env python
#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import random
import sys
import unittest

class buildinfo_test(unittest.TestCase):

    def test_boost_version(self):
        self.assertGreater(len(pkmn.buildinfo.BOOST_VERSION), 0)

    def test_libpkmgc_version(self):
        self.assertGreater(len(pkmn.buildinfo.LIBPKMGC_VERSION), 0)

    def test_pksav_version(self):
        self.assertGreater(len(pkmn.buildinfo.PKSAV_VERSION), 0)

    def test_sqlite3_version(self):
        self.assertGreater(len(pkmn.buildinfo.SQLITE3_VERSION), 0)

    def test_sqlitecpp_version(self):
        self.assertGreater(len(pkmn.buildinfo.SQLITECPP_VERSION), 0)

    def test_swig_version(self):
        self.assertGreater(len(pkmn.buildinfo.SWIG_VERSION), 0)

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(buildinfo_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
