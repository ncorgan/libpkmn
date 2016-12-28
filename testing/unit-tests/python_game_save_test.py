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

class detect_game_save_type_test(unittest.TestCase):

    # TODO: Blue save
    def test_red_blue_yellow(self):
        pokemon_red = "@POKEMON_RED_SAV@"
        pokemon_yellow = "@POKEMON_YELLOW_SAV@"
        expected_result = "Red/Blue/Yellow"

        self.assertEqual(pkmn.detect_game_save_type(pokemon_red), expected_result)
        self.assertEqual(pkmn.detect_game_save_type(pokemon_yellow), expected_result)

    # TODO: Silver save
    def test_gold_silver(self):
        pokemon_gold = "@POKEMON_GOLD_SAV@"
        expected_result = "Gold/Silver"

        self.assertEqual(pkmn.detect_game_save_type(pokemon_gold), expected_result)

    def test_crystal(self):
        pokemon_crystal = "@POKEMON_CRYSTAL_SAV@"
        expected_result = "Crystal"

        self.assertEqual(pkmn.detect_game_save_type(pokemon_crystal), expected_result)

    # TODO: Ruby save
    def test_ruby_sapphire(self):
        pokemon_ruby = "@POKEMON_RUBY_SAV@"
        expected_result = "Ruby/Sapphire"

        self.assertEqual(pkmn.detect_game_save_type(pokemon_ruby), expected_result)

    def test_emerald(self):
        pokemon_emerald = "@POKEMON_EMERALD_SAV@"
        expected_result = "Emerald"

        self.assertEqual(pkmn.detect_game_save_type(pokemon_emerald), expected_result)

    # TODO: LeafGreen save
    def test_emerald(self):
        pokemon_firered = "@POKEMON_FIRERED_SAV@"
        expected_result = "FireRed/LeafGreen"

        self.assertEqual(pkmn.detect_game_save_type(pokemon_firered), expected_result)

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(detect_game_save_type_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
