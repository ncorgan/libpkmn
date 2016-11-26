#!/usr/bin/env python
#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import os
import platform
import sys
import unittest

class paths_test(unittest.TestCase):

    def test_appdata_dir(self):
        if platform.system() == "Windows":
            appdata_dir = "C:\\libpkmn\\appdata\\dir"
        else:
            appdata_dir = "/libpkmn/appdata/dir"

        os.putenv("PKMN_APPDATA_DIR", appdata_dir)
        self.assertEqual(pkmn.get_appdata_dir(), appdata_dir)

    def test_database_path(self):
        #
        # When this unit test is run, the PKMN_DATABASE_PATH environment variable
        # is set to a valid value, so just make sure the call doesn't fail.
        #
        pkmn.get_database_path()

        # Cause a failure with an existing file that isn't a database
        with self.assertRaises(RuntimeError):
            os.putenv("PKMN_DATABASE_PATH", os.path.realpath(__file__))
            pkmn.get_database_path()

        # Cause a failure with a non-existent file
        if platform.system() == "Windows":
            database_path = "C:\\libpkmn\\database\\path"
        else:
            database_path = "/libpkmn/database/path"
        with self.assertRaises(RuntimeError):
            os.putenv("PKMN_DATABASE_PATH", database_path)
            pkmn.get_database_path()

    def test_images_dir(self):
        if platform.system() == "Windows":
            images_dir = "C:\\libpkmn\\images\\dir"
        else:
            images_dir = "/libpkmn/images/dir"

        os.putenv("PKMN_IMAGES_DIR", images_dir)
        self.assertEqual(pkmn.get_images_dir(), images_dir)

    @unittest.skip("TODO")
    def test_tmp_dir(self):
        pkmn.get_tmp_dir()

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(paths_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
