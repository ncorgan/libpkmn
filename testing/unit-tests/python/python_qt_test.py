#!/usr/bin/env python
#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import filecmp
import os
import sys
import unittest

class spinda_test(unittest.TestCase):
    def test_spinda(self):
        if pkmn.buildinfo.QT_VERSION != "N/A":
            libpkmn_test_files = os.environ["LIBPKMN_TEST_FILES"]
            tmp_dir = pkmn.paths.get_tmp_dir()

            spinda_dir = "spinda-qt{0}".format(pkmn.buildinfo.QT_VERSION[0])
            spinda_filename_format = "spinda_{0}_{1}_{2}.png"

            personality = 0x88888888
            personality_shiny = 0xF81C8021

            for generation in range(3, 6):
                test_files_spinda_filepath = os.path.join(
                                                 libpkmn_test_files,
                                                 spinda_dir,
                                                 spinda_filename_format.format(
                                                     generation,
                                                     "0", # Not shiny
                                                     personality
                                                 )
                                             )
                test_spinda_filepath = os.path.join(
                                           tmp_dir,
                                           spinda_filename_format.format(
                                               generation,
                                               "0", # Not shiny
                                               personality
                                           )
                                       )
                pkmn.qt.GenerateSpindaSpriteAtFilepath(
                    generation,
                    personality,
                    False,
                    test_spinda_filepath
                )
                self.assertTrue(os.path.exists(test_spinda_filepath))
                self.assertTrue(
                    filecmp.cmp(
                        test_files_spinda_filepath,
                        test_spinda_filepath
                    )
                )

                os.remove(test_spinda_filepath)

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(spinda_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)

    success = test_program.wasSuccessful()
    sys.exit(0 if success else 1)
